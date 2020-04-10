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
    //DDRB = 0x00; PORTB = 0xFF;
    DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize output on PORTC to 0x00   
    
    //unsigned char i = 0x00;
    //unsigned char count = 0x00;
    //unsigned char pos = 0x00;
    unsigned char tempA = 0x00; 
    //unsigned char tempB = 0x00;

    /* Insert your solution below */
    while (1) {
	tempA = PINA;
	
	if (!tempA) {
	    PORTC = 0x40;
	}
	else if ((tempA == 0x01) || (tempA == 0x02)) {
	    PORTC = 0x60;
	}
	else if ((tempA == 0x03) || (tempA == 0x04)) {
	    PORTC = 0x70;
	}
	else if ((tempA == 0x05) || (tempA == 0x06)) {
	    PORTC = 0x38;
	}
	else if ((tempA == 0x07) || (tempA == 0x09)) {
	    PORTC = 0x3C;
	}
	else if ((tempA == 0x0A) || (tempA == 0x0C)) {
	    PORTC = 0x3E;
	}
	else if ((tempA == 0x0D) || (tempA == 0x0F)) {
	    PORTC = 0x3F;
	}
    }
    return 1;
}
