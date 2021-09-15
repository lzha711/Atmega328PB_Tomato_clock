/*
 * Display.h
 *
 * Created: 13/09/2021 9:53:52 pm
 *  Author: yuanf
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

void WriteDisplays (uint8_t digit, uint8_t displaynum);
void WriteWords(uint8_t word, uint8_t displaynum);
void Display_REST(void);
void DisplayMinutes(void);
void DisplaySeconds(void);

#endif /* DISPLAY_H_ */