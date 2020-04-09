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
    DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize output on PORTC to 0x00   

    unsigned char cntavaila = 0x00;
    unsigned char tempA = 0x00;
    unsigned char first = 0x00;
    unsigned char second = 0x00;
    unsigned char third = 0x00;
    unsigned char fourth = 0x00;

    //unsigned char cnttaken = 0x04
   
    /* Insert your solution below */
    while (1) {
	
	tempA = PINA;

	//cntavaila = (tempA & 0x01) + (tempA & 0x02) + (tempA & 0x04) + (tempA & 0x08);
	
	first = (tempA & 0x01);
	second = (tempA & 0x02);
	third = (tempA & 0x04);
	fourth = (tempA & 0x08);
	
	if (first) {
	    cntavaila++;
	}
	if (second) {
	    cntavaila++;
	}
	if (third) {
	    cntavaila++;
	}
	if (fourth) {
	    cntavaila++;
	}

	cntavaila = 4 - cntavaila;

	if (!cntavaila) {
	    PORTC = 0x80;
	}
	else {
	    PORTC = cntavaila;
	}

	cntavaila = 0x00;

    }
    return 1;
}
