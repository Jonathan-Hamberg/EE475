#include <xc.h>        /* XC8 General Include File */

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include "mcc_generated_files/mcc.h"

#include "sram.h"
#include "communication.h"
#include "camera.h"

/*
 * Definitions.
 */

#define BUFFER_100 1024
#define BUFFER_90 922
#define BUFFER_80 819

/**
 * This enumeration contains the different states that the probe can
 * be physically in.
 */
typedef enum {
    PROBE_STOWED = 0,
    PROBE_LAUNCHED,
    PROBE_LANDED,
    PROBE_DATA,
} ProbeState;

/*
 * Globals.
 */

ProbeState current_state = PROBE_STOWED;

// Variables to store the current position of the camera buffer.
uint8_t probe_data_section = 0;
uint8_t probe_data_address = 0;
uint8_t probe_data_buffer[16];

// Variables to store the state of the two active cameras.
uint16_t camera_one_address = 0;
uint16_t camera_two_address = 1024;

uint8_t camera_active_index = 0;
uint16_t camera_active_address = 0;
uint16_t camera_active_base = 0;

uint8_t camera_store_index = 0;
uint16_t camera_store_address = 0;
uint16_t camera_store_base = 1024;

volatile uint16_t launchedCountdownTimer = 500;

// Flags to indicate different operations.
volatile uint8_t flag_read_section;
volatile uint8_t flag_command_byte;
volatile uint8_t flag_probe_launched;

// Variables used to determine the state for the I2C code.
volatile uint8_t gCommand;
volatile uint8_t gStatus = STATUS_GET_NONE;
volatile uint8_t gReadSection = 0;

void I2C2_StatusCallback(I2C2_SLAVE_DRIVER_STATUS i2c_bus_state) {

    switch (i2c_bus_state) {
        case I2C2_SLAVE_WRITE_COMPLETED:

            gCommand = I2C2_slaveWriteData;
            switch (gCommand) {
                case COMMAND_PROBE_LAUNCH:
                    current_state = PROBE_LAUNCHED;
                    launchedCountdownTimer = 500;
                    gStatus = STATUS_GET_PROBE_LAUNCHED;
                    break;
                case COMMAND_CAMERA1_WORK:
                    privateSetCameraState(0, CAMERA_SHUTDOWN);
                    break;
                case COMMAND_CAMERA2_WORK:
                    privateSetCameraState(1, CAMERA_SHUTDOWN);
                    break;
                case COMMAND_CAMERA3_WORK:
                    privateSetCameraState(2, CAMERA_SHUTDOWN);
                    break;
                case COMMAND_CAMERA1_MALFUNCTION:
                    privateSetCameraState(0, CAMERA_MALFUNCTION);
                    break;
                case COMMAND_CAMERA2_MALFUNCTION:
                    privateSetCameraState(1, CAMERA_MALFUNCTION);
                    break;
                case COMMAND_CAMERA3_MALFUNCTION:
                    privateSetCameraState(2, CAMERA_MALFUNCTION);
                    break;

                default:;
            }

        case I2C2_SLAVE_READ_REQUEST:
            switch (gCommand) {
                case COMMAND_STATUS_GET:
                    SSP2BUF = gStatus;
                    break;
                case COMMAND_PROBE_DATA:
                    // Return with the data to be sent to the mother-ship.
                    if (probe_data_address < 16 && probe_data_section < 64) {
                        SSP2BUF = probe_data_buffer[probe_data_address++];
                    } else {
                        SSP2BUF = 0xDB;
                    }

                    if (probe_data_address)
                        // Handle finished section transmitting.
                        if (probe_data_address >= 16) {
                            // Indicate to the main thread that the next section
                            // Needs to be read from memory.
                            gReadSection = 1;

                            // Set the status that is reported to the mother-ship.
                            if (probe_data_section >= 64) {
                                // Done transmitting all the sections.
                                gStatus = STATUS_GET_NONE;
                            } else {
                                // Set ready to transfer but need load the section
                                // from memory.
                                gStatus = STATUS_GET_READY_TO_TRANSFER;

                                // Increment the section that is being transmitted.
                            }
                        }
                    break;
                case COMMAND_DATA_SECTION:
                    // Return with the current section that is being
                    // transmitted.
                    SSP2BUF = probe_data_section;
                    break;
                case COMMAND_CAMERA_STATUS:
                    SSP2BUF = getCameraStateByte();
                    break;
                default:
                    // Un-handled case just always return 0.
                    SSP2BUF = 0;
            }

        case I2C2_SLAVE_READ_COMPLETED:
        default:;

    } // end switch(i2c_bus_state)

}

