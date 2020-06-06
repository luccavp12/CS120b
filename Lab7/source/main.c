/*	Author: Lucca Psaila
 *  Partner(s) Name: 
 *	Lab Section: 028
 *	Assignment: Lab #7  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * 
 *  Video Link: https://www.youtube.com/watch?v=ALG8qvFDnbE
 * 
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"
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


enum States {Start, Wait, Increment, Decrement, Reset, Pressed, IncPressed, DecPressed} state;

unsigned char tempA = 0x00;
//unsigned char tempB = 0x00;
//unsigned char tempC = 0x00;
unsigned char count = 0x07;
unsigned char elapsed = 0x00;

void Tick() {
    switch(state) {//Transitions
	case Start: 
	    state = Wait;
	    break;
	case Wait:
	    if (tempA == 0x01) {
			state = Increment;
	    }
	    else if (tempA == 0x02){
			state = Decrement;
	    }
		else if (tempA == 0x03) {
			state = Reset;
		}
		else {
			state = Wait;
		}
	    break;
	case Increment:
		// if (tempA == 0x01) {
		// 	state = Increment;
		// }
		// else {
			state = IncPressed;
		// }
	    break;
	case Decrement:
		// if (tempA == 0x02) {
	    // 	state = Decrement;
		// }
		// else {
			state = DecPressed;
		// }
	    break;
	case Reset:
	    // if (tempA == 0x03) {
		// 	state = Reset;
		// }
		// else {
			state = Pressed;
		// }
	    break;
	case Pressed:
		if (tempA == 0x03) {
			state = Reset;
		}
		else if (!tempA) {
			state = Wait;
		}
		else {
			state = Pressed;
		}
		break;
    case IncPressed:
        if (tempA) {
            state = IncPressed;
            elapsed++;
        }
        else {
            state = Wait;
            elapsed = 0x00;
        }
        break;
    case DecPressed:
        if (tempA) {
            state = DecPressed;
            elapsed++;
        }
        else {
            state = Wait;
            elapsed = 0x00;
        }
        break;
	default:
	    state = Start;
	    break;
    };

    switch(state) { // State Actions
	case Start:
		// count = 0x00; 
	    break;
	case Wait:
	    break;
	case Increment:
		if (count < 0x09){
	    	count++;
		}
	    break;
	case Decrement:
	    if (count > 0x00) {
			count--;
		}
	    break;
	case Reset:
	    count = 0x00;
	    break;
	case Pressed:
		break;
    case IncPressed:
        if ((elapsed == 0x0A) && (count < 0x09)) {
            count++;
            elapsed = 0x00;
        }
        break;
    case DecPressed:
        if ((elapsed == 0x0A) && (count > 0x00)) {
            count--;
            elapsed = 0x00;
        }
        break;
	default:
	    break;
    };
}


int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    LCD_init();

    TimerSet(100);
    TimerOn();

    // LCD_DisplayString(1, "Hello World");
    // LCD_WriteData(9 + '0');
    
    state = Start;
    while (1) {
		tempA = ~PINA;
		Tick();
        LCD_WriteData(count + '0');
        while (!TimerFlag);
        TimerFlag = 0;
        LCD_ClearScreen();
        continue;
    }
    return 1;
}
