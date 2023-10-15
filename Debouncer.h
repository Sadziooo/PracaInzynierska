#ifndef DEBOUNCER_H_
#define DEBOUNCER_H_

#include "main.h"

volatile uint8_t button0_down, button1_down, button2_down;

#define BUTTON0_MASK PIN0_bm
#define BUTTON0_PIN PORTC.IN
#define BUTTON0_PORT PORTC.OUTSET
#define BUTTON0_PULLUP PORTC.PIN0CTRL

#define BUTTON1_MASK PIN1_bm
#define BUTTON1_PIN PORTC.IN
#define BUTTON1_PORT PORTC.OUTSET
#define BUTTON1_PULLUP PORTC.PIN1CTRL

#define BUTTON2_MASK PIN2_bm
#define BUTTON2_PIN PORTC.IN
#define BUTTON2_PORT PORTC.OUTSET
#define BUTTON2_PULLUP PORTC.PIN2CTRL

void debounce0(unsigned int x, unsigned int y, unsigned int z);

void debounce1(unsigned int x, unsigned int y, unsigned int z);

void debounce2(unsigned int x, unsigned int y, unsigned int z);

#endif /* DEBOUNCER_H_ */
