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

    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x01;
    
    unsigned char tempA = 0x00;
    unsigned char pressed = 0x00;
    unsigned char switched = 0x00;

    while (1) {
	
	tempA = PINA;

	if (tempA) {
	    pressed += 0x01;
	}
	else {
	    pressed = 0x00;
	}

	if (pressed == 0x01) {
	    if (!switched) {
		PORTB = 0x02;
		switched = 0x01;
	    }
	    else {
		PORTB = 0x01;
		switched = 0x00;
	    }
	}
	
    }
    return 1;
}
