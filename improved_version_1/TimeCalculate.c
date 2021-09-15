/*
 * TimeCalculate.c
 *
 * Created: 13/09/2021 9:45:21 pm
 *  Author: yuanf
 */ 

#include "common.h"
#include "TimeCalculate.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>

extern uint8_t minutes;
extern uint8_t seconds;


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
