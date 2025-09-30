#ifndef _BSP_BOARD_H_
#define _BSP_BOARD_H_

/*********************DEBUG DB9**********************/
#define DEBUG_DB9_LPUART_CLOCK_ROOT     kCLOCK_Root_Lpuart2
#define DEBUG_DB9_LPUART_CLOCK_GATE     kCLOCK_Lpuart2
#define DEBUG_DB9_LPUART_CLK_FREQ       CLOCK_GetIpFreq(DEBUG_DB9_LPUART_CLOCK_ROOT)

#define DEBUG_DB9_LPUART_HANDLE         LPUART2
#define DEBUG_DB9_LPUART_IRQn           LPUART2_IRQn

#define DEBUG_DB9_LPUART_BAUDRATE       115200

/*********************LIBCSP CAN**********************/
#define LIBCSP_CAN_CLOCK_ROOT   kCLOCK_Root_Can2
#define LIBCSP_CAN_CLOCK_GATE   kCLOCK_Can2
#define LIBCSP_CAN_CLK_FREQ     CLOCK_GetIpFreq(LIBCSP_CAN_CLOCK_ROOT)

#define LIBCSP_CAN_HANDLE       CAN2
#define LIBCSP_CAN_IRQn         CAN2_IRQn

#define LIBCSP_CAN_BAUDRATE     115200

#endif /* _BSP_BOARD_H_ */
