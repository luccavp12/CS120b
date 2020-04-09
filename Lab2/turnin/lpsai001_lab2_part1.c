/*	Author: lpsai001
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */

    DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
    DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs
                // Initialize output on PORTB to 0x00
    
    //unsigned char tmpA = 0x00;

    /* Insert your solution below */
    while (1) {
	
	if ((PINA & 0x01) && !(PINA & 0x02)) {
	    PORTB = 1;
	}
	else {
	    PORTB = 0;
	}	

    }
    return 1;
}
