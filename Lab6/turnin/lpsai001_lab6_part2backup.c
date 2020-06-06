/*	Author: lucca
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
    TCCR1B = 0x0B;
    OCR1A = 125;
    TIMSK1 = 0x02;
    TCNT1 = 0;

    _avr_timer_cntcurr = _avr_timer_M;

    SREG |= 0x80;
}

void TimerOff() {
    TCCR1B = 0x00;
}

void TimerISR() {
    TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
    _avr_timer_cntcurr--;
    if (_avr_timer_cntcurr == 0) {
        TimerISR();
        _avr_timer_cntcurr = _avr_timer_M;
    }
}

void TimerSet(unsigned long M) {
    _avr_timer_M = M;
    _avr_timer_cntcurr = _avr_timer_M;
}


enum States {Start, Light0, Light1, Light2, Pressed, Freeze} state;

unsigned char tempA = 0x00;
//unsigned char tempB = 0x00;
unsigned char tempC = 0x00;
unsigned char tempD = 0x00;
// unsigned char curr = 0x00;

void Tick() {
    switch(state) {//Transitions
	case Start: 
        state = Light0;
	    break;
	case Light0:
        if (!tempA) {
            state = Light1;
        }
        else {
            state = Pressed;
            // tempC = 0x02;
            tempC = 0x01;
        }
	    break;
	case Light1:
        if (tempA && (tempD > 0x02)) {
            state = Pressed;
            tempC = 0x01;
        }
        else if (tempA && (tempD < 0x02)) {
            state = Pressed;
            tempC = 0x04;
        }
        else if (tempA && (tempD == 0x02)) {
            state = Pressed;
            tempC = 0x02;
        }
        else if (!tempA && (tempD > 0x02)) {
            state = Light0;
            tempC = 0x02;
        }
        else if (!tempA && (tempD < 0x02)) {
            state = Light2;
            tempC = 0x02;
        }
	    break;
	case Light2:
        if (!tempA) {
            state = Light1;
        }
        else {
            state = Pressed;
            // tempC = 0x02;
            tempC = 0x04;
        }
		break;
	case Pressed:
        if (tempA) {
            state = Pressed;
        }
        else {
            state = Freeze;
        }
		break;  
	case Freeze:
        if (tempA && (tempC == 0x01)) {
            // TimerOn();
            state = Light0;
        }
        else if (tempA && (tempC == 0x02)) {
            // TimerOn();
            state = Light1;
        }
        else if (tempA && (tempC == 0x04)) {
            // TimerOn();
            state = Light2;
        }
        else {
            state = Freeze;
        }
		break;      
	default:
        state = Start;
	    break;
    };

    switch(state) { // State Actions
	case Start: 
	    break;
	case Light0:
        tempC = 0x01;
	    break;
	case Light1:
        tempD = tempC;
        tempC = 0x02;
	    break;
	case Light2:
        tempC = 0x04;
		break;
	case Pressed:
		break;  
	case Freeze:
        // TimerOff();
		break;      
	default:
	    break;
    };
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    
    TimerSet(300);
    TimerOn();
    
    // unsigned char tempC = 0x01;
    state = Start;
    while (1) {
        tempA = ~PINA;
        Tick();
        PORTB = tempC;

        while (!TimerFlag);
        TimerFlag = 0;        
    }
    return 1;
}
