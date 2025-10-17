/*
 * spi_io.c
 *
 *  Created on: Oct 10, 2025
 *      Author: Admin
 */

#include "spi_io.h"
#include "stm32f7xx.h"
#include <stdbool.h>
#include <string.h>  // for memset if needed

#define SPI_MAX_BUS_NUMBER 6

static SPI_TypeDef * const spi_periph[SPI_MAX_BUS_NUMBER + 1] = {
    NULL,
    SPI1,
    SPI2,
    SPI3,
    SPI4,
    SPI5,
    SPI6
};

static uint32_t const spi_clock_en[SPI_MAX_BUS_NUMBER + 1] = {
    0,
    RCC_APB2ENR_SPI1EN,
    RCC_APB1ENR_SPI2EN,
    RCC_APB1ENR_SPI3EN,
    RCC_APB2ENR_SPI4EN,
    RCC_APB2ENR_SPI5EN,
    RCC_APB2ENR_SPI6EN
};

// Assume osSemaphoreTake(osSemaphore *sem, uint32_t timeout) returns status, success OS_SEMAPHORE_OK
// Assume osSemaphoreGiven(osSemaphore *sem)
// Assume ERROR_BUSY and ERROR_NOT_SUPPORTED defined elsewhere

static __attribute__((unused)) void spi_enable_clock(uint32_t ui32SpiNum)
{
    if (ui32SpiNum == 0 || ui32SpiNum > SPI_MAX_BUS_NUMBER) {
        return;
    }

    if (ui32SpiNum == 1 || ui32SpiNum == 4 || ui32SpiNum == 5 || ui32SpiNum == 6) {
        RCC->APB2ENR |= spi_clock_en[ui32SpiNum];
    } else {
        RCC->APB1ENR |= spi_clock_en[ui32SpiNum];
    }
}

uint32_t spi_io_transfer_sync(SPI_Io_t *me, uint8_t	*pui8TxBuff, uint8_t	*pui8RxBuff, uint32_t ui32Length)
{
    SPI_TypeDef *spi = spi_periph[me->ui32SpiPort];
    uint32_t i;
    uint8_t data = 0;
    for (i = 0; i < ui32Length; i++) {
        // Wait for TXE
        while (!(spi->SR & SPI_SR_TXE)) {}
        data = pui8TxBuff[i];
        spi->DR = data;
        // Wait for RXNE
        while (!(spi->SR & SPI_SR_RXNE)) {}
        uint8_t rec = (uint8_t)spi->DR;
        pui8RxBuff[i] = rec;
        }
    // Wait for not busy
    while (spi->SR & SPI_SR_BSY) {}
    return ERROR_OK;
}

/******************************************************************************/
/*************************** Synchronous Functions ****************************/
/******************************************************************************/

uint32_t spi_io_read_sync(SPI_Io_t *me, uint8_t *pui8RxBuff, uint32_t ui32Length)
{
    SPI_TypeDef *spi = spi_periph[me->ui32SpiPort];
    uint32_t i;
    for (i = 0; i < ui32Length; i++) {
        // Wait for TXE
        while (!(spi->SR & SPI_SR_TXE)) {}
        spi->DR = 0;
        // Wait for RXNE
        while (!(spi->SR & SPI_SR_RXNE)) {}
        uint8_t rec = (uint8_t)spi->DR;
        pui8RxBuff[i] = rec;
        }
    // Wait for not busy
    while (spi->SR & SPI_SR_BSY) {}
    return ERROR_OK;
}

uint32_t spi_io_write_sync(SPI_Io_t *me, uint8_t *pui8TxBuff, uint32_t ui32Length)
{
    SPI_TypeDef *spi = spi_periph[me->ui32SpiPort];
    uint32_t i;
    uint8_t data = 0;
    for (i = 0; i < ui32Length; i++) {
        // Wait for TXE
        while (!(spi->SR & SPI_SR_TXE)) {}
        data = pui8TxBuff[i];
        spi->DR = data;
        // Wait for RXNE
        while (!(spi->SR & SPI_SR_RXNE)) {}
        (void)spi->DR;
        }
    // Wait for not busy
    while (spi->SR & SPI_SR_BSY) {}
    return ERROR_OK;
}

/******************************************************************************/
/**************************** Asynchronous Functions **************************/
/******************************************************************************/

// Stub implementations for async (not supported)
uint32_t spi_io_read_async(SPI_Io_t *me, uint8_t *pui8RxBuff, uint32_t ui32Length)
{
    (void)me; (void)pui8RxBuff; (void)ui32Length;
    return ERROR_NOT_SUPPORTED;
}

uint32_t spi_io_write_async(SPI_Io_t *me, uint8_t *pui8TxBuff, uint32_t ui32Length)
{
    (void)me; (void)pui8TxBuff; (void)ui32Length;
    return ERROR_NOT_SUPPORTED;
}

uint32_t spi_io_transfer_async(SPI_Io_t *me, uint8_t *pui8TxBuff, uint8_t *pui8RxBuff, uint32_t ui32Length)
{
    (void)me; (void)pui8TxBuff; (void)pui8RxBuff; (void)ui32Length;
    return ERROR_NOT_SUPPORTED;
}

uint32_t spi_io_write_and_read_async(SPI_Io_t *me, uint8_t *pui8TxBuff, uint32_t ui32TxLength, uint8_t *pui8RxBuff, uint32_t ui32RxLength)
{
    (void)me; (void)pui8TxBuff; (void)ui32TxLength; (void)pui8RxBuff; (void)ui32RxLength;
    return ERROR_NOT_SUPPORTED;
}

/******************************************************************************/
/******************************** DMA Functions *******************************/
// Stub implementations for DMA (not supported)
uint32_t spi_io_read_dma(SPI_Io_t *me, uint8_t *pui8RxBuff, uint32_t ui32Length)
{
    (void)me; (void)pui8RxBuff; (void)ui32Length;
    return ERROR_NOT_SUPPORTED;
}

uint32_t spi_io_write_dma(SPI_Io_t *me, uint8_t *pui8TxBuff, uint32_t ui32Length)
{
    (void)me; (void)pui8TxBuff; (void)ui32Length;
    return ERROR_NOT_SUPPORTED;
}

uint32_t spi_io_transfer_dma(SPI_Io_t *me, uint8_t *pui8TxBuff, uint8_t *pui8RxBuff, uint32_t ui32Length)
{
    (void)me; (void)pui8TxBuff; (void)pui8RxBuff; (void)ui32Length;
    return ERROR_NOT_SUPPORTED;
}

uint32_t spi_io_write_and_read_dma(SPI_Io_t *me, uint8_t *pui8TxBuff, uint32_t ui32TxLength, uint8_t *pui8RxBuff, uint32_t ui32RxLength)
{
    (void)me; (void)pui8TxBuff; (void)ui32TxLength; (void)pui8RxBuff; (void)ui32RxLength;
    return ERROR_NOT_SUPPORTED;
}
