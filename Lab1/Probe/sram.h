#ifndef SRAM_H
#define	SRAM_H

#include <stdint.h>

/**
 * 
 * @param address
 * @param data
 */
void sramWrite(uint16_t address, uint8_t data);

/**
 * 
 * @param address
 * @return 
 */
uint8_t sramRead(uint16_t address);

#endif	/* SRAM_H */

