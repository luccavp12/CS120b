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
    //unsigned char cnttaken = 0x04
   
    /* Insert your solution below */
    while (1) {
	
	cntavaila = (PINA & 0x01) + (PINA & 0x02) + (PINA & 0x04) + (PINA & 0x08);

	cntavaila = 4 - cntavaila;

	PORTC = cntavaila;

    }
    return 1;
}
