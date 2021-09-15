/*
 * common.h
 *
 * Created: 13/09/2021 9:48:29 pm
 *  Author: yuanf
 */ 


#ifndef COMMON_H_
#define COMMON_H_

#define F_CPU 16000000UL
#define true 1
#define false 0
#define POV_delay 5

#define SET_BIT(byte, bit) (byte |= (1 << bit))
#define CLEAR_BIT(byte, bit) (byte &= ~(1 << bit))
#define BIT_IS_SET(byte, bit) (byte & (1<<bit)) //if bit is set
#define BIT_IS_CLEAR(byte, bit) (!(byte & (1<<bit))) // if bit is clear

#endif /* COMMON_H_ */