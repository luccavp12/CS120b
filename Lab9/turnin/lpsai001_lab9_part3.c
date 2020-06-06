/*	Author: Lucca
 *  Partner(s) Name: 
 *	Lab Section: 028
 *	Assignment: Lab #9  Exercise #3
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

void set_PWM(double frequency) {
    static double current_frequency;

    if (frequency != current_frequency) {
        if (!frequency) {
            TCCR3B &= 0x08;
        }
        else {
            TCCR3B |= 0x03;
        }
        if (frequency < 0.954){
            OCR3A = 0xFFFF;
        }
        else if (frequency > 31250) {
            OCR3A = 0x0000;
        }
        else {
            OCR3A = (short)(8000000 / (128 * frequency)) - 1;
        }

        TCNT3 = 0;
        current_frequency = frequency;
    }
}

void PWM_on() {
    TCCR3A = (1 << COM3A0);
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
    set_PWM(0);
}

void PWM_off() {
    TCCR3A = 0x00;
    TCCR3B = 0x00;
}

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

enum States {Start, Wait, Play, Pressed} state;

unsigned char tempA = 0x00;
// unsigned char status = 0x00;
unsigned char i = 0;
unsigned char a = 0x00;
// unsigned char curr = 0x00;
double arr1 [] = {5, 5, 15, 2, 2, 15, 2, 2, 15}; //Time
double arr2 [] = {440, 493.8, 369.99, 293.66, 329.63, 277.18, 369.99, 440, 329.63}; //Notes
// double arr [] = {261.63, 293.66, 329.63, 349.23, 392, 440, 493, 523.25}; //Notes

void Tick() {
    switch(state) {//Transitions
	case Start: 
        state = Wait;
        // i = 0x00;
	    break;
	case Wait:
        if (tempA) {
            state = Play;
            i = 0;
        }
        else {
            state = Wait;
        }
	    break;
    case Play:
        if (i < 9) {
            //Song is still playing
            if (a < arr1[i]) {
                //Currently playing a note
                state = Play;
            }
            else {
                a = 0x00;
                i++;
                state = Play;
            }
        }
        else {
            //Finished song
            state = Pressed;
        }
        break;
    case Pressed:
        if (tempA) {
            state = Pressed;
        }
        else {
            state = Wait;
        }
        break;
	default:
        state = Start;
	    break;
    };

    switch(state) { // State Actions
	case Start: 
	    break;
	case Wait:
        // set_PWM(261);
	    break;
    case Play:
        set_PWM(arr2[i]);
        // a++;
        break;
    case Pressed:
        break;
	default:
	    break;
    };
}


int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x40;
    // DDRD = 0xFF; PORTD = 0x00;

    PWM_on();    
    // set_PWM(261);

    TimerSet(100);
    TimerOn();

    state = Start;
    while (1) {
        tempA = ~PINA;
        Tick();

        a++;

        while (!TimerFlag);
        TimerFlag = 0;
    }
    return 1;
}
