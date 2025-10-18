#include "uart.h"
#include "stm32f7xx.h"
#include <stdbool.h>
#include <stddef.h>

#define UART_MAX_NUM 6

static USART_TypeDef * const uart_periph[UART_MAX_NUM + 1] = {
    NULL,
    USART1,
    USART2,
    USART3,
    UART4,
    UART5,
    USART6
};
static IRQn_Type const uart_irq[UART_MAX_NUM + 1] = {
    0,
    USART1_IRQn,
    USART2_IRQn,
    USART3_IRQn,
    UART4_IRQn,
    UART5_IRQn,
    USART6_IRQn
};
static void uart_enable_clock(uint32_t ui32UartNum)
{
    if (ui32UartNum == 0 || ui32UartNum > UART_MAX_NUM) {
        return;
    }

    if (ui32UartNum == 1 || ui32UartNum == 6) {
        // APB2
        if (ui32UartNum == 1) {
            RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
        } else {
            RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
        }
    } else {
        // APB1
        switch (ui32UartNum) {
            case 2:
                RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
                break;
            case 3:
                RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
                break;
            case 4:
                RCC->APB1ENR |= RCC_APB1ENR_UART4EN;
                break;
            case 5:
                RCC->APB1ENR |= RCC_APB1ENR_UART5EN;
                break;
            default:
                break;
        }
    }
}

static void uart_disable_clock(uint32_t ui32UartNum)
{
    if (ui32UartNum == 0 || ui32UartNum > UART_MAX_NUM) {
        return;
    }

    if (ui32UartNum == 1 || ui32UartNum == 6) {
        // APB2
        if (ui32UartNum == 1) {
            RCC->APB2ENR &= ~RCC_APB2ENR_USART1EN;
        } else {
            RCC->APB2ENR &= ~RCC_APB2ENR_USART6EN;
        }
    } else {
        // APB1
        switch (ui32UartNum) {
            case 2:
                RCC->APB1ENR &= ~RCC_APB1ENR_USART2EN;
                break;
            case 3:
                RCC->APB1ENR &= ~RCC_APB1ENR_USART3EN;
                break;
            case 4:
                RCC->APB1ENR &= ~RCC_APB1ENR_UART4EN;
                break;
            case 5:
                RCC->APB1ENR &= ~RCC_APB1ENR_UART5EN;
                break;
            default:
                break;
        }
    }
}

void uart_active(uint32_t ui32UartNum, uint32_t ui32IntPri)
{
    USART_TypeDef *Uart;
    if (ui32UartNum == 0 || ui32UartNum > UART_MAX_NUM) {
        return;
    }

    Uart = uart_periph[ui32UartNum];
    if (!Uart) {
        return;
    }

    uart_enable_clock(ui32UartNum);
    // Basic configuration: 8N1, enable TX/RX
    Uart->CR1 |= (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);
    // Enable RXNE interrupt
        Uart->CR1 |= USART_CR1_RXNEIE;
        // Set NVIC priority for UART interrupt
        NVIC_SetPriority(uart_irq[ui32UartNum], ui32IntPri );
        // Enable NVIC interrupt for UART
        NVIC_EnableIRQ(uart_irq[ui32UartNum]);
    // Note: Baud rate must be set separately via BRR register if needed
}

void uart_deactive(uint32_t ui32UartNum)
{
    USART_TypeDef *Uart;
    if (ui32UartNum == 0 || ui32UartNum > UART_MAX_NUM) {
        return;
    }

    Uart = uart_periph[ui32UartNum];
    if (!Uart) {
        return;
    }

    // Disable all possible UART interrupts in CR1
    Uart->CR1 &= ~(USART_CR1_PEIE | USART_CR1_TXEIE | USART_CR1_TCIE | USART_CR1_RXNEIE | USART_CR1_IDLEIE | USART_CR1_CMIE | USART_CR1_RTOIE | USART_CR1_EOBIE);
    // Disable interrupts in CR2
    Uart->CR2 &= ~USART_CR2_LBDIE;
    // Disable interrupts in CR3
    Uart->CR3 &= ~(USART_CR3_EIE | USART_CR3_CTSIE);
    // Disable NVIC interrupt for UART
    NVIC_DisableIRQ(uart_irq[ui32UartNum]);
    // Disable UART
    Uart->CR1 &= ~USART_CR1_UE;
    // Disable clock
    uart_disable_clock(ui32UartNum);
}

