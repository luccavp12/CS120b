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

enum States {Start, Locked, HashPressed, HashReleased, Unlocked} state;

unsigned char tempA = 0x00;
unsigned char tempB = 0x00;
unsigned char tempC = 0x00;

void Tick() {
    switch(state) {//Transitions
	case Start: 
	    state = Locked;
	    break;
	case Locked:
	    if (tempA == 0x04) {
		state = HashPressed;
	    }
	    else {
		state = Locked;
	    }
	    break;
	case HashPressed:
	    if (tempA == 0x04) {
		state = HashPressed;
	    }
	    else if (tempA == 0x00) {
		state = HashReleased;
	    }
	    else {
		state = Locked;
	    }
	    break;
	case HashReleased:
	    if (tempA == 0x00) {
		state = HashReleased;
	    }
	    else if (tempA == 0x02) {
		state = Unlocked;
	    }
	    else {
		state = Locked;
	    }
	    break;
	case Unlocked:
	    if (tempA >> 7) {
		state = Locked;
	    }
	    else {
		state = Unlocked;
	    }
	    break;
	default:
	    state = Start;
	    break;
    };

    switch(state) { // State Actions
	case Start:
	    tempC = 0x00; 
	    break;
	case Locked:
	    tempB = 0x00;
	    tempC = 0x01;
	    break;
	case HashPressed:
	    tempC = 0x02;
	    break;
	case HashReleased:
	    tempC = 0x03;
	    break;
	case Unlocked:
	    tempB = 0x01;
	    tempC = 0x04;
	    break;
	default:
	    break;
    };
}


int main(void) {

    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;    

    state = Start;
    while (1) {
	tempA = PINA;
	Tick();
	PORTB = tempB;
    }
    return 1;
}
