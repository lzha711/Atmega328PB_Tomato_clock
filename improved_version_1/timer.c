/*
 * timer.c
 *
 * Created: 15/09/2021 8:58:29 pm
 *  Author: yuanf
 */ 
#include "common.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "timer.h"

// Timer0 for switch deboucing 
#define NUM_OF_CONSECUTIVE_PRESSES 800 // after some testing
#define NUM_OF_CONSECUTIVE_NON_PRESSES 4

volatile int intConsecutivePresses = 0; //initialize them to 0
volatile int intConsecutiveNonPresses = 0;
extern bool need_reset;
extern uint8_t minutes;
extern uint8_t seconds; 

void TIMER0_init(void){
	TCCR0A = 0b00000010; //CTC mode, see TCCR0B too
	TCCR0B = 0b00000010; //chip clock / 8
	SET_BIT(TIMSK0, OCIE0A); //enable interrupt
	OCR0A = 0xFE;  // set compare register 
}

ISR(TIMER0_COMPA_vect){
	if (BIT_IS_CLEAR(PINC, PC1))
	{
		intConsecutivePresses++; // increment counter for number of presses
		if(intConsecutivePresses >= NUM_OF_CONSECUTIVE_PRESSES) {			// if enough presses to constitute a press		
			if (minutes >= 30) //reset the time to the alternative
			{
				minutes = 25; 
				seconds = 0; 
			}else{
				minutes = 45; 
				seconds = 0;
			}										
			intConsecutivePresses = 0;										// and reset press counts
			intConsecutiveNonPresses = 0;
		}	
	}else if(BIT_IS_SET(PINC, PC1)) {						// else if button is not pressed (logic low)
		intConsecutiveNonPresses++;
		if(intConsecutiveNonPresses >= NUM_OF_CONSECUTIVE_NON_PRESSES) {
			intConsecutivePresses = 0;										// reset press counts
			intConsecutiveNonPresses = 0;
		}
	}
}

// Timer1 for 1s counting
void TIMER1_init(void){
	TCCR1A = 0b00000000; //normal mode
	TCCR1B = 0b00001101; //CTC mode, clk_t0 = clk_io/1024
	OCR1A = 0x3D08; // trigger interrupt every 1s
	// OCR1A = 0x0024; //for quick testing
	SET_BIT(TIMSK1, OCIE1A); // timer output compare match A interrupt enable
	SET_BIT(PORTB, PB1); //set OC1A as output, doesn't matter input or output
}


// execute every 1s, down counting function is here
ISR(TIMER1_COMPA_vect){	//store value here only
	if (!need_reset) // if no need reset, keep down-counting
	{
		if(seconds < 1){ //when second reaches 0
			seconds = 60;
			minutes -= 1;
		}
		if (minutes < 1)
		{
			minutes = 45;//reset
			seconds = 0;
			need_reset = true; // need reset when time is up
		}
		seconds--;
		}else{
		// no counting at all
	}
}
