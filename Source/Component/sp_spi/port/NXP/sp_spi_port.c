/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "sp_spi_port.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*! blocking TX buffer dataSize */
#define TX_BLOCKING_SIZE 64U
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static uint8_t TX_blocking_buffer[TX_BLOCKING_SIZE] = {0U};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static uint8_t SP_SPI_make_header(uint8_t reg_addr, sp_spi_command_t command_type);
static void SP_SPI_prepare_TX_data(sp_spi_frame_t* p_frame, sp_spi_command_t command_type);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/**
 * @brief Initialize the SPI peripheral with the provided configuration
 * @param me Pointer to the SPI_Io_t structure
 * @param init Pointer to the SPI_IoInit_t structure containing initialization parameters
 * @return ERROR_OK on success, or an error code on failure
 */
void SP_SPI_master_init(sp_spi_t *me, const sp_spi_init_t *init)
{
    if (!me || !init || !init->p_spi_base) 
    {
        return;
    }

    // Copy initialization parameters to the SPI_Io_t structure
    me->p_spi_base     = init->p_spi_base;
    // me->irqn           = init->irqn;

    // me->srcClockHz     = init->srcClockHz;
    // me->baudHz         = init->baudHz;

    // me->mode           = init->mode;
    // me->bitOrder       = init->bitOrder;
    // me->csLevel        = init->csLevel;

    // me->bitsPerFrame   = init->bitsPerFrame ? init->bitsPerFrame : 8;
    me->pcs            = init->pcs; // 0..3
    me->pcsContinuous  = init->pcsContinuous;
    
    // me->bBusy          = false;

    return;
}

void SP_SPI_master_write_blocking(sp_spi_t* me, sp_spi_frame_t* p_frame)
{
	SP_SPI_master_transfer_blocking(me, p_frame, SPI_WRITE);
}

void SP_SPI_master_read_blocking(sp_spi_t* me, sp_spi_frame_t* p_frame)
{
	SP_SPI_master_transfer_blocking(me, p_frame, SPI_READ);
}

void SP_SPI_master_transfer_blocking(sp_spi_t* me, sp_spi_frame_t* p_frame, sp_spi_command_t command_type)
{
	// NVIC_DisableIRQ(me->irqn);
	// me->bBusy = true;
	// NVIC_EnableIRQ(me->irqn);

    SP_SPI_prepare_TX_data(p_frame, command_type);

    lpspi_transfer_t transfer_handle;
    transfer_handle.txData      = TX_blocking_buffer;
    transfer_handle.rxData      = p_frame->p_RX_buffer;
    transfer_handle.dataSize    = p_frame->data_size + 1; // + 1 for addr

    transfer_handle.configFlags = ((me->pcs & 3u) << LPSPI_MASTER_PCS_SHIFT) |
            (me->pcsContinuous ? kLPSPI_MasterPcsContinuous : 0);

    LPSPI_MasterTransferBlocking(me->p_spi_base, &transfer_handle);

	// NVIC_DisableIRQ(me->irqn);
	// me->bBusy = false;
	// NVIC_EnableIRQ(me->irqn);

	return;
}

// bool SP_SPI_is_busy(sp_spi_t *me)
// {
// 	return me->bBusy;
// }

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static uint8_t SP_SPI_make_header(uint8_t reg_addr, sp_spi_command_t command_type)
{
    // Tạo header: [reg_addr(6b) | command(2b)]
    return (uint8_t)((reg_addr << 2) | (command_type & 0x03));
}

static void SP_SPI_prepare_TX_data(sp_spi_frame_t* p_frame, sp_spi_command_t command_type)
{
    TX_blocking_buffer[0] = SP_SPI_make_header(p_frame->addr, command_type);

    for (uint32_t index = 0; index < p_frame->data_size; index++)
    {
        TX_blocking_buffer[index + 1] = ((command_type & 0x03) == SPI_READ) 
                                        ? 0xFF 
                                        : p_frame->p_TX_buffer[index];
    }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */