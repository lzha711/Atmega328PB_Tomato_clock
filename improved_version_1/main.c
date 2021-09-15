/*
 * 7SegmentDisplay.c
 * 
 * Created: 5/28/2021 10:10:48 AM
 * Author : lzha711
 */ 

// commons 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>

#include "common.h"
#include "Display.h"
#include "buzzer.h"
#include "timer.h"

// set the initial start time
volatile uint8_t minutes = 45;
volatile uint8_t seconds = 0;
bool need_reset = false;
bool song_flag = true;

void IO_init(void){
	DDRD = 0b11111110; //set PD1-PD7 as output
	DDRB = 0b00111100; //set PB2-PB3 as output buzzer
	DDRC = 0b00111101; //set PC1-PC5 as output, PC1 as input
	SET_BIT(PORTC, PC1); //use internal pull-up resistor
}


int main(void)
{
    // initialization 
	IO_init();
	TIMER0_init();
	TIMER1_init();
	
	sei(); //enable interrupt

    while (1) 
    {	
		if (!need_reset)
		{
			//display time
			DisplaySeconds();
			DisplayMinutes();
			
		}else{		
			//display REST
			Display_REST();
			if (song_flag) // play the tone once
			{
				play_tone_1(); 
				song_flag = false; //disable buzzer sound
			}
		}
    }
}