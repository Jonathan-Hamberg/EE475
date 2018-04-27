#ifndef MOTHERSHIP_H
#define	MOTHERSHIP_H

typedef enum {
    MOTHERSHIP_CHECK_PROBE = 0,
    MOTHERSHIP_COLLECT_DATA        
} MothershipState;

/**
 * 
 */
void mothership_setup();

/**
 * 
 */
void mothership_loop();

#endif	/* MOTHERSHIP_H */

