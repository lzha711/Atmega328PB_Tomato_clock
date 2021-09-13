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
#define POV_delay 5 //15ms works fine too

// library
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>

//declare variables and functions
void CalculateMinutes (void);
void CalculateSeconds (void);
void WriteDisplays (uint8_t digit, uint8_t displaynum);
void WriteWords(uint8_t word, uint8_t displaynum);
void Display_REST(void);
void IO_init(void);
void TIMER1_init(void);
static void buzz_sound(uint16_t buzz_length_ms, uint16_t buzz_delay_us);


uint8_t minutes = 25;
uint8_t seconds = 0;
bool need_reset_flag = false;
bool song_flag = false;

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
	DDRB |= (1<<PB1); //set PB1 as timer square wave output
	DDRC = 0b00111101; //set PC2-PC5 as chip select output, PC0 as buzzer output
	PORTC |= (1<<PC1); //use internal pull-up resistor
}

void TIMER1_init(void){
	TCCR1A = 0b00000000; //normal mode
	TCCR1B = 0b00001101; //CTC mode, clk_t0 = clk_io/1024
	OCR1A = 0x3D08; // trigger interrupt every 1s
	// OCR1A = 0x0024; //for quick testing
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

/* Toggle buzzer every buzz_delay_us, for a duration of buzz_length_ms. */
static void buzz_sound(uint16_t buzz_length_ms, uint16_t buzz_delay_us)
{
	uint32_t buzz_length_us;
	uint16_t z;

	buzz_length_us = buzz_length_ms * (uint32_t)1000;
	while (buzz_length_us > buzz_delay_us*2) {
		buzz_length_us -= buzz_delay_us*2;

		/* toggle the buzzer at various speeds */
		PORTC &= ~(1<<PC0);
		PORTC |= (1<<PC0);
		//		_delay_us((double) buzz_delay_us);
		z= buzz_delay_us;
		while (--z){};

		PORTC |= (1<<PC0);
		PORTC &= ~(1<<PC0);
		//		_delay_us((double) buzz_delay_us);
		z= buzz_delay_us;
		while (--z){};
	}
}


int main(void)
{
	// uint8_t pin, digit=0;
	IO_init();
	TIMER1_init();
	sei(); //enable interrupt
	song_flag = true; //initialize song flag true

	while (1)
	{
		if (!(PINC & (1<<PC1))) //if PC1 input is low, i.e, if restart button is pressed
		{
			need_reset_flag = false;
			song_flag = true;
		}
		if (!need_reset_flag)
		{
			//display time
			CalculateSeconds();
			CalculateMinutes();
			
		}else {
			//display REST
			Display_REST();
			if (song_flag) // play the tone
			{
				buzz_sound(500,3000);
				_delay_ms(50);
				buzz_sound(550,2500);
				_delay_ms(50);
				buzz_sound(600,2000);
				_delay_ms(50);
				buzz_sound(650,1500);
				_delay_ms(50);
				song_flag = false; //disable buzzer sound
			}
		}
	}
}

