/*	Author: Lucca Psaila
 *  Partner(s) Name: 
 *	Lab Section: 028
 *	Assignment: Lab #6  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Video Link: https://www.youtube.com/watch?v=Y2wze65oPLk
 * 
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


enum States {Start, Light0, Light1L, Light1R, Light2, Pressed, Freeze} state;

unsigned char tempA = 0x00;
//unsigned char tempB = 0x00;
unsigned char tempC = 0x00;
unsigned char tempD = 0x00;
unsigned char resume = 0x00;
// unsigned char curr = 0x00;

void Tick() {
    switch(state) {//Transitions
	case Start: 
        state = Light0;
	    break;
	case Light0:
        if (!tempA || resume) {
            state = Light1L;
        }
        else {
            state = Pressed;
            // tempC = 0x02;
            tempC = 0x01;
        }
	    break;
	// case Light1:
    //     if (tempA && (tempD > 0x02)) {
    //         state = Pressed;
    //         tempC = 0x01;
    //     }
    //     else if (tempA && (tempD < 0x02)) {
    //         state = Pressed;
    //         tempC = 0x04;
    //     }
    //     else if (tempA && (tempD == 0x02)) {
    //         state = Pressed;
    //         tempC = 0x02;
    //     }
    //     else if ((!tempA || resume) && (tempD > 0x02)) {
    //         state = Light0;
    //         tempC = 0x02;
    //     }
    //     else if ((!tempA || resume) && (tempD < 0x02)) {
    //         state = Light2;
    //         tempC = 0x02;
    //     }
	//     break;
    case Light1L:
        if (!tempA || resume) {
            state = Light2;
        }
        else {
            state = Pressed;
            tempC = 0x02;
            tempD = 0x00;
        }
        break;
    case Light1R:
        if (!tempA || resume) {
            state = Light0;
        }
        else {
            state = Pressed;
            tempC = 0x02;
            tempD = 0x01;
        }
        break;
	case Light2:
        if (!tempA || resume) {
            state = Light1R;
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
            state = Light0;
            resume = 0x01;
        }
        else if (tempA && (tempC == 0x02)) {
            if (tempD == 0x00) {
                state = Light1L;
            }
            else {
                state = Light1R;
            }
            resume = 0x01;
        }
        else if (tempA && (tempC == 0x04)) {
            state = Light2;
            resume = 0x01;
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
	// case Light1:
    //     tempD = tempC;
    //     tempC = 0x02;
	//     break;
    case Light1L:
        tempC = 0x02;
        break;
    case Light1R:
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
        if (!tempA) {
            resume = 0x00;
        }

        while (!TimerFlag);
        TimerFlag = 0;        
    }
    return 1;
}
