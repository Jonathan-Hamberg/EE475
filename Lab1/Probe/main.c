#include <xc.h>        /* XC8 General Include File */

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/drivers/i2c_slave.h"
#include "mcc_generated_files/i2c2_driver.h"

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

uint8_t active_camera_index = 0;
uint16_t active_camera_address = 0;

// Flags to indicate different operations.
volatile uint8_t flag_read_section;
volatile uint8_t flag_command_byte;
volatile uint8_t flag_probe_launched;

// Variables used to determine the state for the I2C code.
volatile uint8_t probe_command = COMMAND_STATUS_GET;
volatile uint8_t probe_status = STATUS_GET_NONE;

void i2cReadInterrupt(void) {
    probe_command = i2c2_driver_getRXData();

    switch (probe_command) {
        case COMMAND_STATUS_GET:
            break;
        case COMMAND_PROBE_LAUNCH:
            current_state = PROBE_LAUNCHED;
            i2c_slave_writeData = STATUS_PROBE_LAUNCHED;
            break;
        case COMMAND_PROBE_DATA:
            break;
        case COMMAND_DATA_REMAINING:
            break;
        case COMMAND_CAMERA_STATUS:
            break;
        default:
            break;
    }
}

void i2cWriteInterrupt(void) {
//    i2c2_driver_TXData(probe_status);
    i2c2_driver_TXData(0x32);

    //    switch (probe_command) {
    //        case COMMAND_STATUS_GET:
    //            i2c2_driver_TXData(probe_status);
    //            break;
    //        case COMMAND_PROBE_DATA:
    //            if (probe_data_address < 16) {
    //                // TODO(jrh) roll over the address when greater than 16
    //                i2c2_driver_TXData(probe_data_buffer[probe_data_address++]);
    //            } else {
    //
    //            }
    //            break;
    //        case COMMAND_DATA_REMAINING:
    //            // TODO(jrh) make sure this section is correct.
    //            i2c2_driver_TXData(16 - probe_data_section);
    //            break;
    //        default:
    //            i2c2_driver_TXData(0x01);
    //            break;
    //    }

}

void main(void) {
    // Initialize the system.
    SYSTEM_Initialize();

    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    // Start the timer to poll the button.
    TMR0_StartTimer();


    mssp2_enableIRQ();
    i2c_slave_open();
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

