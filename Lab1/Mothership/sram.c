#include "sram.h"

uint16_t previousShiftValue = 0;

/**
 * 
 * @param address
 */
void shiftAddress(uint8_t address);

void sramWrite(uint16_t address, uint8_t data);

uint8_t sramRead(uint16_t address);