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
    DDRB = 0xFF; PORTB = 0x00;    

    unsigned char tempA = 0x00;
    unsigned char tempB = 0x00;
    unsigned char pressed = 0x00;
    unsigned char count = 0x07;
    unsigned char changed = 0x00;

    while (1) {
	tempB = tempA;
	tempA = PINA;

	if (tempB != tempA) {
	    changed = 0x01;
	}
	else {
	    changed = 0x00;
	}

	if (tempA) {
	    pressed += 0x01;
	}
	else {
	    pressed = 0x00;
	}

	if ((pressed == 0x01) && (changed)) {
            if ((count == 0x09) && (tempA == 0x01)){
	        PORTC = count;
	    }
	    else if ((count == 0x00) && (tempA == 0x02)) {
		PORTC = count;
	    }
	    else {	    
		if (tempA == 0x01) {
		    count++;
		}
		else if (tempA == 0x02) {
		    count--;
		}
		else {
		    count = 0x00;
		}
	    }	    
	}
	else if (tempA == 0x03) {
	   count = 0x00;
	}
	PORTC = count;	
    }
    return 1;
}
