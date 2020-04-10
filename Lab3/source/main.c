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
    DDRB = 0x00; PORTB = 0xFF;
    DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize output on PORTC to 0x00   
    
    unsigned char i = 0x00;
    unsigned char count = 0x00;
    unsigned char pos = 0x00;
    unsigned char tempA = 0x00; 
    unsigned char tempB = 0x00;

    /* Insert your solution below */
    while (1) {
	tempA = PINA;
	tempB = PINB;

	pos = 0x01;

	for (i = 0; i < 8; i++) {
	    if ((tempA & pos)) {
		count++;
	    }
	    if ((tempB & pos)) {
		count++;
	    }		 
	    pos *= 2;
	}
	
	PORTC = count;	
	count = 0x00;
    }
    return 1;
}
