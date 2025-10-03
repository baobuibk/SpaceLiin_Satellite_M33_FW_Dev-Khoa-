/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "sp_spi_stdio_port.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/**
 * @brief Initialize the SPI peripheral with the provided configuration
 * @param me Pointer to the SPI_Io_t structure
 * @param init Pointer to the SPI_IoInit_t structure containing initialization parameters
 * @return ERROR_OK on success, or an error code on failure
 */
void sp_spi_master_init(sp_spi_t *me, const sp_spi_init_t *init)
{
    if (!me || !init || !init->p_spi_base) 
    {
        return;
    }

    // Copy initialization parameters to the SPI_Io_t structure
    me->p_spi_base     = init->p_spi_base;
    me->irqn           = init->irqn;

    me->srcClockHz     = init->srcClockHz;
    me->baudHz         = init->baudHz;

    me->mode           = init->mode;
    me->bitOrder       = init->bitOrder;
    me->csLevel        = init->csLevel;

    me->bitsPerFrame   = init->bitsPerFrame ? init->bitsPerFrame : 8;
    me->pcs            = init->pcs; // 0..3
    me->pcsContinuous  = init->pcsContinuous;
    
    me->bBusy          = false;

    return;
}

void sp_spi_master_write_blocking(sp_spi_t* me, const uint8_t* p_TX_buff, uint32_t length)
{
	sp_spi_master_transfer_blocking(me, p_TX_buff, NULL, length);
}

void sp_spi_master_read_blocking(sp_spi_t* me, uint8_t* p_RX_buff, uint32_t length)
{
	sp_spi_master_transfer_blocking(me, NULL, p_RX_buff, length);
}

void sp_spi_master_transfer_blocking(sp_spi_t* me, const uint8_t* p_TX_buff, uint8_t* p_RX_buff, uint32_t length)
{
	NVIC_DisableIRQ(me->irqn);
	me->bBusy = true;
	NVIC_EnableIRQ(me->irqn);

    lpspi_transfer_t transfer_handle;
    transfer_handle.txData      = (uint8_t*)p_TX_buff;
    transfer_handle.rxData      = p_RX_buff;
    transfer_handle.dataSize    = length;

    transfer_handle.configFlags = ((me->pcs & 3u) << LPSPI_MASTER_PCS_SHIFT) |
            (me->pcsContinuous ? kLPSPI_MasterPcsContinuous : 0);

    LPSPI_MasterTransferBlocking(me->p_spi_base, &transfer_handle);

	NVIC_DisableIRQ(me->irqn);
	me->bBusy = false;
	NVIC_EnableIRQ(me->irqn);

	return;
}

bool sp_spi_is_busy(sp_spi_t *me)
{
	return me->bBusy;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */