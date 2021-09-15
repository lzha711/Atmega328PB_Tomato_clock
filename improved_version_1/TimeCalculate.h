/*
 * TimeCalculate.h
 *
 * Created: 13/09/2021 9:45:01 pm
 *  Author: yuanf
 */ 

#include "common.h"

// commons
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>

#ifndef TIMECALCULATE_H_
#define TIMECALCULATE_H_

void TIMER1_init(void);
void CalculateMinutes (void);
void CalculateSeconds (void);

#endif /* TIMECALCULATE_H_ */