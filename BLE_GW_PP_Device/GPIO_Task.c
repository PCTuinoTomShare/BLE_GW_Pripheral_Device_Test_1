/*
 * GPIO_Task.c
 *
 * Created: 2016/7/29 下午 12:50:41
 *  Author: tom hsieh
 */ 

#include <avr/io.h>

// GPIO initialize.
void GPIO_Init( void )
{
	// PORT B bit #1, outport.
	DDRB |= 0x02;
	// PB1 output low.
	PORTB &= 0xfd;
}