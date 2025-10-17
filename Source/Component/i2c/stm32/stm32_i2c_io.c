/*
 * stm32_i2c_io.c
 *
 *  Created on: Oct 11, 2025
 *      Author: Admin
 */

#include "i2c_io.h"
#include "stm32f7xx.h"

#define I2C_MAX_NUM 3

static I2C_TypeDef * const i2c_periph[I2C_MAX_NUM + 1] = {
    NULL,
    I2C1,
    I2C2,
    I2C3
};

uint32_t i2c_io_send(struct i2c_io_t *me, uint8_t ui8SlaveAddr, const char *buf, int count)
{
    if (!me || count <= 0 || !buf) {
        return 0;
    }

    uint32_t ui32I2cNum = me->ui32I2cPort;
    if (ui32I2cNum == 0 || ui32I2cNum > I2C_MAX_NUM) {
        return 0;
    }

    I2C_TypeDef *i2c = i2c_periph[ui32I2cNum];
    if (!i2c) {
        return 0;
    }

    // Wait for BUSY to clear
    while (i2c->ISR & I2C_ISR_BUSY) {}

    // Configure timing (example for 100kHz, adjust as needed)
    i2c->TIMINGR = 0x00B01A4B;  // Standard mode, 100kHz assuming 42MHz PCLK

    // Set slave address and number of bytes
    i2c->CR2 = (ui8SlaveAddr << I2C_CR2_SADD_Pos) | (count << I2C_CR2_NBYTES_Pos) | I2C_CR2_AUTOEND;

    // Generate START for write
    i2c->CR2 |= I2C_CR2_RD_WRN_Msk;
    i2c->CR2 |= I2C_CR2_START;

    // Send data bytes
    for (int i = 0; i < count; i++) {
        while (!(i2c->ISR & I2C_ISR_TXIS)) {}
        i2c->TXDR = (uint8_t)buf[i];
    }

    // Generate STOP
    i2c->CR2 |= I2C_CR2_STOP;

    return (uint32_t)count;
}

uint32_t i2c_io_recv(struct i2c_io_t *me, uint8_t ui8SlaveAddr, char *buf, int count)
{
    if (!me || count <= 0 || !buf) {
        return 0;
    }

    uint32_t ui32I2cNum = me->ui32I2cPort;
    if (ui32I2cNum == 0 || ui32I2cNum > I2C_MAX_NUM) {
        return 0;
    }

    I2C_TypeDef *i2c = i2c_periph[ui32I2cNum];
    if (!i2c) {
        return 0;
    }

    // Wait for BUSY to clear
    while (i2c->ISR & I2C_ISR_BUSY) {}

    // Configure timing (example for 100kHz, adjust as needed)
    i2c->TIMINGR = 0x00B01A4B;  // Standard mode, 100kHz assuming 42MHz PCLK

    // Set slave address and number of bytes
    i2c->CR2 = (ui8SlaveAddr << I2C_CR2_SADD_Pos) | (count << I2C_CR2_NBYTES_Pos) | I2C_CR2_AUTOEND;

    // Generate START for read
    i2c->CR2 &= ~I2C_CR2_RD_WRN_Msk;
    i2c->CR2 |= I2C_CR2_START;

    // Read data bytes
    for (int i = 0; i < count; i++) {
        while (!(i2c->ISR & I2C_ISR_RXNE)) {}
        buf[i] = (char)i2c->RXDR;
    }

    return (uint32_t)count;
}
