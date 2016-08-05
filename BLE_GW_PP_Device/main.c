/*
 * BLE_GW_PP_Device.c
 *
 * Created: 2016/7/29 上午 10:53:25
 * Author : tom hsieh
 */ 

#include <avr/io.h>
#include <avr/fuse.h>
#include <avr/interrupt.h>
//
// RN4020 presetting :
// - Peripheral,
//
// - Private service :
//		UUID : 3564-8ed7-18b3-4375-b9fdd7f7a059c34c
//
//		--- One read private characteristic :
//				UUID     : 3564-8ed6-18b3-4375-b9fdd7f7a059c34c
//				Handle   : 0x000e
//				Property : 0x02
//	
//		--- One write private characteristic :
//				UUID     : 3564-8ed5-18b3-4375-b9fdd7f7a059c34c
//				Handle   : 0x0010
//				Property : 0x04
//
// - Baud rate 9600 bps.
//
#include "Timer0_Task.h"
#include "UART_Task.h"
#include "ADC_Task.h"
#include "GPIO_Task.h"

FUSES = {
	// 8MHz internal RC oscillator.
	// Slow rising power.
	.low = (FUSE_SUT_CKSEL0 & FUSE_SUT_CKSEL2 & FUSE_SUT_CKSEL3 & FUSE_SUT_CKSEL4 ),
	// Boot from 0x0000.
	// SPI programming enable.
	// debugWIRE enable.
	.high = (FUSE_BOOTSZ0 & FUSE_BOOTSZ1 & FUSE_SPIEN & FUSE_DWEN ),
	// No boot loader lock bit.
	.extended = EFUSE_DEFAULT,
};

int main(void)
{
	// System clock prescaler 1.
	CLKPR = 0x80;  // Enable.
	CLKPR = 0x00;  // Write value.
	
	// GPIO initialize.
	GPIO_Init();
	// Timer #0 initialize.
	Timer0_Init();
	// UART #0 initialize.
	UART0_Init();
	// ADC initialize.
	ADC_Init();
	
	// Enable global interrupt.
	sei();
		
    /* Replace with your application code */
    while (1){
		
		// Timer task.
		Timer0_Task();
		// ADC task.
		ADC_Task();
		// UART received data check.
		UART0_Rec_Check();
		// UART transmit task.
		UART0_Trn_Task();
    }
}

