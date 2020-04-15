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
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize output on PORTC to 0x00   
    
    //unsigned char i = 0x00;
    //unsigned char count = 0x00;
    //unsigned char pos = 0x00;
    unsigned char tempA = 0x00; 
    unsigned char tempB = 0x00;
    unsigned char tempC = 0x00;

    /* Insert your solution below */
    while (1) {

	tempA = PINA;

	tempB = tempA >> 4;
	PORTB = tempB;
	//tempC = tempA & 0x0F;
	tempC = tempA << 4;
	PORTC = tempC;

	tempA = 0x00;
	tempB = 0x00;
	tempC = 0x00;
    }
    return 1;
}
