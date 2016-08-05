/*
 * var.h
 *
 * Created: 2016/6/26 下午 11:54:25
 *  Author: tom hsieh
 */ 

#include <avr/io.h>

#ifndef VAR_H_
#define VAR_H_

// RN4020, write server service characteristic value.  
uint8_t rn4020_char_value[6];

// Temporary #1.
uint8_t temp1;
// Temporary #2.
uint8_t temp2;

// UART #0 data received
uint8_t uart0_rec_data[17];
// UART #0 received data count.
uint8_t uart0_rec_cnt;
// UART #0 received timeout.
uint8_t uart0_rec_to;
// UART #0 transmit data. 
uint8_t uart0_trn_data[16];
// UART #0 output data pointer.
uint8_t *puart0_trn_data;
// UART #0 data transmitted count.
uint8_t uart0_trn_cnt;
// UART #0 transmit task count.
uint8_t uart0_trn_task;

// ADC value.
uint8_t adc_value;
// Check value.
uint8_t chk_value;

#endif /* VAR_H_ */