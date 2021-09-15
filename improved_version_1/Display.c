/*
 * Display.c
 *
 * Created: 13/09/2021 9:53:34 pm
 *  Author: yuanf
 */ 

#include "common.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>

#include "Display.h"

extern uint8_t minutes;
extern uint8_t seconds;

//a-g 7 segment values for each number
uint8_t seven_seg_digits[10][7] = {
	{ 1,1,1,1,1,1,0 },  // = 0
	{ 0,1,1,0,0,0,0 },  // = 1
	{ 1,1,0,1,1,0,1 },  // = 2
	{ 1,1,1,1,0,0,1 },  // = 3
	{ 0,1,1,0,0,1,1 },  // = 4
	{ 1,0,1,1,0,1,1 },  // = 5
	{ 1,0,1,1,1,1,1 },  // = 6
	{ 1,1,1,0,0,0,0 },  // = 7
	{ 1,1,1,1,1,1,1 },  // = 8
	{ 1,1,1,1,0,1,1 }   // = 9
};

//a-g 7 segment values for each number
uint8_t seven_seg_word[4][7] = {
	{ 1,1,1,0,1,1,1 },  // = R
	{ 1,0,0,1,1,1,1 },  // = E
	{ 1,0,1,1,0,1,1 },  // = S
	{ 1,0,0,0,1,1,0 },  // = T
};

void WriteDisplays(uint8_t digit, uint8_t displaynum){
	uint8_t PD_pin; // initialize to PD2 (PD2 refers to 2)
	PORTD &= 0b00000000; //clear port D
	
	// display select
	if (displaynum == 0){
		PORTC &= ~(1<<PC2); //select Sec2 display
		PORTC |= (1<<PC3) | (1<<PC4) | (1<<PC5); //disable all other modules
		}else if (displaynum == 1){
		PORTC &= ~(1<<PC3); //select Sec1 display
		PORTC |= (1<<PC2) | (1<<PC4) | (1<<PC5); //disable all other modules
		}else if (displaynum == 2){
		PORTC &= ~(1<<PC4); //select Min2
		PORTC |= (1<<PC2) | (1<<PC3) | (1<<PC5); //disable all other modules
		}else if (displaynum == 3){
		PORTC &= ~(1<<PC5); //select Min1
		PORTC |= (1<<PC2) | (1<<PC3) | (1<<PC4); //disable all other modules
	}
	
	// assign display number
	for (int segCount = 0; segCount <7; ++segCount){
		PD_pin = segCount + 1;
		PORTD |= (seven_seg_digits[digit][segCount]<<PD_pin); //assign [digit][a-g] to PD1-PD7
	}
}

void WriteWords(uint8_t word, uint8_t displaynum){
	uint8_t PD_pin; // initialize to PD2 (PD2 refers to 2)
	PORTD &= 0b00000000; //clear port D
	
	// display select
	if (displaynum == 0){
		PORTC &= ~(1<<PC2); //select Sec2 display
		PORTC |= (1<<PC3) | (1<<PC4) | (1<<PC5); //disable all other modules
		}else if (displaynum == 1){
		PORTC &= ~(1<<PC3); //select Sec1 display
		PORTC |= (1<<PC2) | (1<<PC4) | (1<<PC5); //disable all other modules
		}else if (displaynum == 2){
		PORTC &= ~(1<<PC4); //select Min2
		PORTC |= (1<<PC2) | (1<<PC3) | (1<<PC5); //disable all other modules
		}else if (displaynum == 3){
		PORTC &= ~(1<<PC5); //select Min1
		PORTC |= (1<<PC2) | (1<<PC3) | (1<<PC4); //disable all other modules
	}
	
	// assign display number
	for (int segCount = 0; segCount <7; ++segCount){
		PD_pin = segCount + 1;
		PORTD |= (seven_seg_word[word][segCount]<<PD_pin); //assign [word][a-g] to PD1-PD7
	}
}


void Display_REST(void){
	WriteWords(3,0); //T at sec2
	_delay_ms(POV_delay);
	WriteWords(2,1); //S at sec1
	_delay_ms(POV_delay);
	WriteWords(1,2); //E at min2
	_delay_ms(POV_delay);
	WriteWords(0,3); //R at min1
	_delay_ms(POV_delay);
}

// figure out what to display on sec2 and sec1
void DisplaySeconds(void){
	if (seconds/10 == 0 && seconds < 10){
		WriteDisplays(0,1);
		_delay_ms(POV_delay);
		WriteDisplays(seconds,0);
		_delay_ms(POV_delay);
		}else if (seconds%10 != 0){
		WriteDisplays(seconds/10,1);
		_delay_ms(POV_delay);
		WriteDisplays(seconds%10,0);
		_delay_ms(POV_delay);
		}else if (seconds%10 == 0){
		WriteDisplays(seconds/10,1);
		_delay_ms(POV_delay);
		WriteDisplays(0,0);
		_delay_ms(POV_delay);
	}
}

// figure out what to display on min2 and min1
void DisplayMinutes(void){
	if (minutes/10 == 0 && minutes < 10){
		WriteDisplays(0,3);
		_delay_ms(POV_delay);
		WriteDisplays(minutes,2);
		_delay_ms(POV_delay);
		}else if (minutes%10 != 0){
		WriteDisplays(minutes/10,3);
		_delay_ms(POV_delay);
		WriteDisplays(minutes%10,2);
		_delay_ms(POV_delay);
		}else if (minutes%10 == 0){
		WriteDisplays(minutes/10,3);
		_delay_ms(POV_delay);
		WriteDisplays(0,2);
		_delay_ms(POV_delay);
	}
}
