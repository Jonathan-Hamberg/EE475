#include <xc.h>        /* XC8 General Include File */
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "../Probe/communication.h"
#include "../Probe/camera.h"

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/eusart1.h"

#define ADDRESS 0x50
#define SECTION_SIZE 18
#define SECTIONS 64
#define BYTE 1
#define EARTH_PROBE_LAUNCH 's'

typedef enum {
    MOTHERSHIP_CHECK_PROBE = 0,
    MOTHERSHIP_PERMISSION_DENIED,
    MOTHERSHIP_REQUEST_TRANSFER,
    MOTHERSHIP_COLLECT_DATA,
} MothershipState;

MothershipState mothership_state = MOTHERSHIP_CHECK_PROBE;

// Mothership data buffers
volatile uint8_t probe_data_section[SECTION_SIZE];
volatile uint8_t section = 0;

// Tracking probe state
volatile uint8_t probe_status;

// Mothership commands
volatile uint8_t mothership_command;

// Earth commands
volatile uint8_t earth_command = 0;
volatile char * earth_console;

// Camera probe camera status.
uint8_t currentCameraStatus = 0;
uint8_t previousCameraStatus = 0;

/**
 * @param Command to write to the slave I2C device.
 */
void I2C_WriteCommand(uint8_t command) {
    I2C2_MESSAGE_STATUS status = I2C2_MESSAGE_PENDING;

    uint8_t buffer[1];
    buffer[0] = command;

    // write one byte to EEPROM (3 is the number of bytes to write)
    I2C2_MasterWrite(buffer,
            1,
            0x50,
            &status);

    PIR3bits.SSP2IF = true;

    // wait for the message to be sent or status has changed.
    while (status == I2C2_MESSAGE_PENDING);
}

/**
 * Read N bytes from the I2C slave device.
 * @param buffer The buffer to store the data in.
 * @param command The command to read from the device.
 * @param size The number of bytes being requested.
 * 
 */
void I2C_ReadData(uint8_t* buffer, uint8_t command, uint8_t size) {
    I2C2_MESSAGE_STATUS status = I2C2_MESSAGE_PENDING;

    buffer[0] = command;

    // write one byte to EEPROM (3 is the number of bytes to write)
    I2C2_MasterWrite(buffer,
            1,
            0x50,
            &status);

    PIR3bits.SSP2IF = true;

    // wait for the message to be sent or status has changed.
    while (status == I2C2_MESSAGE_PENDING);


    // write one byte to EEPROM (3 is the number of bytes to write)
    I2C2_MasterRead(buffer,
            size,
            0x50,
            &status);

    PIR3bits.SSP2IF = true;

    // wait for the message to be sent or status has changed.
    while (status == I2C2_MESSAGE_PENDING);
}

/**
 * Returns the byte with odd parity at the MSB. 
 */
uint8_t getOddParity(uint8_t byte) {
    byte ^= (uint8_t) (byte >> 4);
    byte ^= (uint8_t) (byte >> 2);
    byte ^= (uint8_t) (byte >> 1);
    return (uint8_t) (byte & 1);
}

/*
 * @return A byte from the RS232 console masking out the parity bit.
 */
uint8_t RS232_ReadByte() {
    return (uint8_t) (EUSART1_Read() & 0x7F);
}

/**
 * @param byte The byte to be sent to the RS232 console.
 */
void RS232_WriteByte(uint8_t byte) {
    uint8_t parity = getOddParity(byte);
    EUSART1_Write((uint8_t) ((uint8_t) (parity << 7) | byte));
}

/**
 * @param buffer The buffer to be sent to the RS232 console.
 * @param length The length of the data to be sent to the RS232 console.
 */
void RS232_WriteNBytes(uint8_t* buffer, uint8_t length) {
    for (uint8_t i = 0; i < length; i++) {
        RS232_WriteByte(buffer[i]);
    }
}

