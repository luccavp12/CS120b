/*	Author: Lucca Psaila
 *  Partner(s) Name: 
 *	Lab Section: 028
 *	Assignment: Lab #8  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init() {
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void) {
    /* Insert DDR and PORT initializations */

    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
    PINA = ~PINA;
    ADC_init();

    while (1) {
        unsigned short x = ADC;
        if (x >= 350/2) {
            PORTB = 0xFF;
        }
        if (x >= 307/2) {
            PORTB = 0x7F;
        }
        if (x >= 264/2) {
            PORTB = 0x3F;
        }
        if (x >= 221/2) {
            PORTB = 0x1F;
        }
        if (x >= 178/2) {
            PORTB = 0x0F;
        }
        if (x >= 135/2) {
            PORTB = 0x07;
        }
        if (x >= 92/2) {
            PORTB = 0x03;
        }
        if (x >= 49/2) {
            PORTB = 0x01;
        }
        if (x >= 0) {
            PORTB = 0x00;
        }
    }
    return 1;
}
