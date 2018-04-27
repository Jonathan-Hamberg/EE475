#include <xc.h>        /* XC8 General Include File */
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "mothership.h"
#include "communication.h"

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/drivers/i2c_simple_master.h"

#define ADDRESS 0x50
#define SECTION_SIZE 16
#define SECTIONS 64

MothershipState mothership_state = MOTHERSHIP_CHECK_PROBE;

// Mothership data buffers
volatile uint8_t probe_data_section[SECTION_SIZE];

// Tracking probe state
volatile uint8_t probe_status;


void mothership_read_data() { 
    for(int section = 0; section < SECTIONS; section++) {
        probe_status = i2c_read1ByteRegister(ADDRESS, COMMAND_STATUS_GET);
        if (probe_status == STATUS_GET_READY_TO_TRANSFER_SECTION) {
            i2c_readDataBlock(ADDRESS, COMMAND_PROBE_DATA, probe_data_section, SECTION_SIZE);
        }
        __delay_us(500);
    }
}

/**
 * 
 */
void mothership_setup() {
    // Enable Global and Peripheral Interrupts
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    
    // Start timer to poll launch signal
    TMR0_StartTimer();
    
    // Enable interrupts 
    mssp2_enableIRQ();
    
    // Launch probe
    i2c_write1ByteRegister(ADDRESS, COMMAND_PROBE_LAUNCH, COMMAND_PROBE_LAUNCH);
    
    // Get probe status
    probe_status = i2c_read1ByteRegister(ADDRESS, COMMAND_STATUS_GET);
    
    while (probe_status !=  STATUS_GET_PROBE_LANDED) {
        if (TMR0IF) {
            i2c_write1ByteRegister(ADDRESS, COMMAND_PROBE_LAUNCH, COMMAND_PROBE_LAUNCH);
            
            probe_status = i2c_read1ByteRegister(ADDRESS, COMMAND_STATUS_GET);
            
            TMR0IF = 0;
            TMR0_Reload(); 
        }
    }
    
    TMR0_StopTimer();
}

void mothership_loop() {
    
    // Start timer to poll probe status
    TMR0_StartTimer();
    
    // Switches between the different mothership operations
    switch(mothership_state) {
        case MOTHERSHIP_CHECK_PROBE:
            if (TMR0IF) {
                probe_status = i2c_read1ByteRegister(ADDRESS, COMMAND_STATUS_GET);
                
                if (probe_status == STATUS_GET_READY_TO_TRANSFER) {
                    mothership_state = MOTHERSHIP_RECIEVE_DATA;
                }
               
                TMR0IF = 0;
                TMR0_Reload(); 
            }
            break;
        case MOTHERSHIP_RECIEVE_DATA:
            
            // Read data
            mothership_read_data();
            
            mothership_state = MOTHERSHIP_RECIEVE_DATA;
            
            break;
        default:
            mothership_state = MOTHERSHIP_CHECK_PROBE;
            break;
    }
}



