#include "probe.h"
#include "sram.h"
#include "camera.h"
#include "communication.h"

#include "mcc_generated_files/drivers/i2c_slave.h"
#include "mcc_generated_files/i2c1_driver.h"
#include "mcc_generated_files/mcc.h"

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
    probe_command = i2c1_driver_getRXData();
}

void i2cWriteInterrupt(void) {
    switch (probe_command) {
        case COMMAND_STATUS_GET:
            i2c1_driver_TXData(probe_status);
            break;
        case COMMAND_PROBE_DATA:
            if (probe_data_address < 16) {
                // TODO(jrh) roll over the address when greater than 16
                i2c1_driver_TXData(probe_data_buffer[probe_data_address++]);
            } else {
                
            }
            break;
        case COMMAND_DATA_REMAINING:
            // TODO(jrh) make sure this section is correct.
            i2c1_driver_TXData(16 - probe_data_section);
            break;
        default:
            i2c1_driver_TXData(0x01);
            break;
    }

}

void probe_setup() {
    mssp1_enableIRQ();
    i2c_slave_open();
    i2c_slave_setWriteIntHandler(i2cReadInterrupt);
    i2c_slave_setReadIntHandler(i2cWriteInterrupt);
}

void probe_loop() {
    if (flag_read_section) {
        // Read new section into memory to transmit the camera data.
        for (uint8_t i = 0; i < 16; i++) {
            probe_data_buffer[i] = sramRead(probe_data_section * 16u + i);
        }
    }

    // Run the probe state machine.
    switch (probe_state) {
        case PROBE_STOWED:
            if (flag_probe_launched) {
                probe_state = PROBE_LAUNCHED;
            }
            break;
        case PROBE_LAUNCHED:
            // TODO(jrh) add timer to wait for certain amount of time before landing.
            break;
        case PROBE_LANDED:
            break;
        case PROBE_DATA:
            break;
        default:
            break;
    }
    // Determine if the timer has overflowed.
    if (TMR0IF) {
        uint8_t data;

        // TODO(jrh) check for errors.
        getCameraData(active_camera_index, &data);

        // Write the data to the SRAM buffer.
        sramWrite(active_camera_address++, data);

        // Check for 80% buffer full.

        // Check for 90% buffer full.

        // Check for 100% buffer full.

        // Handle 2 working cameras.

        // Handle 1 working camera.

        // Handle 0 working cameras.

        // clear the TMR0 interrupt flag
        TMR0IF = 0;

        // Reload the initial value of TMR0
        TMR0_Reload();
    }
}
