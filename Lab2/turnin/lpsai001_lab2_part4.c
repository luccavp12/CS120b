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

    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0x00; PORTB = 0xFF;   
    DDRC = 0x00; PORTC = 0xFF; // ABC are input
    DDRD = 0xFF; PORTD = 0x00; // D is the output
 
    unsigned char tmpA = 0x00;
    unsigned char tmpB = 0x00;
    unsigned char tmpC = 0x00;
    unsigned char tmpD = 0x00;
   
    /* Insert your solution below */
    while (1) {
	
	tmpA = PINA;
	tmpB = PINB;
	tmpC = PINC;

	if ((tmpA + tmpB + tmpC) > 0x8C) {
	    tmpD = (tmpD | 0x01);
	}
	else {
	    tmpD = (tmpD & 0xFE);
	}

	if (tmpA > tmpC) {
	    if ((tmpA - tmpC) > 0x50) {
		tmpD = (tmpD | 0x02);
	    }
	    else {
		tmpD = (tmpD & 0xFD);
	    }
	}
	else {
	    if ((tmpC - tmpA) > 0x50) {
		tmpD = (tmpD | 0x02);
	    }
	    else {
		tmpD = (tmpD & 0xFD);
	    }
	}

	PORTD = tmpD;

    }
    return 1;
}
