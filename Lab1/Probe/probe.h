#ifndef PROBE_H
#define	PROBE_H

#include <stdint.h>
/**
 * 
 */
typedef enum {
    PROBE_STOWED = 0,
    PROBE_LAUNCHED,
    PROBE_LANDED,
    PROBE_DATA,
} ProbeState;

/**
 * 
 */
void probe_setup();

/**
 * 
 */
void probe_loop();

#endif	/* PROBE_H */

