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

typedef enum {
    READ,
    WRITE
} State;

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
State current_state = WRITE;

bool current_button = 0;
bool previous_button = 0;

uint16_t address = 0;

ProbeState probe_state = PROBE_STOWED;

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

uint16_t launchedCountdownTimer;

// Flags to indicate different operations.
volatile uint8_t flag_read_section;
volatile uint8_t flag_command_byte;
volatile uint8_t flag_probe_launched;

// Variables used to determine the state for the I2C code.
volatile uint8_t probe_command = COMMAND_STATUS_GET;
volatile uint8_t probe_status = STATUS_GET_NONE;

void main(void) {
    // Initialize the system.
    SYSTEM_Initialize();

    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    // Start the timer to poll the button.
    TMR0_StartTimer();

    
    
    //    i2c_slave_open();
    //  i2c_slave_setWriteIntHandler(i2cReadInterrupt);
    //    i2c_slave_setReadIntHandler(i2cWriteInterrupt);

    while (1) {

        // Determine if the timer has overflowed.
        if (TMR0IF) {
            // Get the button state.  Polled every 16 milli-seconds.
            previous_button = current_button;

            sramWrite(address, address & 0xFF);

            uint8_t data = sramRead(address);
            if (data == address & 0xFF) {
                setCameraState(0, CAMERA_STANDBY);
                setCameraState(1, CAMERA_STANDBY);
                setCameraState(2, CAMERA_STANDBY);
            } else {
                setCameraState(0, CAMERA_SHUTDOWN);
                setCameraState(1, CAMERA_SHUTDOWN);
                setCameraState(2, CAMERA_SHUTDOWN);
            }

            updateCameraLED();

            // Increment the address counter.
            address++;

            // Toggle the state machine state.
            if (!current_button && previous_button) {
            }

            //            if (SW1_GetValue()) {
            //                setCameraState(0, CAMERA_SHUTDOWN);
            //            } else {
            //                setCameraState(0, CAMERA_MALFUNCTION);
            //            }
            //
            //            if (SW2_GetValue()) {
            //                setCameraState(1, CAMERA_STANDBY);
            //            } else {
            //                setCameraState(1, CAMERA_MALFUNCTION);
            //            }
            //
            //            if (SW3_GetValue()) {
            //                setCameraState(2, CAMERA_FILMING);
            //            } else {
            //                setCameraState(2, CAMERA_MALFUNCTION);
            //            }


            // Handle the PROBE_LAUNCHED state.
            if (current_state == PROBE_LAUNCHED) {
                if (launchedCountdownTimer-- == 0) {
                    // Change the state to the landed position.
                    current_state = PROBE_LANDED;
                    // Set the update message to send back to the mothership.
                  //  i2c_slave_writeData = STATUS_GET_PROBE_LANDED;
                }
            }

            // Handle the PROBE_LANDED state.
            if (current_state == PROBE_LANDED) {
                if (camera_active_address >= BUFFER_100) {
                   // Stop filming with the active camera.
                    setCameraState(camera_active_index, CAMERA_SHUTDOWN);
                    
                    // Replace the store camera with the active camera.
                    camera_store_address = 0;
                    camera_store_base = camera_active_base;
                    camera_store_index = camera_active_index;
                    
                    // Find new parameters for the active camera.
                    camera_active_index = findOtherCamera(camera_active_index);
                    camera_active_base = 1024 - camera_active_base;
                    camera_active_address = 0;
                    
                    
                } else if (camera_active_address >= BUFFER_90) {
                    if(!startFilming(camera_store_index)) {
                        // TODO(jrh) handle the error case.
                    }
                } else if (camera_active_address >= BUFFER_80) {
                    // Send the request to transfer command to the mother-ship.
//                    i2c_slave_writeData = STATUS_GET_REQUEST_TO_TRANSFER;
                } else {

                }

                if (cameraHasData(camera_active_index)) {
                    // Write the current camera data to the SRAM.
                    sramWrite(camera_active_base + camera_active_address++,
                            getCameraData(camera_active_index));
                } else {
                    // TODO(jrh) handle malfunctioning camera.
                }
            }

            // clear the TMR0 interrupt flag
            TMR0IF = 0;

            // Reload the initial value of TMR0
            TMR0_Reload();
        }


        switch (current_state) {
            case READ:
                // Set the control pins to write the data to the SRAM.

                break;
            case WRITE:
                // Set the control pins to write the data to the SRAM.

                break;
            default:
                current_state = WRITE;
                break;
        }






        // 4ms delay.  Fill 1024 bytes in approximately 4s.
        __delay_ms(4);
    }
}