/**
 * @param string String to be printed to the RS232 console.
 */
void RS232_WriteString(const char * string) {
    while (*string != NULL) {
        // Make sure not sending backspace and start of text.
        RS232_WriteByte(*string);
        string++;
    }
}

/**
 * 
 */
void main(void) {
    // Initializes communication systems
    SYSTEM_Initialize();

    // Enable Global and Peripheral Interrupts
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    // Delay before running the rest of the program.
    __delay_ms(100);

    RS232_WriteString("Welcome to the Space Explorer Earth Station \n");
    RS232_WriteString("To launch the probe from the mothership, press 's'\n");

    // Waits for Earth command
    do {
        earth_command = RS232_ReadByte();
    } while (earth_command != EARTH_PROBE_LAUNCH);

    RS232_WriteString("Launching Probe");

    // Send the launch command to the probe.
    I2C_WriteCommand(COMMAND_PROBE_LAUNCH);

    // Poll the probe until it has reported as landed.
    do {
        // Display status updates to the console.
        RS232_WriteString(".");
        __delay_ms(150);
        // Query the probe.
        I2C_ReadData((uint8_t*) & probe_status, COMMAND_STATUS_GET, BYTE);
    } while (probe_status == STATUS_GET_PROBE_LAUNCHED);

    RS232_WriteString("\nProbe successfully launched\n");

    uint8_t rs232Buffer[2] = {0, 0};

    while (1) {
        // Read a byte into the rs-232 buffer.
        if (EUSART1_DataReady) {
            rs232Buffer[1] = rs232Buffer[0];
            rs232Buffer[0] = RS232_ReadByte();
        }

        // Determine if any probe commands need to be send to the probe.
        if (rs232Buffer[1] == 'e') {
            // An enable command needs to be send.
            if (rs232Buffer[0] == '0') {
                I2C_WriteCommand(COMMAND_CAMERA1_WORK);
                RS232_WriteString("Camera 0 Enable\n");
            } else if (rs232Buffer[0] == '1') {
                I2C_WriteCommand(COMMAND_CAMERA2_WORK);
                RS232_WriteString("Camera 1 Enable\n");
            } else if (rs232Buffer[0] == '2') {
                I2C_WriteCommand(COMMAND_CAMERA3_WORK);
                RS232_WriteString("Camera 2 Enable\n");
            }
        } else if (rs232Buffer[1] == 'd') {
            if (rs232Buffer[0] == '0') {
                I2C_WriteCommand(COMMAND_CAMERA1_MALFUNCTION);
                RS232_WriteString("Camera 0 Malfunction\n");
            } else if (rs232Buffer[0] == '1') {
                I2C_WriteCommand(COMMAND_CAMERA2_MALFUNCTION);
                RS232_WriteString("Camera 1 Malfunction\n");
            } else if (rs232Buffer[0] == '2') {
                I2C_WriteCommand(COMMAND_CAMERA3_MALFUNCTION);
                RS232_WriteString("Camera 2 Malfunction\n");
            }
        }

        // Reset the buffer if a command is found.
        if ((rs232Buffer[1] == 'e' || rs232Buffer[1] == 'd') &&
                (rs232Buffer[0] >= '0' && rs232Buffer[0] <= '2')) {
            rs232Buffer[0] = 0;
            rs232Buffer[1] = 0;
        }

        // Query the camera status from the probe.
        previousCameraStatus = currentCameraStatus;
        I2C_ReadData((uint8_t*) & currentCameraStatus, COMMAND_CAMERA_STATUS, BYTE);

        // Update the camera status to the console if necessary.
        if (currentCameraStatus != previousCameraStatus) {
            uint8_t cameraState = currentCameraStatus;
            for (uint8_t i = 0; i < 3; i++) {
                // Get the camera state at the lowest two bytes of the state.
                CameraState cState = (CameraState) (cameraState & 0x03);

                // Write the camera state to the console.
                if (cState == CAMERA_SHUTDOWN) {
                    RS232_WriteByte('S');
                } else if (cState == CAMERA_FILMING) {
                    RS232_WriteByte('F');
                } else if (cState == CAMERA_MALFUNCTION) {
                    RS232_WriteByte('M');
                } else {
                    RS232_WriteByte('?');
                }

                // Shift in the next camera state.
                cameraState >>= 2;
            }

            // Print newline to the console.
            RS232_WriteByte('\n');
        }

        // Determine what logic to communicate with the probe.
        switch (mothership_state) {
            case MOTHERSHIP_CHECK_PROBE:
                if (probe_status == STATUS_GET_REQUEST_TO_TRANSFER) {
                    mothership_state = MOTHERSHIP_REQUEST_TRANSFER;
                    RS232_WriteString("Request To Transfer (y/n):\n");
                    // Clear the input buffer.
                    rs232Buffer[0] = 0;
                    rs232Buffer[1] = 0;
                }

                __delay_ms(5);

                I2C_ReadData((uint8_t*) & probe_status, COMMAND_STATUS_GET, BYTE);
                break;

            case MOTHERSHIP_PERMISSION_DENIED:

                I2C_ReadData((uint8_t*) & probe_status, COMMAND_STATUS_GET, BYTE);
                if (probe_status != STATUS_GET_REQUEST_TO_TRANSFER) {
                    mothership_state = MOTHERSHIP_CHECK_PROBE;
                }

                __delay_ms(5);
                break;
            case MOTHERSHIP_REQUEST_TRANSFER:
                // Move to data collection state if yes received from earth.
                if (rs232Buffer[0] == 'y') {
                    mothership_state = MOTHERSHIP_COLLECT_DATA;
                    RS232_WriteString("Earth Permission Granted.\n");
                    // Acknowledge the 'yes' answer.
                    rs232Buffer[0] = 0;
                    section = 0;
                } else if (rs232Buffer[0] == 'n') {
                    mothership_state = MOTHERSHIP_PERMISSION_DENIED;
                    RS232_WriteString("Earth Permission Denied.\n");
                    // Acknowledge the 'no' answer.
                    rs232Buffer[0] = 0;
                    rs232Buffer[1] = 0;
                }

                __delay_ms(5);

                // Move to check probe state if no received from earth.
                I2C_ReadData((uint8_t*) & probe_status, COMMAND_STATUS_GET, BYTE);
                if (probe_status == STATUS_GET_READY_TO_TRANSFER) {
                    mothership_state = MOTHERSHIP_CHECK_PROBE;
                    RS232_WriteString("Earth Permission Not Found.\n");
                }
                break;


                // Move to check probe state get state is not ready to transfer.
            case MOTHERSHIP_COLLECT_DATA:

                if (section < SECTIONS) {
                    // Get the probe status.
                    I2C_ReadData((uint8_t*) & probe_status, COMMAND_STATUS_GET, BYTE);

                    // If ready to transfer read 16 bytes from the probe.
                    if (probe_status == STATUS_GET_READY_TO_TRANSFER) {
                        // Read the data from the probe.
                        I2C_ReadData((uint8_t*) probe_data_section, COMMAND_PROBE_DATA, 18);

                        // Send the data to earth.
                        RS232_WriteNBytes((uint8_t*) (probe_data_section + 2), 16);

                        // Print valid marker.
                        if (probe_data_section[0] == 0x02 && probe_data_section[1] == 0x88) {
                            RS232_WriteString(" V ");
                        } else {
                            RS232_WriteString("   ");
                        }

                        if (((section + 1) & 0x03) == 0 && section != 0) {
                            RS232_WriteByte('\n');
                        }

                        section++;
                    }

                } else {

                    mothership_state = MOTHERSHIP_CHECK_PROBE;
                    RS232_WriteString("\nTransmission Complete.\n");
                }

                break;
        }
    }
}

