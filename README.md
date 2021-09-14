# AtmelProgramming_Tomato_clock
A timer application for the Pomodoro technique (tomato clock). Let's beat procrastination!

Basic functions: 
Original idea: Starts from 25:00 (25mins), down counting to 0, trigger the buzzer/alarm and reset to 25:00. 
              
              BOM:
              
              1. ATmega329P
              
              2. 4* 7 segment display to show seconds and minutes. 
              
              3. 16Mhz xtal oscillator (or use xplained mini)
              
              4. 4* AA battery as the power source, use 4148 diode as protection + provide some voltage drop so that 5V is fed in the microcontroller
              
              5. push button * 2
              
              6. buzzer
              
PCB made in SMT. Current functions: 
    Start from 25mins, down counting to 0, and display "REST" with buzzer triggered. 
    When manually press the reset button, it starts over again from 25mins. 
    There is also an RST button to reset the microcontroller in case you get interrupted and want to start over. 


Improve ideas: 
  Currently on PC1 the restart button is duplicated with the reset button. Change the function of push button to achieve the following: 
      1. If current setting is 25mins from where the timer starts to down count, once button pressed, the timer starts from 45mins 
      2. Vice versa 
      
  Steps to achieve this: 
    1. Remove the variable bool need_reset_flag;
    2. .... (to be continued)
