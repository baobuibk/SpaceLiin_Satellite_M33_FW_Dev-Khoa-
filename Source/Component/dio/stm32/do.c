/*
 * do.c
 *
 *  Created on: Oct 10, 2025
 *      Author: Admin
 */

#include "do.h"
#include "stm32f7xx.h"
#include <stdbool.h>
#include "stddef.h"

#define GPIO_MAX_PORTS 11  // A to K

static GPIO_TypeDef * const gpio_ports[GPIO_MAX_PORTS + 1] = {
    NULL,
    GPIOA,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOE,
    GPIOF,
    GPIOG,
    GPIOH,
    GPIOI,
    GPIOJ,
    GPIOK
};

static uint32_t const gpio_clock_en[GPIO_MAX_PORTS + 1] = {
    0,
    RCC_AHB1ENR_GPIOAEN,
    RCC_AHB1ENR_GPIOBEN,
    RCC_AHB1ENR_GPIOCEN,
    RCC_AHB1ENR_GPIODEN,
    RCC_AHB1ENR_GPIOEEN,
    RCC_AHB1ENR_GPIOFEN,
    RCC_AHB1ENR_GPIOGEN,
    RCC_AHB1ENR_GPIOHEN,
    RCC_AHB1ENR_GPIOIEN,
    RCC_AHB1ENR_GPIOJEN,
    RCC_AHB1ENR_GPIOKEN
};

static void gpio_enable_clock(uint32_t ui32PortNum)
{
    if (ui32PortNum == 0 || ui32PortNum > GPIO_MAX_PORTS) {
        return;
    }

    RCC->AHB1ENR |= gpio_clock_en[ui32PortNum];
}

void do_set(do_t *me)
{
    if (!me) {
        return;
    }

    if (me->port == 0 || me->port > GPIO_MAX_PORTS || me->pin >= 16) {
        return;
    }

    GPIO_TypeDef *port = gpio_ports[me->port];
    if (!port) {
        return;
    }

    gpio_enable_clock(me->port);
    // Set the pin high using BSRR (atomic set)
    port->BSRR = (1UL << me->pin);
    me->bStatus = true;
}

void do_reset(do_t *me)
{
    if (!me) {
        return;
    }

    if (me->port == 0 || me->port > GPIO_MAX_PORTS || me->pin >= 16) {
        return;
    }

    GPIO_TypeDef *port = gpio_ports[me->port];
    if (!port) {
        return;
    }

    gpio_enable_clock(me->port);
    // Set the pin low using BSRR (atomic reset)
    port->BSRR = (1UL << (me->pin + 16));
    me->bStatus = false;
}