void uart_send_byte_async(uint32_t ui32UartNum, uint8_t data)
{
    USART_TypeDef *Uart;
    if (ui32UartNum == 0 || ui32UartNum > UART_MAX_NUM) {
        return;
    }

    Uart = uart_periph[ui32UartNum];
    if (!Uart) {
        return;
    }

    // Write to DR to start transmission (async if TXEIE enabled)
    Uart->TDR = (uint32_t)data;
}

uint8_t uart_receive_byte(uint32_t ui32UartNum)
{
    USART_TypeDef *Uart;
    if (ui32UartNum == 0 || ui32UartNum > UART_MAX_NUM) {
        return 0;
    }

    Uart = uart_periph[ui32UartNum];
    if (!Uart) {
        return 0;
    }

    // Blocking wait for RXNE
    while (!(Uart->ISR & USART_ISR_RXNE)) {
        // Optional: timeout handling can be added here
    }

    return (uint8_t)(Uart->RDR & 0xFF);
}

void uart_enable_interrupt(uint32_t ui32UartNum, uint32_t ui32IntNumber)
{
    USART_TypeDef *Uart;
    if (ui32UartNum == 0 || ui32UartNum > UART_MAX_NUM) {
        return;
    }

    Uart = uart_periph[ui32UartNum];
    if (!Uart) {
        return;
    }

    switch (ui32IntNumber) {
        case UART_INT_TXE:
            Uart->CR1 |= USART_CR1_TXEIE;
            break;
        case UART_INT_RXNE:
            Uart->CR1 |= USART_CR1_RXNEIE;
            break;
        case UART_INT_ORE:
            // ORE interrupt is enabled via RXNEIE
            Uart->CR1 |= USART_CR1_RXNEIE;
            break;
        default:
            break;
    }
}

void uart_disable_interrupt(uint32_t ui32UartNum, uint32_t ui32IntNumber)
{
    USART_TypeDef *Uart;
    if (ui32UartNum == 0 || ui32UartNum > UART_MAX_NUM) {
        return;
    }

    Uart = uart_periph[ui32UartNum];
    if (!Uart) {
        return;
    }

    switch (ui32IntNumber) {
        case UART_INT_TXE:
            Uart->CR1 &= ~USART_CR1_TXEIE;
            break;
        case UART_INT_RXNE:
            Uart->CR1 &= ~USART_CR1_RXNEIE;
            break;
        case UART_INT_ORE:
            // ORE interrupt is disabled via RXNEIE
            Uart->CR1 &= ~USART_CR1_RXNEIE;
            break;
        default:
            break;
    }
}

bool uart_is_active_flag(uint32_t ui32UartNum, uint32_t ui32IntNumber)
{
    USART_TypeDef *Uart;
    if (ui32UartNum == 0 || ui32UartNum > UART_MAX_NUM) {
        return false;
    }

    Uart = uart_periph[ui32UartNum];
    if (!Uart) {
        return false;
    }

    uint32_t flag_mask = 0;
    switch (ui32IntNumber) {
        case UART_INT_TXE:
            flag_mask = USART_ISR_TXE;
            break;
        case UART_INT_RXNE:
            flag_mask = USART_ISR_RXNE;
            break;
        case UART_INT_ORE:
            flag_mask = USART_ISR_ORE;
            break;
        default:
            return false;
    }

    return (Uart->ISR & flag_mask) != 0;
}

bool uart_clear_flag(uint32_t ui32UartNum, uint32_t ui32IntNumber)
{
    USART_TypeDef *Uart;
    if (ui32UartNum == 0 || ui32UartNum > UART_MAX_NUM) {
        return false;
    }

    Uart = uart_periph[ui32UartNum];
    if (!Uart) {
        return false;
    }

    bool was_set = false;
    switch (ui32IntNumber) {
        case UART_INT_TXE:
            // TXE is cleared by writing to DR; here we check if set, but can't force clear without data
            if (Uart->ISR & USART_ISR_TXE) {
                was_set = true;
                // Optionally write dummy byte, but avoided to prevent unintended transmission
            }
            return was_set;
        case UART_INT_RXNE:
            if (Uart->ISR & USART_ISR_RXNE) {
                was_set = true;
                (void)Uart->RDR;  // Read RDR to clear RXNE
            }
            return was_set;
        case UART_INT_ORE:
            if (Uart->ISR & USART_ISR_ORE) {
                was_set = true;
                Uart->ICR |= USART_ICR_ORECF;  // Clear ORE flag
            }
            return was_set;
        default:
            return false;
    }
}
