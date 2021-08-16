/*
 * 7SegmentDisplay.c
 * 
 * Created: 5/28/2021 10:10:48 AM
 * Author : lzha711
 */ 

// 16MHz external xtal
#define F_CPU 16000000UL
#define true 1
#define false 0
#define POV_delay 5 // set delay time 5ms

// library
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>

//declare variables and functions
void CalculateMinutes (void);
void CalculateSeconds (void);
void WriteDisplays (uint8_t digit, uint8_t displaynum);
void IO_init(void);
void TIMER1_init(void);

uint8_t minutes = 25;
uint8_t seconds = 0;
bool need_reset_flag = false; 

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
		PORTB &= ~(1<<PB2); //select Sec2 display
		PORTB |= (1<<PB3) | (1<<PB4) | (1<<PB5); //disable all other modules
	}else if (displaynum == 1){
		PORTB &= ~(1<<PB3); //select Sec1 display
		PORTB |= (1<<PB2) | (1<<PB4) | (1<<PB5); //disable all other modules
	}else if (displaynum == 2){
		PORTB &= ~(1<<PB4); //select Min2
		PORTB |= (1<<PB0); //dp on when min2 is selected
		PORTB |= (1<<PB2) | (1<<PB3) | (1<<PB5); //disable all other modules
	}else if (displaynum == 3){
		PORTB &= ~(1<<PB5); //select Min1
		PORTB |= (1<<PB2) | (1<<PB3) | (1<<PB4); //disable all other modules
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
		PORTB &= ~(1<<PB2); //select Sec2 display
		PORTB |= (1<<PB3) | (1<<PB4) | (1<<PB5); //disable all other modules
		}else if (displaynum == 1){
		PORTB &= ~(1<<PB3); //select Sec1 display
		PORTB |= (1<<PB2) | (1<<PB4) | (1<<PB5); //disable all other modules
		}else if (displaynum == 2){
		PORTB &= ~(1<<PB4); //select Min2
		PORTB |= (1<<PB2) | (1<<PB3) | (1<<PB5); //disable all other modules
		}else if (displaynum == 3){
		PORTB &= ~(1<<PB5); //select Min1
		PORTB |= (1<<PB2) | (1<<PB3) | (1<<PB4); //disable all other modules
	}
	
	// assign display number
	for (int segCount = 0; segCount <7; ++segCount){
		PD_pin = segCount + 1;
		PORTD |= (seven_seg_word[word][segCount]<<PD_pin); //assign [word][a-g] to PD1-PD7
	}
}

// figure out what to display on sec2 and sec1
void CalculateSeconds(void){
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
void CalculateMinutes(void){
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

void IO_init(void){
	DDRD = 0b11111110; //set PD1-PD7 as output
	DDRB = 0b00111111; //set PB0-PB5 as output
	DDRC &= ~(1<<PC1); //set PC1 as input
	PORTC |= (1<<PC1); //use internal pull-up resistor
}

void TIMER1_init(void){
    TCCR1A = 0b00000000; //normal mode
	TCCR1B = 0b00001101; //CTC mode, clk_t0 = clk_io/1024
	OCR1A = 0x3D08; // trigger interrupt every 1s
	// OCR1A = 0x00F4; //for quick testing
	TIMSK1 = 0b00000010; // set bit OCIE1A, timer/counter1 output compare match A interrupt enable
	PORTB |= (1<<PB1); //set OC1A as output, doesn't matter input or output
}


// execute every 1s, down counting function is here
ISR(TIMER1_COMPA_vect){	//store value here only
	if (!need_reset_flag)
	{
		if(seconds < 1){ //when second reaches 0
			seconds = 60;
			minutes -= 1;
		}
		if (minutes < 1)
		{
			minutes = 25;//reset to 25
			seconds = 0;
			need_reset_flag = true;
		}
		seconds--;
	}else{
		// no counting at all
		minutes = 25;
		seconds = 0; 
	}
}

int main(void)
{
   // uint8_t pin, digit=0;
	IO_init();
	TIMER1_init();
	sei(); //enable interrupt

    while (1) 
    {	
		if (!(PINC & (1<<PC1))) //if PC1 input is low
		{
			need_reset_flag = false;
		}
		if (!need_reset_flag)
		{
			//display time
			CalculateSeconds();
			CalculateMinutes();
		}else{
			//display REST
			Display_REST();
		}
    }
	// return 0; // should never get here
}

