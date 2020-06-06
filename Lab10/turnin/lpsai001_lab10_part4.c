/*	Author: Lucca Psaila
 *  Partner(s) Name: 
 *	Lab Section: 028
 *	Assignment: Lab #10  Exercise #4
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

typedef struct task {
    int state;
    unsigned long period;
    unsigned long elapsedTime;
    int(*TickFct)(int);
}task;

// const unsigned long tasksPeriod = 100;
const unsigned long tasksPeriod = 1;
const unsigned short tasksNum = 4;
task tasks[4];

unsigned char threeLEDs = 0;
unsigned char blinkingLED = 0;
unsigned char speakertoggle = 0;
unsigned char speakerfreq = 1;
unsigned char tempA = 0x00;
unsigned char tempB = 0x00;
unsigned char i = 0x00;
unsigned char x = 0x00;

enum TL_States {TL_SMStart, TL_S1, TL_S2, TL_S3};
int TickFct_ThreeLeds(int state){
    //switch statements
    switch(state) {
    case TL_SMStart:
        state = TL_S1;
        break;
    case TL_S1:
        threeLEDs = 0x01;
        state = TL_S2;
        break;
    case TL_S2:
        threeLEDs = 0x02;
        state = TL_S3;
        break;
    case TL_S3:
        threeLEDs = 0x04;
        state = TL_S1;
        break;
    }
    return state;
}

enum BL_States {BL_SMStart, BL_S1};
int TickFct_BlinkLed(int state){
    //switch statements
    switch(state) {
    case BL_SMStart:
        state = BL_S1;
        break;
    case BL_S1:
        if (!blinkingLED) {
            blinkingLED = 1;
        }
        else {
            blinkingLED = 0;
        }
        state = BL_S1;
        break;
    }
    return state;
}

enum SP_States {SP_SMStart, SP_1, SP_2};
int TickFct_SpeakerBeep(int state){
    //switch statements
    switch(state) {//Transitions
    case SP_SMStart:
        state = SP_1;
        break;
    case SP_1:
        if (tempA == 0x04) {
            if (!speakertoggle) {
                speakertoggle = 1;
            }
            else {
                speakertoggle = 0;
            }
        }
        else if (tempA == 0x01) {
            if (speakerfreq < 1000) {
                speakerfreq++;
            }
            tasks[2].period = speakerfreq;
            state = SP_2;
        }
        else if (tempA == 0x02) {
            if (speakerfreq > 1) {
                speakerfreq--;
            }
            tasks[2].period = speakerfreq;
            state = SP_2;
        }
        break;
    case SP_2:
        if (tempA) {
            state = SP_2;
        }
        else {
            state = SP_1;
        }
    };
    return state;
}

enum Comb_States {Comb_SMStart, Comb_S1};
int TickFct_CombLeds(int state){
    //switch statements
    switch(state) {
    case Comb_SMStart:
        state = Comb_S1;
        break;
    case Comb_S1:
        tempB = (0x10 * speakertoggle) + (0x08 * blinkingLED) + threeLEDs;
        break;
    }
    return state;
}

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
    unsigned char i;
    for (i = 0; i < tasksNum; i++) {
        if (tasks[i].elapsedTime >= tasks[i].period) {
            tasks[i].state = tasks[i].TickFct(tasks[i].state);
            tasks[i].elapsedTime = 0;
        }
        tasks[i].elapsedTime += tasksPeriod;
    }
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

///////////////////////////////////////////////////////////////////////////////////////////

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;


    unsigned char i = 0;
    tasks[i].state = BL_SMStart;
    tasks[i].period = 1000; //every 1 second
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &TickFct_BlinkLed;
    i++;
    tasks[i].state = TL_SMStart;
    tasks[i].period = 300; //every 300 ms
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &TickFct_ThreeLeds;
    i++;
    tasks[i].state = SP_SMStart;
    tasks[i].period = 2; //
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &TickFct_SpeakerBeep;
    i++;
    tasks[i].state = Comb_SMStart;
    // tasks[i].period = 100;
    tasks[i].period = 1;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &TickFct_CombLeds;
    
    TimerSet(tasksPeriod);
    TimerOn();

    while (1) {
        tempA = ~PINA;
        PORTB = tempB;
    }
    return 1;
}
