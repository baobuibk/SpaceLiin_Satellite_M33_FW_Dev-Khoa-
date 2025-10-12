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
    // IRQn_Type           irqn;

    // uint32_t            srcClockHz;
    // uint32_t            baudHz;
    
    // sp_spi_mode_t       mode;
    // sp_spi_bit_order_t  bitOrder;
    // sp_spi_cs_level_t   csLevel;
    
    // uint8_t             bitsPerFrame;
    uint8_t             pcs;
    bool                pcsContinuous;

    // bool                bBusy;
} sp_spi_t;

typedef struct _sp_spi_init_t_
{
    LPSPI_Type*         p_spi_base;
    // IRQn_Type           irqn;

    // uint32_t            srcClockHz;
    // uint32_t            baudHz;
    
    // sp_spi_mode_t       mode;
    // sp_spi_bit_order_t  bitOrder;
    // sp_spi_cs_level_t   csLevel;

    // uint8_t             bitsPerFrame;
    uint8_t             pcs;
    bool                pcsContinuous;
} sp_spi_init_t;

// Only the 2 LSB are used for SPI header (00: WRITE, 01: READ)
// Higher bits can be used for internal logic.
typedef enum _sp_spi_command_t_
{
    SPI_WRITE = 0,                          // DO NOT MOVE OR CHANGE POSITION
    SPI_READ  = 1,                          // DO NOT MOVE OR CHANGE POSITION
} sp_spi_command_t;

typedef struct _sp_spi_frame_t_
{
    uint8_t     addr;
    uint8_t*    p_TX_buffer;
    uint8_t*    p_RX_buffer;

    uint32_t    data_size;
} sp_spi_frame_t;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#endif /* _SP_SPI_PORT_H_ */
