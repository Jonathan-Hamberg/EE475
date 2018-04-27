#include <xc.h>        /* XC8 General Include File */
#include "mothership.h"
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include "mcc_generated_files/mcc.h"



/**
 * 
 */
void main(void) {
    
    // Initiallze the mothership system
    mothership_setup();
    
    while(1) {
        mothership_loop();
    }
     
}

