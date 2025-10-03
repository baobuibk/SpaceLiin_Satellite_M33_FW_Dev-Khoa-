#ifndef _SP_SPI_PORT_H_
#define _SP_SPI_PORT_H_

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Include ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Standard API for SP SPI STDIO includes. */
#include "sp_spi_api.h"

/* Port specific includes. */
#include "fsl_lpspi.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
typedef struct _sp_spi_t_
{
    /* SPI bus address */
    LPSPI_Type*         p_spi_base;
    IRQn_Type           irqn;

    uint32_t            srcClockHz;
    uint32_t            baudHz;
    
    sp_spi_mode_t       mode;
    sp_spi_bit_order_t  bitOrder;
    sp_spi_cs_level_t   csLevel;
    
    uint8_t             bitsPerFrame;
    uint8_t             pcs;
    bool                pcsContinuous;

    bool                bBusy;
} sp_spi_t;

typedef struct _sp_spi_init_t_
{
    LPSPI_Type*         p_spi_base;
    IRQn_Type           irqn;

    uint32_t            srcClockHz;
    uint32_t            baudHz;
    
    sp_spi_mode_t       mode;
    sp_spi_bit_order_t  bitOrder;
    sp_spi_cs_level_t   csLevel;

    uint8_t             bitsPerFrame;
    uint8_t             pcs;
    bool                pcsContinuous;
} sp_spi_init_t;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#endif /* _SP_SPI_PORT_H_ */
