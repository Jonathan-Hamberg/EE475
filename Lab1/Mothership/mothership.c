#include <xc.h>        /* XC8 General Include File */
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "mothership.h"
#include "communication.h"

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/eusart1.h"

#define ADDRESS 0x50
#define SECTION_SIZE 16
#define SECTIONS 64
#define BYTE 1
#define EARTH_BUFFER_SIZE 3

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

uint8_t I2C_ReadCommand(uint8_t command) {
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
        
        
        // write one byte to EEPROM (3 is the number of bytes to write)
        I2C2_MasterRead(buffer,
                1,
                0x50,
                &status);

        PIR3bits.SSP2IF = true;

        // wait for the message to be sent or status has changed.
        while (status == I2C2_MESSAGE_PENDING);
        
        // Return the message from the slave.
        return buffer[0];
        
}

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
    byte ^= (byte >> 4);  
    byte ^= (byte >> 2); 
    byte ^= (byte >> 1);  
    return (byte & 1); 
}


uint8_t RS232_ReadByte() {
    return EUSART1_Read() & 0x7F; 
}

void RS232_WriteByte(uint8_t byte) {
    uint8_t parity = getOddParity(byte);
    EUSART1_Write((parity << 7) | byte);
}


void RS232_WriteNByte(uint8_t * buffer, uint8_t size) {
    for(int byte = 0; byte < size; byte++) {
        RS232_WriteByte(*buffer);
        buffer++;
    }
}


void RS232_WriteString(char * string) {
    while(*string != NULL) {
        RS232_WriteByte(*string);
        string++; 
    }
}

/**
 * 
 */
void mothership_setup() {
    // Initializes communication systems
    SYSTEM_Initialize();

    // Enable Global and Peripheral Interrupts
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    RS232_WriteString("Welcome to the Space Explorer Earth Station \n\r");
    RS232_WriteString("To launch the probe from the mothership, press 's'\n\r");
    
    // Waits for Earth command
    while(earth_command != EARTH_PROBE_LAUNCH) {
        earth_command = RS232_ReadByte();
    }
    
    RS232_WriteString("Launching Probe");
    
    // Send launch command
    mothership_command = COMMAND_PROBE_LAUNCH; 
    //__delay_ms(5000);
    I2C_WriteCommand(mothership_command);
    
    // Read probe launched signal
    __delay_ms(10);
    mothership_command = COMMAND_STATUS_GET;
    I2C_ReadData(&probe_status, mothership_command, BYTE);
    while (probe_status != STATUS_GET_PROBE_LAUNCHED) {
        RS232_WriteString("...");
        __delay_ms(10);
        mothership_command = COMMAND_STATUS_GET;
        __delay_ms(10);
        I2C_ReadData(&probe_status, mothership_command, BYTE);
    }
    
    RS232_WriteString("\n\rProbe successfully launched\n\r");
}

void mothership_loop() {
    switch(mothership_state) {
        case MOTHERSHIP_CHECK_PROBE: 
            if (probe_status == STATUS_GET_READY_TO_TRANSFER) {
                mothership_state = MOTHERSHIP_COLLECT_DATA;
            }
            __delay_ms(5);
            mothership_command = COMMAND_STATUS_GET;
        
            I2C_ReadData(&probe_status, mothership_command, BYTE);
            break;
        case MOTHERSHIP_COLLECT_DATA:
            section = 0;
            while(section < SECTIONS) { 
                mothership_command = COMMAND_STATUS_GET;
                
                I2C_ReadData(&probe_status, mothership_command, BYTE);
                if (probe_status == STATUS_GET_READY_TO_TRANSFER) {
                    mothership_command = COMMAND_PROBE_DATA;
                    I2C_ReadData(&probe_data_section, mothership_command, 16*BYTE);
                    section++;
                }
                __delay_us(500);
            }
            break;
    }
}



