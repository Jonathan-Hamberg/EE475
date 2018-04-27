#include <xc.h>        /* XC8 General Include File */
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "mothership.h"
#include "communication.h"

#include "mcc_generated_files/mcc.h"


#define ADDRESS 0x50
#define SECTION_SIZE 16
#define SECTIONS 64
#define BYTE 1



MothershipState mothership_state = MOTHERSHIP_CHECK_PROBE;

// Mothership data buffers
volatile uint8_t probe_data_section[SECTION_SIZE];
volatile uint8_t section = 0;

// Tracking probe state
volatile uint8_t probe_status;

// Mothership commands
volatile uint8_t mothership_command;

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

void mothership_read_data() {
    for (int section = 0; section < SECTIONS; section++) {
        //        probe_status = i2c_read1ByteRegister(ADDRESS, COMMAND_STATUS_GET);
        if (probe_status == STATUS_GET_READY_TO_TRANSFER_SECTION) {
            //            i2c_readDataBlock(ADDRESS, COMMAND_PROBE_DATA, probe_data_section, SECTION_SIZE);
        }
        __delay_us(500);
    }
}

/**
 * 
 */
void mothership_setup() {
    // Initializes systems
    SYSTEM_Initialize();

    // Enable Global and Peripheral Interrupts
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    
//    // I2C2 communications status 
//    I2C2_MESSAGE_STATUS status = I2C2_MESSAGE_PENDING;
//    
    // Send launch command
    __delay_ms(5000);
    mothership_command = COMMAND_PROBE_LAUNCH;
    I2C_WriteCommand(mothership_command);
    
    // Read probe launched signal
    __delay_ms(10);
    mothership_command = COMMAND_STATUS_GET;
    I2C_ReadData(&probe_status, mothership_command, BYTE);
    while (probe_status != STATUS_GET_PROBE_LAUNCHED) {
        __delay_ms(10);
        mothership_command = COMMAND_STATUS_GET;
      //  I2C_WriteCommand(mothership_command);
        
        __delay_ms(10);
        I2C_ReadData(&probe_status, mothership_command, BYTE);
    }
    
//    // wait for the message to be sent or status has changed.
//    while (status == I2C2_MESSAGE_PENDING);
//    
//    // Enable interrupts 
//    //    mssp2_enableIRQ();
//
//    // Test code
//    while (1) {
//
//        I2C2_MESSAGE_STATUS status = I2C2_MESSAGE_PENDING;
//        uint8_t buffer[3] = {0x11, 0x22, 0x33};
//        uint8_t bufferRead[3];
//
//        // write one byte to EEPROM (3 is the number of bytes to write)
//        I2C2_MasterWrite(buffer,
//                3,
//                0x50,
//                &status);
//
//        PIR3bits.SSP2IF = true;
//
//        // wait for the message to be sent or status has changed.
//        while (status == I2C2_MESSAGE_PENDING);
//
//        __delay_ms(4);
//
//        // write one byte to EEPROM (3 is the number of bytes to write)
//        I2C2_MasterRead(buffer,
//                3,
//                0x50,
//                &status);
//
//        PIR3bits.SSP2IF = true;
//
//        // wait for the message to be sent or status has changed.
//        while (status == I2C2_MESSAGE_PENDING);
//
//        __delay_ms(4);
//    }

    //    uint8_t launch = 0xAA;
    //    
    //    while(1) {
    //        uint8_t buffer[3];
    //        launch = 0x11;
    //        i2c_writeNBytes(ADDRESS, &launch, 1);
    //        launch = 0x22;
    //        
    //        i2c_writeNBytes(ADDRESS, &launch, 1);
    //        launch = 0x33;
    //        i2c_writeNBytes(ADDRESS, &launch, 1);
    //        i2c_readNBytes(0x50, buffer, 3);
    ////        uint8_t launch = 0xAA;
    ////        i2c_writeNBytes(ADDRESS, &launch, 1);
    ////        __delay_ms(4);
    ////        probe_status = i2c_read1ByteRegister(ADDRESS, COMMAND_STATUS_GET);
    //        __delay_ms(4);
    //    }
    // Start timer to poll launch signal
//    TMR0_StartTimer();
//
//    uint8_t launch = COMMAND_PROBE_LAUNCH;
//
//    // Launch probe
//    //    i2c_writeNBytes(ADDRESS, &launch, 1);
//
//    // Get probe status
//    //    probe_status = i2c_read1ByteRegister(ADDRESS, COMMAND_STATUS_GET);
//
//    while (probe_status != STATUS_GET_PROBE_LAUNCHED) {
//        if (TMR0IF) {
//            launch = COMMAND_PROBE_LAUNCH;
//            //            i2c_writeNBytes(ADDRESS, &launch, 1);
//
//            //            probe_status = i2c_read1ByteRegister(ADDRESS, COMMAND_STATUS_GET);
//
//            TMR0IF = 0;
//            TMR0_Reload();
//        }
//    }
//
//    TMR0_StopTimer();
}

void mothership_loop() {
    switch(mothership_state) {
        case MOTHERSHIP_CHECK_PROBE: 
            if (probe_status == STATUS_GET_READY_TO_TRANSFER) {
                mothership_state = MOTHERSHIP_COLLECT_DATA;
            }
            __delay_ms(5);
            mothership_command = COMMAND_STATUS_GET;
           // I2C_WriteCommand(mothership_command);
        
            I2C_ReadData(&probe_status, mothership_command, BYTE);
            break;
        case MOTHERSHIP_COLLECT_DATA:
            section = 0;
            while(section < SECTIONS) { 
                mothership_command = COMMAND_STATUS_GET;
                //I2C_WriteCommand(mothership_command);
                
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
//    // Start timer to poll probe status
//    TMR0_StartTimer();
//
//    // Switches between the different mothership operations
//    switch (mothership_state) {
//        case MOTHERSHIP_CHECK_PROBE:
//            if (TMR0IF) {
//                //                probe_status = i2c_read1ByteRegister(ADDRESS, COMMAND_STATUS_GET);
//
//                if (probe_status == STATUS_GET_READY_TO_TRANSFER) {
//                    mothership_state = MOTHERSHIP_RECIEVE_DATA;
//                }
//
//                TMR0IF = 0;
//                TMR0_Reload();
//            }
//            break;
//        case MOTHERSHIP_COLLECT_DATA:
//
//            // Read data
//            mothership_read_data();
//
//            mothership_state = MOTHERSHIP_RECIEVE_DATA;
//
//            break;
//        default:
//            mothership_state = MOTHERSHIP_CHECK_PROBE;
//            break;
//    }
}