void main(void) {
    // Initialize the system.
    SYSTEM_Initialize();

    // Enable interrupts for the I2C subsystem.
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    // Start the timer to poll the button.
    TMR0_StartTimer();

    while (1) {

        // Determine if the timer has overflowed.
        if (TMR0IF) {

            // Handle the PROBE_LAUNCHED state.
            if (current_state == PROBE_LAUNCHED) {
                // Check to see if the launched countdown has finished.
                // If so start the filming process and update the status.
                if (launchedCountdownTimer-- == 0) {
                    // Change the state to the landed position.
                    current_state = PROBE_LANDED;
                    gStatus = STATUS_GET_PROBE_LANDED;
                    camera_active_base = 0;
                    camera_active_address = 0;
                    // Find a working camera to start filming with.
                    camera_active_index = (uint8_t) findOtherCamera(-1);
                    // Start filming with the working camera.
                    // No working cameras are found the startFilming function
                    // will do nothering.
                    startFilming(camera_active_index);
                }
            }


            // Handle the PROBE_LANDED state.
            if (current_state == PROBE_LANDED) {
                // Check to see if the active camera has data.  If the camera
                // does not have data.  Find another camera that is working.
                if (cameraHasData(camera_active_index)) {
                    // Write the current camera data to the SRAM.
                    sramWrite(camera_active_base + camera_active_address++,
                            getCameraData(camera_active_index));
                } else {
                    // Check to see if there is any other camera available.
                    uint8_t available = (uint8_t)(findOtherCamera(-1));

                    // Make sure the camera is valid.  If no camera is available
                    // the function will return -1.
                    if (available >= 0 && available < 3) {
                        // Update the active camera index.
                        camera_active_index = available;
                        // Start filming with the new active camera.
                        startFilming(camera_active_index);
                        // Write the camera data to the SRAM.
                        sramWrite(camera_active_base + camera_active_address++,
                                getCameraData(camera_active_index));
                    }

                    // If no cameras are not available.  Do not collect any data.
                    // Just wait until a camera is available.

                }

                if (camera_active_address >= BUFFER_100) {
                    // Stop filming with the active camera.
                    setCameraState(camera_active_index, CAMERA_SHUTDOWN);

                    // Replace the store camera with the active camera.
                    camera_store_address = 0;
                    camera_store_base = camera_active_base;
                    camera_store_index = camera_active_index;

                    // Find new parameters for the active camera.
                    camera_active_index = (uint8_t) findOtherCamera(camera_active_index);
                    camera_active_base = 1024 - camera_active_base;
                    camera_active_address = 0;

                    // Load the data from the camera buffer.
                    gReadSection = 1;
                } else if (camera_active_address == BUFFER_90) {
                    // Find another camera to start filming.  Essentially make
                    // sure the camera is working when it is expected to start
                    // filming.
                    camera_store_index = (uint8_t) findOtherCamera(camera_active_index);
                    // Tell the camera to start filming.
                    startFilming(camera_store_index);
                } else if (camera_active_address == BUFFER_80) {
                    // Send the request to transfer command to the mother-ship.
                    gStatus = STATUS_GET_REQUEST_TO_TRANSFER;
                }
            }

            // clear the TMR0 interrupt flag
            TMR0IF = 0;

            // Reload the initial value of TMR0
            TMR0_Reload();
        }

        // Read data from the SRAM.
        if (gReadSection) {
            // Increment the probe_data_section and reset the address.
            probe_data_address = 0;

            // Lead the buffer data into the 16 byte buffer.
            for (uint8_t i = 0; i < 16; i++) {
                // TODO(jrh) add after testing.
                probe_data_buffer[i] =
                        sramRead(camera_store_base + 16u * probe_data_section
                        + i);
            }

            gStatus = STATUS_GET_READY_TO_TRANSFER;

            // Disable the read section flag.
            gReadSection = 0;
        }
    }
}

