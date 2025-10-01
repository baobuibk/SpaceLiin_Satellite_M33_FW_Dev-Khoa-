/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <stdarg.h>

#include "can_stdio.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//*****************************************************************************
//
// Macros to determine number of free and used bytes in the transmit buffer.
//
//*****************************************************************************
#define TX_BUFFER_SIZE(p_can)              ((p_can)->max_items)

#define TX_BUFFER_USED(p_can)              (ring_buffer_get_buffer_count((p_can)->p_TX_buffer))

#define TX_BUFFER_FREE(p_can)              (ring_buffer_get_free_space((p_can)->p_TX_buffer))

#define TX_BUFFER_EMPTY(p_can)             (ring_buffer_is_empty((p_can)->p_TX_buffer))

#define TX_BUFFER_FULL(p_can)              (ring_buffer_is_full((p_can)->p_TX_buffer))

#define TX_BUFFER_PUSH(p_can, p_buff)      (ring_buffer_push((p_can)->p_TX_buffer, (p_buff)))

#define TX_BUFFER_POP(p_can, p_buff)       (ring_buffer_pop((p_can)->p_TX_buffer, (void*)(p_buff)))

#define TX_BUFFER_OVERWRITE(p_can, p_buff) (ring_buffer_overwrite((p_can)->p_TX_buffer, (p_buff)))

#define TX_BUFFER_PEAK(p_can, p_buff)      (ring_buffer_peak((p_can)->p_TX_buffer, (p_buff)))

//*****************************************************************************
//
// Macros to determine number of free and used bytes in the receive buffer.
//
//*****************************************************************************
#define RX_BUFFER_SIZE(p_can)              ((p_can)->max_items)

#define RX_BUFFER_USED(p_can)              (ring_buffer_get_buffer_count((p_can)->p_RX_buffer))

#define RX_BUFFER_FREE(p_can)              (ring_buffer_get_free_space((p_can)->p_RX_buffer))

#define RX_BUFFER_EMPTY(p_can)             (ring_buffer_is_empty((p_can)->p_RX_buffer))

#define RX_BUFFER_FULL(p_can)              (ring_buffer_is_full((p_can)->p_RX_buffer))

#define RX_BUFFER_PUSH(p_can, p_buff)      (ring_buffer_push((p_can)->p_RX_buffer, (p_buff)))

#define RX_BUFFER_POP(p_can, p_buff)       (ring_buffer_pop((p_can)->p_RX_buffer, (void*)(p_buff)))

#define RX_BUFFER_OVERWRITE(p_can, p_buff) (ring_buffer_overwrite((p_can)->p_RX_buffer, (p_buff)))

#define RX_BUFFER_PEAK(p_can, p_buff)      (ring_buffer_peak((p_can)->p_RX_buffer, (p_buff)))

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static void CAN_Enqueue(can_stdio_t *p_can, const flexcan_frame_t *frame, uint32_t frame_count);
static void CAN_Prime_Transmit(can_stdio_t* p_can);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void CAN_stdio_Init(  
                can_stdio_t *p_can,
                CAN_Type*   _p_base,
                IRQn_Type   _irqn,
                flexcan_handle_t*      _p_flexcan_handle,
                flexcan_mb_transfer_t* _p_TX_mb,
                flexcan_mb_transfer_t* _p_RX_mb,
                ring_buffer_t *_p_TX_buffer,
                ring_buffer_t *_p_RX_buffer)
{
    p_can->p_base     = _p_base;
    p_can->irqn     = _irqn;

    p_can->p_flexcan_handle = _p_flexcan_handle;
    p_can->p_TX_mb          = _p_TX_mb;
    p_can->p_RX_mb          = _p_RX_mb;

    p_can->p_TX_buffer  = _p_TX_buffer;
    p_can->p_RX_buffer  = _p_RX_buffer;

    p_can->is_TX_busy   = false;
    p_can->is_RX_busy   = false;

    FLEXCAN_TransferReceiveNonBlocking(p_can->p_base, p_can->p_flexcan_handle, p_can->p_RX_mb);
}

void CAN_Send_Frame(can_stdio_t* p_can, const flexcan_frame_t* p_frame)
{
    CAN_Enqueue(p_can, p_frame, 1);
}

void CAN_Send_Buffer(can_stdio_t *p_can, const flexcan_frame_t *p_buff, uint32_t frame_count)
{
    CAN_Enqueue(p_can, p_buff, frame_count);
}

void CAN_Get_Frame(can_stdio_t* p_can, flexcan_frame_t* p_return_frame)
{
    if (RX_BUFFER_EMPTY(p_can))
    {
        return;
    }
    
    //
    // Read a character from the buffer.
    //
    RX_BUFFER_POP(p_can, p_return_frame);

    //
    // Return the character to the caller.
    //
    return;
}

void CAN_stdio_TX_idle_subhandle(can_stdio_t* p_can)
{
    if(TX_BUFFER_EMPTY(p_can))
    {
        // Buffer empty, no more queue
        p_can->is_TX_busy = false;
        return;
    }

    // There is more data in the output buffer. Send the next byte
    p_can->is_TX_busy = true;
    CAN_Prime_Transmit(p_can);
    return;
}

void CAN_stdio_RX_idle_subhandle(can_stdio_t* p_can)
{
    if(!RX_BUFFER_FULL(p_can))
    {
        RX_BUFFER_PUSH(p_can, p_can->p_RX_mb->frame);
    }

    FLEXCAN_TransferReceiveNonBlocking(p_can->p_base, p_can->p_flexcan_handle, p_can->p_RX_mb);
    return;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static void CAN_Enqueue(can_stdio_t *p_can, const flexcan_frame_t *p_buff, uint32_t frame_count)
{
    uint8_t Idx;

    
    // Check for valid arguments.
    
    
    // Send the characters
    
    for(Idx = 0; Idx < frame_count; Idx++)
    {
        if(TX_BUFFER_FULL(p_can))
        {
            break;
        }

        TX_BUFFER_PUSH(p_can, &p_buff[Idx]);
    }

    //
    // If the usart txe irq is disable, this mean an usart phase is finished
    // we need to enable the txe irq and kick start the transmit process.
    //
    if (p_can->is_TX_busy == false)
    {
        // NOTE: Turn on TXE after prime transmit,
        // if turn on TXE b4 prime transmit create a
        // bug where the index = 2 char don't get
        // send.

        CAN_Prime_Transmit(p_can);
    }
}

static void CAN_Prime_Transmit(can_stdio_t* p_can)
{
    //
    // Do we have any data to transmit?
    //
    flexcan_frame_t tx_frame;
    TX_BUFFER_POP(p_can, &tx_frame);
    p_can->p_TX_mb->frame = &tx_frame;

    FLEXCAN_TransferSendNonBlocking(p_can->p_base, p_can->p_flexcan_handle, p_can->p_TX_mb);
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */