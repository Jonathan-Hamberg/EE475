#include <xc.h>        /* XC8 General Include File */

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
#include "mcc_generated_files/mcc.h"

/*
 * Definitions.
 */

typedef enum {
    READ,
    WRITE
} State;

/*
 * Globals.
 */
State current_state = WRITE;

bool current_button = 0;
bool previous_button = 0;

uint16_t address = 0;

/**
 * Address[7:0] = PORTA[7:0] output.
 * Address[9:8] = PORTB[1:0] output.
 * 
 * Control Switch  = PORTC[6] input.
 * Control LED     = PORTC[5] output.
 * Latch           = PORTC[4] output.
 * Data Enable     = PORTC[3] output.
 * Data Enable Bar = PORTc[2] output.
 * SRAM OE         = PORTC[1] output.
 * SRAM WE         = PORTC[0] output.
 */

/**
 * 
 */
void StateTransitionWrite() {
    // Disable the WE pin.
    IO_RC0_SetHigh();
    
    // Enable the OE pin.
    IO_RC1_SetLow();
    
    // Enable data enable.
    IO_RC3_SetHigh();
    
    // Enable data enable bar.
    IO_RC2_SetLow();
}

/**
 * 
 */
void StateTransitionRead() {
    // Disable the OE pin.
    IO_RC1_SetHigh();
    
    // Enable the WE pin.
    IO_RC0_SetLow();
    
    // Disable data enable.
    IO_RC3_SetLow();
    
    // Disable data enable bar.
    IO_RC2_SetHigh();
}

/**
 * 
 */
void StateWrite() {
    // Toggle the WE low to high to write the data.
    IO_RC5_SetLow();
    IO_RC5_SetHigh();
}

/**
 * 
 */
void main(void)
{
    // Initialize the system.
    SYSTEM_Initialize();
    
    /* Initialize I/O and Peripherals for application */
    InitApp();

    while(1) {
        IO_RC5_SetHigh();
        __delay_ms(1000);
        IO_RC5_SetLow();
        __delay_ms(1000);
    }

    while(1) {
        // Determine if the timer has overflowed.
        if(TMR0IF)
        {
            // Get the button state.  Polled every 16 milli-seconds.
            previous_button = current_button;
            current_button = IO_RC6_GetValue();
            
            // Toggle the state machine state.
            if(current_button && !previous_button) {
                if(current_state == READ) {
                    current_state = WRITE;
                    // Turn on the LED control indicator.
                    IO_RC5_SetHigh();
                    
                    // Transition to the Write state.
                    StateTransitionWrite();
                } else {
                    current_state = READ;
                    // Turn off the LED control indicator.
                    IO_RC5_SetLow();
                    
                    // Transition to the Read state.
                    StateTransitionRead();
                }
            }
            
            // clear the TMR0 interrupt flag
            TMR0IF = 0;

            // Reload the initial value of TMR0
            TMR0_Reload();
        }
        
        
        switch(current_state) {
            case READ:
                // Nothing is required to read the data other than setting the
                // address.
                break;
            case WRITE:
                // Set the control pins to write the data to the SRAM.
                StateWrite();
                break;
            default:
                current_state = WRITE;
                break;
        }   
        
        // Increment the address counter.
        address++;
        
        // Assign the address to the SRAM.
        LATA = (uint8_t)(address & 0xFF);
        LATB = (uint8_t)((address >> 8) & 0x03);
        
        // 4ms delay.  Fill 1024 bytes in approximately 4s.
        __delay_ms(4);
                
    }
}

