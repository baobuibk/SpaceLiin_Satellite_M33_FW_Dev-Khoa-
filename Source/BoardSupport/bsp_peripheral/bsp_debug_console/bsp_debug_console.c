/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <stdio.h>

/* Freescale includes. */
#include "fsl_common.h"
#include "fsl_lpuart.h"

/* Board Support includes. */
#include "bsp_board.h"
#include "bsp_debug_console.h"

/* Component includes. */
#include "uart_stdio.h"
#include "ring_buffer.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define DEBUG_DB9_PRINTF_BUFFER_SIZE    128
#define DEBUG_DB9_IRQHandler            LPUART2_IRQHandler

//*****************************************************************************
//
// Macros to determine number of free and used bytes in the transmit buffer.
//
//*****************************************************************************
#define TX_BUFFER_SIZE(p_uart)              ((p_uart)->max_items)

#define TX_BUFFER_USED(p_uart)              (ring_char_buffer_get_buffer_count((p_uart)->p_TX_buffer))

#define TX_BUFFER_FREE(p_uart)              (ring_char_buffer_get_free_space((p_uart)->p_TX_buffer))

#define TX_BUFFER_EMPTY(p_uart)             (ring_char_buffer_is_empty((p_uart)->p_TX_buffer))

#define TX_BUFFER_FULL(p_uart)              (ring_char_buffer_is_full((p_uart)->p_TX_buffer))

#define TX_BUFFER_PUSH(p_uart, char)        (ring_char_buffer_push((p_uart)->p_TX_buffer, (char)))

#define TX_BUFFER_POP(p_uart, p_buff)       (ring_char_buffer_pop((p_uart)->p_TX_buffer, (p_buff)))

#define TX_BUFFER_OVERWRITE(p_uart, char)   (ring_char_buffer_overwrite((p_uart)->p_TX_buffer, (char)))

#define TX_BUFFER_PEAK(p_uart, p_buff)      (ring_char_buffer_peak((p_uart)->p_TX_buffer, (p_buff)))

//*****************************************************************************
//
// Macros to determine number of free and used bytes in the receive buffer.
//
//*****************************************************************************
#define RX_BUFFER_SIZE(p_uart)              ((p_uart)->max_items)

#define RX_BUFFER_USED(p_uart)              (ring_char_buffer_get_buffer_count((p_uart)->p_RX_buffer))

#define RX_BUFFER_FREE(p_uart)              (ring_char_buffer_get_free_space((p_uart)->p_RX_buffer))

#define RX_BUFFER_EMPTY(p_uart)             (ring_char_buffer_is_empty((p_uart)->p_RX_buffer))

#define RX_BUFFER_FULL(p_uart)              (ring_char_buffer_is_full((p_uart)->p_RX_buffer))

#define RX_BUFFER_PUSH(p_uart, char)        (ring_char_buffer_push((p_uart)->p_RX_buffer, (char)))

#define RX_BUFFER_POP(p_uart, p_buff)       (ring_char_buffer_pop((p_uart)->p_RX_buffer, (p_buff)))

#define RX_BUFFER_OVERWRITE(p_uart, char)   (ring_char_buffer_overwrite((p_uart)->p_RX_buffer, (char)))

#define RX_BUFFER_PEAK(p_uart, p_buff)      (ring_char_buffer_peak((p_uart)->p_RX_buffer, (p_buff)))

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
uart_stdio_t        DEBUG_DB9_UART;

ring_char_buffer_t  DEBUG_DB9_UART_TX_ring_buffer;
uint8_t             g_DEBUG_DB9_UART_TX_buffer[2048];

ring_char_buffer_t  DEBUG_DB9_UART_RX_ring_buffer;
uint8_t             g_DEBUG_DB9_UART_RX_buffer[64];

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//*****************************************************************************
//
//! Init the bsp debug console
//
//*****************************************************************************
void bsp_debug_console_init()
{
    ring_char_buffer_init(&DEBUG_DB9_UART_TX_ring_buffer, g_DEBUG_DB9_UART_TX_buffer, sizeof(g_DEBUG_DB9_UART_TX_buffer));
    ring_char_buffer_init(&DEBUG_DB9_UART_RX_ring_buffer, g_DEBUG_DB9_UART_RX_buffer, sizeof(g_DEBUG_DB9_UART_RX_buffer));

    UART_stdio_Init(&DEBUG_DB9_UART, DEBUG_DB9_LPUART_BASE, DEBUG_DB9_LPUART_IRQn,
                    &DEBUG_DB9_UART_TX_ring_buffer, &DEBUG_DB9_UART_RX_ring_buffer);

    EnableIRQ(DEBUG_DB9_LPUART_IRQn);
}

