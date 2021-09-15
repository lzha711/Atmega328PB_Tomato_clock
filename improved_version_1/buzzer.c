/*
 * buzzer.c
 *
 * Created: 13/09/2021 10:04:53 pm
 *  Author: yuanf
 */ 
#include "common.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>

#include "buzzer.h"

/* Toggle buzzer every buzz_delay_us, for a duration of buzz_length_ms. */
void buzz_sound(uint16_t buzz_length_ms, uint16_t buzz_delay_us)
{
	uint32_t buzz_length_us;
	uint16_t z;

	buzz_length_us = buzz_length_ms * (uint32_t)1000;
	while (buzz_length_us > buzz_delay_us*2) {
		buzz_length_us -= buzz_delay_us*2;

		/* toggle the buzzer at various speeds */
		PORTC &= ~(1<<PC0);
		//		_delay_us((double) buzz_delay_us);
		z= buzz_delay_us;
		while (--z){};

		PORTC |= (1<<PC0);
		//		_delay_us((double) buzz_delay_us);
		z= buzz_delay_us;
		while (--z){};
	}
}

void play_tone_1(void){ // four notes going up sounds  
		buzz_sound(500,3000);
		_delay_ms(50);
		buzz_sound(550,2500);
		_delay_ms(50);
		buzz_sound(600,2000);
		_delay_ms(50);
		buzz_sound(650,1500);
		_delay_ms(50);
}

// can write more songs below 
