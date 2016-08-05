/*
 * UART_Task.c
 *
 * Created: 2016/6/26 下午 11:40:16
 *  Author: tom hsieh
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "UART_Task.h"
#include "var.h"

// RN4020 command.
const uint8_t rn4020_cmd[4] = {'s','u','w',','};

// RN4020 command UUID128 option.
const uint8_t rn4020_uuid[33] = {'3','5','6','4','8','e','d','6',
	                             '1','8','b','3','4','3','7','5',
								 'b','9','f','d','d','7','f','7',
								 'a','0','5','9','c','3','4','c',
								 ','};


// UART #0 received ISR.
ISR( USART0_RX_vect )
{
	// Clear received time out.
	uart0_rec_to = 0;
	// Hold received data.		
	uart0_rec_data[uart0_rec_cnt] = UDR0;
	if( uart0_rec_cnt < 16 ){
		++uart0_rec_cnt;
	} 
}

// UART #0 initialize.
void UART0_Init( void )
{
	// 9600 bps @ 8MHz.
	UBRR0H = 0;
	UBRR0L = 51;
	// RX interrupt enable.
	// RX enable.
	// TX enable.
	// 8 bit data.
	// No parity.
	UCSR0B = 0x98;
	// Variable reset.
	rn4020_char_value[0] = 0x30;
	rn4020_char_value[1] = 0x30;
	rn4020_char_value[2] = 0x30;
	rn4020_char_value[3] = 0x30;
	rn4020_char_value[4] = 0x0d;
	rn4020_char_value[5] = 0x0a;
	
	uart0_trn_cnt = 0;
	uart0_trn_task = 0;	
}

// UART #0 transmit task.
void UART0_Trn_Task( void )
{
	// Check data counter.
	if( uart0_trn_cnt == 0 ){
		// Task #3, send command.
		if( uart0_trn_task == 3 ){			
			puart0_trn_data = rn4020_cmd;
			uart0_trn_cnt = 4;
			--uart0_trn_task;
		}
		// Task #2, send UUID128.
		else if( uart0_trn_task == 2 ){
			puart0_trn_data = rn4020_uuid;
			uart0_trn_cnt = 33;
			--uart0_trn_task;			
		}
		// Task #3, send characteristic value.
		else if( uart0_trn_task == 1 ){
			puart0_trn_data = rn4020_char_value;
			uart0_trn_cnt = 6;
			--uart0_trn_task;			
		}
		return;
	}
	// Check transmit buffer.
	temp1 = UCSR0A;
	temp1 &= 0x20;
	if( temp1 == 0 ){
		return;
	}
	// Data out.
	UDR0 = *puart0_trn_data;
	// Next data.
	++puart0_trn_data;
	--uart0_trn_cnt;
}

// UART #0 received check.
void UART0_Rec_Check( void )
{
	// Check received count.
	if( uart0_rec_cnt < 15 ){
		return;
	}
	// Clear received count.
	uart0_rec_cnt = 0;
	// Check end character, '\n'.
	if( uart0_rec_data[14] != 0x0a ){
		return;
	}
	// Check end character, '\r'.
	if( uart0_rec_data[13] != 0x0d ){
		return;
	}
	// Check end character '.'.
	if( uart0_rec_data[12] != '.' ){
		return;
	}
	// Get input control/value.
	temp2 = uart0_rec_data[10];
	ToValue();
	if( temp1 == 0x10 ){
		return;
	}
	chk_value = temp1;
	chk_value <<= 4;
	
	temp2 = uart0_rec_data[11];
	ToValue();
	if( temp1 == 0x10 ){
		return;
	}
	chk_value |= temp1;
	
	if( chk_value == 0 ){
		// LED off.
		PORTB &= 0xfd;
	}
	else if( chk_value == 0x01 ){
		// LED on.
		PORTB |= 0x02;
	}
	else if( chk_value == 0x02 ){
		// Update characteristic value for remote read.
		temp1 = adc_value;
		temp1 >>= 4;
		ToChar();	
		rn4020_char_value[2] = temp1;
		temp1 = adc_value;
		temp1 &= 0x0f;
		ToChar();
		rn4020_char_value[3] = temp1;
		// Set task count.
		uart0_trn_task = 3;		
	}
}

// Character to value.
void ToValue( void )
{
	temp1 = temp2;
	temp1 &= 0xf0;
	
	// '0' ~ '9'.
	if( temp1 == 0x30 ){
		temp1 = temp2;
		temp1 &= 0x0f;
		if( temp1 < 10 ){
			return;
		}
	}
	// 'A' ~ 'F'.
	else if( temp1 == 0x40 ){
		temp1 = temp2;
		temp1 &= 0x0f;
		if( temp1 > 0 && temp1 < 7 ){
			temp1 += 10;
			--temp1;
			return;
		}
	}
	// 'a' ~ 'f'.
	else if( temp1 == 0x60 ){
		temp1 = temp2;
		temp1 &= 0x0f;
		if( temp1 > 0 && temp1 < 7 ){
			temp1 += 10;
			--temp1;
			return;
		}
	}
	// Others.
	// Input error.	
	temp1 = 0x10;
}

// Value to character.
void ToChar( void )
{
	// 0 ~ 9.
	if( temp1 < 10 ){
		temp1 += 0x30;
		return;
	}
	// a ~ f.
	temp1 -= 10;
	temp1 += 0x61;	
}