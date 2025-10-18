#ifndef _UART_STDIO_H_
#define _UART_STDIO_H_

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Include ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "fsl_common.h"

#include "ring_char_buffer.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
typedef struct _uart_stdio_t_
{
    LPUART_Type*            handle;
    IRQn_Type		        irqn;

    ring_char_buffer_t*     p_TX_buffer;
    ring_char_buffer_t*     p_RX_buffer;
    volatile    uint8_t     RX_irq_char;
} uart_stdio_t;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void UART_stdio_Init( uart_stdio_t* p_uart, LPUART_Type* _handle,
                IRQn_Type _irqn, ring_char_buffer_t* _p_TX_buffer, ring_char_buffer_t* _p_RX_buffer);

void        UART_Send_Char(uart_stdio_t* p_uart, const char Char);
void        UART_Send_String(uart_stdio_t* p_uart, const char *pcBuf);
uint16_t    UART_Write(uart_stdio_t* p_uart, const char *pcBuf, uint16_t ui16Len);
void        UART_Printf(uart_stdio_t* p_uart, const char *pc_string, ...);
uint16_t    UART_FSP(uart_stdio_t* p_uart, const char *pcBuf, uint16_t ui16Len);
char        UART_Get_Char(uart_stdio_t* p_uart);

void        UART_Prime_Transmit(uart_stdio_t* p_uart);

#endif // _UART_STDIO_H_
