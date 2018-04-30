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
 * 
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

uint8_t sw1_current = 0, sw1_previous = 0;
uint8_t sw2_current = 0, sw2_previous = 0;
uint8_t sw3_current = 0, sw3_previous = 0;

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

                default:;
            }

        case I2C2_SLAVE_READ_REQUEST:
            switch (gCommand) {
                case COMMAND_STATUS_GET:
                    SSP2BUF = gStatus;
                    break;
                case COMMAND_PROBE_LAUNCH:
                    current_state = PROBE_LAUNCHED;
                    launchedCountdownTimer = 1000;
                    gStatus = STATUS_GET_PROBE_LAUNCHED;
                    break;
                case COMMAND_PROBE_DATA:
                    // Return with the data to be sent to the mother-ship.
                    if(probe_data_address < 16 && probe_data_section < 64) {
                        SSP2BUF = probe_data_buffer[probe_data_address++];
                    } else {
                        SSP2BUF = 0xDB;
                    }
                    
                    if(probe_data_address)

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
    
    if(SW1_GetValue()) {
        privateSetCameraState(0, CAMERA_SHUTDOWN);
    } else {
        privateSetCameraState(0, CAMERA_MALFUNCTION);
    }
    
    if(SW2_GetValue()) {
        privateSetCameraState(1, CAMERA_SHUTDOWN);
    } else {
        privateSetCameraState(1, CAMERA_MALFUNCTION);
    }
    
    if(SW3_GetValue()) {
        privateSetCameraState(2, CAMERA_SHUTDOWN);
    } else {
        privateSetCameraState(2, CAMERA_MALFUNCTION);
    }

    while (1) {

        // Determine if the timer has overflowed.
        if (TMR0IF) {
            // Perform input edge detection on the switches.
            sw1_previous = sw1_current;
            sw2_previous = sw2_current;
            sw3_previous = sw3_current;
            sw1_current = SW1_GetValue();
            sw2_current = SW2_GetValue();
            sw3_current = SW3_GetValue();

            // Enable and disable camera 1.
            if (sw1_current ^ sw1_previous) {
                if (sw1_current) {
                    privateSetCameraState(0, CAMERA_SHUTDOWN);
                } else {
                    privateSetCameraState(0, CAMERA_MALFUNCTION);
                }
            }

            // Enable and disable camera 2.
            if (sw2_current ^ sw2_previous) {
                if (sw2_current) {
                    privateSetCameraState(1, CAMERA_SHUTDOWN);
                } else {
                    privateSetCameraState(1, CAMERA_MALFUNCTION);
                }
            }

            // Enable and disable camera 3.
            if (sw3_current ^ sw3_previous) {
                if (sw3_current) {
                    privateSetCameraState(2, CAMERA_SHUTDOWN);
                } else {
                    privateSetCameraState(2, CAMERA_MALFUNCTION);
                }
            }

            // Handle the PROBE_LAUNCHED state.
            if (current_state == PROBE_LAUNCHED) {
                if (launchedCountdownTimer-- == 0) {
                    // Change the state to the landed position.
                    current_state = PROBE_LANDED;
                    gStatus = STATUS_GET_PROBE_LANDED;
                    camera_active_base = 0;
                    camera_active_address = 0;
                    // TODO(jrh) check for case of all cameras malfunctioning.
                    camera_active_index = (uint8_t)findOtherCamera(-1);
                    // TODO(jrh) check for case that the camera cannot start filming.
                    startFilming(camera_active_index);
                }
            }


            // Handle the PROBE_LANDED state.
            if (current_state == PROBE_LANDED) {

                if (cameraHasData(camera_active_index)) {
                    // Write the current camera data to the SRAM.
                    // TODO(jrh) add after testing sram.
                    sramWrite(camera_active_base + camera_active_address++,
                            getCameraData(camera_active_index));
                } else {
                    // Check to see if there is any other camera available.
                    int8_t available = findOtherCamera(-1);
                    
                    if(available >= 0 < 3) {
                        camera_active_index = available;
                        startFilming(camera_active_index);
                        
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
                    camera_active_index = (uint8_t)findOtherCamera(camera_active_index);
                    camera_active_base = 1024 - camera_active_base;
                    camera_active_address = 0;

                    // Load the data from the camera buffer.
                    gReadSection = 1;
                } else if (camera_active_address == BUFFER_90) {
                    // TODO(jrh) check to see there is another camera available.
                    camera_store_index = (uint8_t)findOtherCamera(camera_active_index);
                    // TODO(jrh) if the only camera left is the active camera figure out what to do.
                    if (!startFilming(camera_store_index)) {
                        // TODO(jrh) handle the error case.
                    }
                } else if (camera_active_address == BUFFER_80) {
                    // Send the request to transfer command to the mother-ship.
                    gStatus = STATUS_GET_REQUEST_TO_TRANSFER;
                }
            }

            // Update the camera LED status lights.
            updateCameraLED();

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

