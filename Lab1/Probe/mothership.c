#include "mothership.h"
#include "communication.h"
#include "mcc_generated_files/drivers/i2c_simple_master.h"

#define BUFFER &buffer
#define ADDRESS 0x00


char buffer[100]; 

/**
 * 
 */
void mothership_setup() {
    
    // Enable Global Interrupts
    INTERRUPT_GlobalInterruptEnable();
    
    // Enable Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();
    
    // Send launch command 
    i2c_write1ByteRegister(ADDRESS, COMMAND_PROBE_LAUNCH, BUFFER);
    
    i2c_read1ByteRegister(ADDRESS, BUFFER);
    
    1c2_
    
    
}

void mothership_loop() {
    
}