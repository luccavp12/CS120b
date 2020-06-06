/*	Author: Lucca Psaila
 *  Partner(s) Name: 
 *	Lab Section: 028
 *	Assignment: Lab #7  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Video Link: https://www.youtube.com/watch?v=0ThppsDSkQA&list=PLNlSY4Y-hAEz9vM1Yeq-XgLod01qRS5Ht&index=8
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

enum States {Start, Light0, Light1L, Light1R, Light2, Pressed, Freeze, WinPress, Win} state;

unsigned char tempA = 0x00;
//unsigned char tempB = 0x00;
unsigned char tempC = 0x00;
unsigned char tempD = 0x00;
unsigned char resume = 0x00;
unsigned char score = 0x07;
// unsigned char curr = 0x00;

void Tick() {
    switch(state) {//Transitions
	case Start: 
        state = Light0;
        score = 0x05;
	    break;
	case Light0:
        if (!tempA || resume) {
            state = Light1L;
        }
        else {
            state = Pressed;
            // tempC = 0x02;
            tempC = 0x01;
            if (score > 0) {
                score--;
            }
        }
	    break;
    case Light1L:
        if (!tempA || resume) {
            state = Light2;
        }
        else {
            if (score++ == 0x08) {
                state = WinPress;
            }
            else {
                state = Pressed;
            }
            tempC = 0x02;
            tempD = 0x00;
            // score++;
        }
        break;
    case Light1R:
        if (!tempA || resume) {
            state = Light0;
        }
        else {
            if (score++ == 0x09) {
                state = WinPress;
            }
            else {
                state = Pressed;
            }
            tempC = 0x02;
            tempD = 0x01;
            // score++;
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
            if (score > 0) {
                score--;
            }
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
    case WinPress:
        if (tempA) {
            state = WinPress;
        }
        else {
            state = Win;
        }
        break;   
    case Win:
        if (!tempA) {
            state = Win;
        }
        else {
            state = Start;
            // score = 0x05;
        }
        break;   
	default:
        state = Start;
	    break;
    };

    switch(state) { // State Actions
	case Start: 
        score = 0x05;
	    break;
	case Light0:
        tempC = 0x01;
	    break;
    case Light1L:
        tempC = 0x02;
        // score++;
        break;
    case Light1R:
        tempC = 0x02;
        // score++;
        break;
	case Light2:
        tempC = 0x04;
		break;
	case Pressed:
        // if (score > 0) {
        //     score--;
        // }
		break;  
	case Freeze:
        // TimerOff();
		break;      
    case WinPress:
        break;   
    case Win:
        // LCD_DisplayString(1, "Winner!!!");
        break;   
	default:
	    break;
    };
}


int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    LCD_init();

    TimerSet(300);
    TimerOn();
    
    state = Start;
    while (1) {
		tempA = ~PINA;

		Tick();
        
        PORTB = tempC;
        
        if (score == 0x09) {
            LCD_DisplayString(1, "Winner!!!");
        }
        else {
            LCD_WriteData(score + '0');
        }
        
        if (!tempA) {
            resume = 0x00;
        }
        
        while (!TimerFlag);
        TimerFlag = 0;
        LCD_ClearScreen();
        continue;
    }
    return 1;
}
