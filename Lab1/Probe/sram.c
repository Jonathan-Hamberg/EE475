#include "sram.h"
#include "mcc_generated_files/mcc.h"

uint16_t previousShiftValue = 0;

/**
 * 
 * @param address
 */
void shiftAddress(uint16_t address) {

    // Shift in lower 8 bits.
    LATC = (uint8_t)(address & 0xFF);

    // Shift in upper 3 bits.
    LATB = (uint8_t)(((address >> 3) & 0xE0) | (LATB & ~0xE0));
}

void sramWrite(uint16_t address, uint8_t data) {
    // Set the lower 8 bits of the address.
    shiftAddress(address);

    // Enable the SRAM module.
    CE_SetLow();

    // Enable PORTA as output.
    TRISA = 0x00;

    // Write data to PORTA.
    LATA = data;

    // Toggle WE.
    WE_SetLow();
    WE_SetHigh();

    // Enable PORTA as input (high-z).
    TRISA = 0xFF;

    // Disable chip enable.
    CE_SetHigh();
}

uint8_t sramRead(uint16_t address) {

    uint8_t data;

    // Set the address of the SRAM.
    shiftAddress(address);

    // Enable the SRAM module.
    CE_SetLow();

    // Toggle WE.
    OE_SetLow();

    // Write data to PORTA.
    data = PORTA;

    OE_SetHigh();

    // Disable chip enable.
    CE_SetHigh();

    return data;
}