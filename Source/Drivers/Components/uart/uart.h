/*
 * uart.h
 *
 *  Created on: Oct 4, 2025
 *      Author: Admin
 */

#ifndef DRIVERS_UART_UART_H_
#define DRIVERS_UART_UART_H_
#include "stdint.h"
#include "stdbool.h"

enum {
	UART_INT_TXE,
	UART_INT_RXNE,
	UART_INT_ORE
};


void uart_active(uint32_t ui32UartNum, uint32_t ui32IntPri);
void uart_deactive(uint32_t ui32UartNum);
void uart_send_byte_async(uint32_t ui32UartNum, uint8_t data);
uint8_t uart_receive_byte(uint32_t ui32UartNum);
void uart_enable_interrupt(uint32_t ui32UartNum, uint32_t ui32IntNumber);
void uart_disable_interrupt(uint32_t ui32UartNum, uint32_t ui32IntNumber);

bool uart_is_active_flag(uint32_t ui32UartNum, uint32_t ui32IntNumber);
bool uart_clear_flag(uint32_t ui32UartNum, uint32_t ui32IntNumber);
#endif /* DRIVERS_UART_UART_H_ */
