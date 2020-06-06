/*	Author: Lucca
 *  Partner(s) Name: 
 *	Lab Section: 028
 *	Assignment: Lab #9  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
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


enum States {Start, Wait, Pressed, Power, Inc, Dec} state;

unsigned char tempA = 0x00;
unsigned char status = 0x00;
unsigned char i = 0x00;
unsigned char x = 0x00;
double arr [] = {261.63, 293.66, 329.63, 349.23, 392, 440, 493, 523.25};

void Tick() {
    switch(state) {//Transitions
	case Start: 
        state = Wait;
        i = 0x00;
        status = 0x00;
	    break;
	case Wait:
        if (tempA) {
            x = tempA;
            state = Pressed;
        }
        else {
            state = Wait;
        }
	    break;
    case Pressed:
        if (tempA) {
            state = Pressed;
        }
        else {
            if (x == 0x01) {
                state = Power;
            }
            else if (x == 0x02 && status) {
                state = Dec;
            }
            else if (x == 0x04 && status) {
                state = Inc;
            }
            else {
                state = Wait;
            }
        }
        break;
    case Power:
        state = Wait;
        break;
    case Inc:
        state = Wait;
        break;
	case Dec:
        state = Wait;
		break;
	default:
        state = Start;
	    break;
    };

    switch(state) { // State Actions
	case Start: 
	    break;
	case Wait:
        if (status) {
            set_PWM(arr[i]);
        }
        else {
            set_PWM(0);
        }
	    break;
    case Pressed:
        break;
    case Power:
        if (!status) {
            status = 0x01;
        }
        else {
            status = 0x00;
        }
        // i = 0x00;
        break;
    case Inc:
        if (i < 0x07) {
            i++;
        }
        break;
	case Dec:
        if (i > 0x00) {
            i--;
        }
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

    state = Start;
    while (1) {
        tempA = ~PINA;
        Tick();
    }
    return 1;
}
