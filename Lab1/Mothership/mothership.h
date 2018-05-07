#ifndef MOTHERSHIP_H
#define	MOTHERSHIP_H

#include <stdint.h>        /* For uint8_t definition */

typedef enum {
    MOTHERSHIP_CHECK_PROBE = 0,
    MOTHERSHIP_COLLECT_DATA,
    MOTHERSHIP_CHECK_EARTH        
} MothershipState;

/**
 * 
 */
void mothership_setup();

/**
 * 
 */
void mothership_loop();

void RS232_WriteByte(uint8_t byte);

void RS232_WriteString(char * string);

#endif	/* MOTHERSHIP_H */

