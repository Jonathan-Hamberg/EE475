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
 * Address[9:8] = PORTC[1:0] output.
 * 
 * Control LED     = PORTB[5] output.
 * Control Switch  = PORTB[4] input.
 * Data Enable     = PORTB[3] output.
 * Data Enable Bar = PORTB[2] output.
 * SRAM OE         = PORTC[3] output.
 * SRAM WE         = PORTC[2] output.
 */

/**
 * 
 */
void StateTransitionWrite() {
    // Disable the OE pin.
    IO_RC3_SetHigh();

    // Enable data_enable PORTB[3], enable ~data_enable PORTB[2]
    LATB = 0b00001000 | (LATB & ~0b00001100);

    // Turn on the LED control indicator.
    IO_RB5_SetHigh();
}

/**
 * 
 */
void StateTransitionRead() {
    // Disable data_enable PORTB[3], disable ~data_enable PORTB[2]
    LATB = 0b00000100 | (LATB & ~0b00001100);

    // Enable the OE pin.
    IO_RC3_SetLow();

    // Turn off the LED control indicator.
    IO_RB5_SetLow();
}

/**
 * 
 */
void StateWrite() {
    // Toggle the WE low to high to write the data.
    IO_RC2_SetLow();
    IO_RC2_SetHigh();
}

/**
 * 
 */
void main(void) {
    // Initialize the system.
    SYSTEM_Initialize();

    // Start the timer to poll the button.
    TMR0_StartTimer();

    // Disable the WE pin.
    IO_RC2_SetHigh();

    // Enter the write state.
    StateTransitionWrite();

    while (1) {
        // Determine if the timer has overflowed.
        if (TMR0IF) {
            // Get the button state.  Polled every 16 milli-seconds.
            previous_button = current_button;
            current_button = IO_RB4_GetValue();

            // Toggle the state machine state.
            if (!current_button && previous_button) {
                if (current_state == READ) {
                    current_state = WRITE;

                    // Transition to the Write state.
                    StateTransitionWrite();
                } else {
                    current_state = READ;


                    // Transition to the Read state.
                    StateTransitionRead();
                }
            }

            // clear the TMR0 interrupt flag
            TMR0IF = 0;

            // Reload the initial value of TMR0
            TMR0_Reload();
        }


        switch (current_state) {
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
        LATC = (uint8_t) (((address >> 8) & 0b00000011) | (LATC & ~0b00000011));
        LATA = (uint8_t) (address & 0xFF);

        // 4ms delay.  Fill 1024 bytes in approximately 4s.
        __delay_ms(4);

    }
}

