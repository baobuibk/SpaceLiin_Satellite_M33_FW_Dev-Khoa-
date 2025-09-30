#ifndef _CAN_STDIO_H_
#define _CAN_STDIO_H_

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Include ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Freescale includes. */
#include "fsl_common.h"
#include "fsl_flexcan.h"

/* User depency includes. */
#include "ring_buffer.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// typedef enum
// {
//     CAN_STD   = 0,   /* 11-bit ID */
//     CAN_EXT   = 1    /* 29-bit ID */
// } can_id_type_t;

// typedef struct
// {
//     uint32_t        id;         /* raw ID value (11 or 29 bits). Lower bits significant */
//     can_id_type_t   type;       /* standard or extended */
//     uint8_t         rtr;        /* 1 = remote frame request */
//     uint8_t         len;        /* 0..8 data length in bytes */
//     uint8_t         data[8];
//     uint16_t        timestamp;  /* optional: HW timestamp ticks (filled on RX if enabled) */
// } can_stdio_frame_t;

// typedef struct
// {
//     uint32_t        id;     /* filter ID */
//     uint32_t        mask;   /* filter mask */
//     can_id_type_t   type;   /* standard or extended */
//     uint8_t         rtr;    /* 0 = data frames only, 1 = allow RTR (optional) */
// } can_stdio_filter_t;

typedef struct _can_stdio_t_
{
    /* Hardware handles (provided by core init code) */
    CAN_Type                *handle;
    IRQn_Type               irqn;

    /* Which resources can_stdio owns/uses */
    flexcan_handle_t*       p_flexcan_handle;
    flexcan_mb_transfer_t*  p_TX_mb;    /* a dedicated TX MB index */
    flexcan_mb_transfer_t*  p_RX_mb;    /* a dedicated RX MB index */

    /* Software queues (frames, not bytes) */
    ring_buffer_t*          p_TX_buffer;/* element size = sizeof(can_stdio_frame_t) */
    ring_buffer_t*          p_RX_buffer;/* element size = sizeof(can_stdio_frame_t) */

    /* Status variable */
    bool                    is_TX_busy;
    bool                    is_RX_busy;
} can_stdio_t;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* One-time attach: can_stdio does NOT set bit timing or pins. Do that in core_*. */
void CAN_stdio_Init( 
                can_stdio_t *p_can,
                CAN_Type*   _handle,
                IRQn_Type   _irqn,
                flexcan_handle_t*      _p_flexcan_handle,
                flexcan_mb_transfer_t* _p_TX_mb,
                flexcan_mb_transfer_t* _p_RX_mb,
                ring_buffer_t *_p_TX_buffer,
                ring_buffer_t *_p_RX_buffer);

void CAN_Send_Frame(can_stdio_t* p_can, const flexcan_frame_t* p_frame);

void CAN_Send_Buffer(can_stdio_t *p_can, const flexcan_frame_t* p_buff, uint32_t frame_count);

void CAN_Get_Frame(can_stdio_t* p_can, const flexcan_frame_t* p_return_frame);

void CAN_stdio_TX_idle_subhandle(can_stdio_t* p_can);

void CAN_stdio_RX_idle_subhandle(can_stdio_t* p_can);

#endif /* _CAN_STDIO_H_ */
