#include "sram.h"
#include "mcc_generated_files/mcc.h"

uint16_t previousShiftValue = 0;

/**
 * 
 * @param address
 */
void shiftAddress(uint8_t address) {

    for (uint8_t i = 0; i < 8; i++) {

        // Set the data bit to the shift register.
        if (address & 0x01) {
            SHIFT_SER_SetHigh();
        } else {
            SHIFT_SER_SetLow();
        }

        // Toggle the SRAM RCLK.
        SHIFT_SRCLK_SRAM_SetHigh();
        SHIFT_SRCLK_SRAM_SetLow();

        address >>= 1;
    }
    // Update the data.
    SHIFT_RCLK_SetHigh();
    SHIFT_RCLK_SetLow();
}

void sramWrite(uint16_t address, uint8_t data) {
    // Set the lower 8 bits of the address.
    shiftAddress(address & 0xFF);

    // Set the upper 3 bits of the address.
    LATC = (LATC & ~0x70) | ((address >> 4) & 0x70);
    

    // Enable the SRAM module.
    SRAM_CE_SetLow();

    // Enable PORTA as output.
    TRISA = 0x00;
    
    // Write data to PORTA.
    LATA = data;
    
    // Toggle WE.
    SRAM_WE_SetLow();
    SRAM_WE_SetHigh();

    // Enable PORTA as input (high-z).
    TRISA = 0xFF;

    // Disable chip enable.
    SRAM_CE_SetHigh();
}

uint8_t sramRead(uint16_t address) {

    uint8_t data;

    // Set the lower 8 bits of the address.
    shiftAddress(address & 0xFF);

    //    // Set the upper 3 bits of the address.
    //    // RC4, RC5, RC6.
    LATC = (LATC & ~0x70) | ((address >> 4) & 0x70);

    // Enable the SRAM module.
    SRAM_CE_SetLow();

    // Toggle WE.
    SRAM_OE_SetLow();
    
    NOP();
    NOP();
    NOP();
    
    // Write data to PORTA.
    data = PORTA;
    
    SRAM_OE_SetHigh();

    // Disable chip enable.
    SRAM_CE_SetHigh();
    
    return data;
}