//*****************************************************************************
//
//! Debug DB9 LPUART IRQHandler
//
//*****************************************************************************
void DEBUG_DB9_IRQHandler(void)
{
    if((kLPUART_TxDataRegEmptyFlag & LPUART_GetStatusFlags(DEBUG_DB9_LPUART_BASE)) == kLPUART_TxDataRegEmptyFlag)
    {
        if(TX_BUFFER_EMPTY(&DEBUG_DB9_UART))
        {
            // Buffer empty, so disable interrupts
            LPUART_DisableInterrupts(DEBUG_DB9_LPUART_BASE, kLPUART_TxDataRegEmptyInterruptEnable);
        }
        else
        {
            // There is more data in the output buffer. Send the next byte
            UART_Prime_Transmit(&DEBUG_DB9_UART);
        }
    }

    if((kLPUART_RxDataRegFullFlag & LPUART_GetStatusFlags(DEBUG_DB9_LPUART_BASE)) == kLPUART_RxDataRegFullFlag)
    {
        DEBUG_DB9_UART.RX_irq_char = (char)LPUART_ReadByte(DEBUG_DB9_LPUART_BASE);

        // NOTE: On win 10, default PUTTY when hit enter only send back '\r',
        // while on default HERCULES when hit enter send '\r\n' in that order.
        // The code bellow is modified so that it can work on PUTTY and HERCULES.
        if((!RX_BUFFER_FULL(&DEBUG_DB9_UART)) && (DEBUG_DB9_UART.RX_irq_char != '\n'))
        {
            if (DEBUG_DB9_UART.RX_irq_char == '\r')
            {
                RX_BUFFER_PUSH(&DEBUG_DB9_UART, '\n');
            }
            else
            {
                RX_BUFFER_PUSH(&DEBUG_DB9_UART, DEBUG_DB9_UART.RX_irq_char);
            }
        }
    }

    SDK_ISR_EXIT_BARRIER;
}

//*****************************************************************************
//
//! Print string with format to the debug console.
//
//*****************************************************************************
void bsp_debug_console_printf(const char * format, ...)
{
    char temp_buffer[DEBUG_DB9_PRINTF_BUFFER_SIZE];
    va_list args;

    // Khởi tạo danh sách tham số biến đổi
    va_start(args, format);

    // Định dạng chuỗi vào bộ đệm tạm
    int len = vsnprintf(temp_buffer, DEBUG_DB9_PRINTF_BUFFER_SIZE, format, args);

    // Kết thúc danh sách tham số
    va_end(args);

    // Kiểm tra độ dài hợp lệ
    if (len < 0 || len >= DEBUG_DB9_PRINTF_BUFFER_SIZE) {
        return ;
    }

    // Gửi chuỗi đã định dạng qua UART
    UART_Write(&DEBUG_DB9_UART, temp_buffer, (uint16_t)len);
}

//*****************************************************************************
//
//! Send a char to the debug console.
//
//*****************************************************************************
void bsp_debug_console_send_char(const char Char)
{
	UART_Send_Char(&DEBUG_DB9_UART, Char);
}

//*****************************************************************************
//
//! Send the string to the debug console.
//!
//! \param pcBuf points to a buffer containing the string to transmit.
//
//*****************************************************************************
void bsp_debug_console_send_string(const char *pcBuf)
{
	UART_Send_String(&DEBUG_DB9_UART, pcBuf);
}

//*****************************************************************************
//
//! Get a char from the buffer.
//
//*****************************************************************************
char bsp_debug_console_get_char(void)
{
	return UART_Get_Char(&DEBUG_DB9_UART);
}

//*****************************************************************************
//
//! Check wheter RX buffer empty or not.
//
//*****************************************************************************
bool bsp_debug_console_RX_buffer_empty(void)
{
	return RX_BUFFER_EMPTY(&DEBUG_DB9_UART);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
