#ifndef _BSP_BOARD_H_
#define _BSP_BOARD_H_

/*********************DEBUG DB9**********************/
#define DEBUG_DB9_LPUART_CLOCK_ROOT     kCLOCK_Root_Lpuart2
#define DEBUG_DB9_LPUART_CLOCK_GATE     kCLOCK_Lpuart2
#define DEBUG_DB9_LPUART_CLK_FREQ       CLOCK_GetIpFreq(DEBUG_DB9_LPUART_CLOCK_ROOT)

#define DEBUG_DB9_LPUART_BASE           LPUART2
#define DEBUG_DB9_LPUART_IRQn           LPUART2_IRQn

#define DEBUG_DB9_LPUART_BAUDRATE       115200

/*********************LIBCSP CAN**********************/
#define LIBCSP_CAN_CLOCK_ROOT           kCLOCK_Root_Can2
#define LIBCSP_CAN_CLOCK_GATE           kCLOCK_Can2
#define LIBCSP_CAN_CLK_FREQ             CLOCK_GetIpFreq(LIBCSP_CAN_CLOCK_ROOT)

#define LIBCSP_CAN_BASE                 CAN2
#define LIBCSP_CAN_IRQn                 CAN2_IRQn

#define LIBCSP_CAN_BITRATE_BPS          (500000u)

/*********************TEC SPI**********************/
#define TEC_SPI_CLOCK_ROOT              kCLOCK_Root_Lpspi6
#define TEC_SPI_CLOCK_GATE              kCLOCK_Lpspi6
#define TEC_SPI_CLK_FREQ                CLOCK_GetIpFreq(TEC_SPI_CLOCK_ROOT)

#define TEC_SPI_BASE                    LPSPI6
#define TEC_SPI_IRQn                    LPSPI6_IRQn

#define TEC_SPI_BAUDRATE                (500000u)

#define TEC_SPI_GPIO_CS_CLOCK_ROOT      kCLOCK_Root_BusWakeup
#define TEC_SPI_GPIO_CS_CLOCK_GATE      kCLOCK_Gpio2
#define TEC_SPI_GPIO_CS_CLK_FREQ        CLOCK_GetIpFreq(TEC_SPI_GPIO_CS_CLOCK_ROOT)

#define TEC_SPI_GPIO_CS_PORT            GPIO2
#define TEC_SPI_GPIO_CS_PIN             0U

/*********************I/O EXPANDER I2C**********************/
#define IO_EXPAN_CLOCK_ROOT              kCLOCK_Root_Lpi2c6
#define IO_EXPAN_CLOCK_GATE              kCLOCK_Lpi2c6
#define IO_EXPAN_CLK_FREQ                CLOCK_GetIpFreq(IO_EXPAN_CLOCK_ROOT)

#define IO_EXPAN_BASE                    LPI2C6
#define IO_EXPAN_IRQn                    LPI2C6_IRQn

#define IO_EXPAN_BAUDRATE_HZ             (100000U)

#endif /* _BSP_BOARD_H_ */
