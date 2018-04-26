#ifndef MOTHERSHIP_H
#define	MOTHERSHIP_H

typedef enum {
    MOTHERSHIP_READ = 0,
    MOTHERSHIP_WRITE
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

