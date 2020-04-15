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
    DDRA = 0xFF; PORTA = 0x00; // Configure port A's 8 pins as inputs
    DDRB = 0xFE; PORTB = 0x00;
    DDRD = 0x00; PORTC = 0xFF; // All of D is inputs, while only B0 is input   
    
    //unsigned char i = 0x00;
    //unsigned char count = 0x00;
    //unsigned char pos = 0x00;
    //unsigned char tempA = 0x00; 
    //unsigned char tempB = 0x00;
    //unsigned char tempC = 0x00;

    unsigned short weight = 0;

    /* Insert your solution below */
    while (1) {
	if (!(PINB)) {
	    weight = PIND + PINB;
	}
	else {
	    weight = PIND * 2;
	}
	PORTA = weight;
	if (weight >= 70) {
	    PORTB = 0x02;
	}
	else if (weight > 5 && weight < 70) {
	    PORTB = 0x04;
	}
	else if (weight <= 5) {
	    PORTB = 0x00;
	}

	weight = 0;
    }
   return 1;
}
