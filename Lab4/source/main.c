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
    DDRC = 0xFF; PORTC = 0x07;
    
    unsigned char tempA = 0x00;
    unsigned char pressed = 0x00;
    unsigned char count = 0x07;

    while (1) {
	
	tempA = PINA;

	if (tempA) {
	    pressed += 0x01;
	}
	else {
	    pressed = 0x00;
	}

	if (pressed == 0x01) {
            if (count == 0x09){
	        PORTC = count;
	    }
	    else if (count == 0x00) {
		PORTC = count;
	    }
	    else {	    
		if (tempA == 0x01) {
		    count++;
		}
		else if (tempA == 0x02) {
		    count = 0;
		}
		else {
		    count--;
		}
	    }	    
	}

	PORTC = count;	
    }
    return 1;
}
