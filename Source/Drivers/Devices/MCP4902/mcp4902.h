#ifndef MCP4902_H_
#define MCP4902_H_

#include <stdint.h>
#include "spi_io.h"
#include "do.h"

/* Vref (mV) */
#define MCP4902_VREF_MV          3300u  /* giống _VREF_DAC cũ */

/* Kênh */
#define MCP4902_NUM_CHANNEL      2u
#define MCP4902_CHA              0u
#define MCP4902_CHB              1u

/* Bit field trong khung 16-bit của MCP4902 (giữ như mã gốc) */
#define MCP4902_AB_BIT           15u   /* 1 = kênh B, 0 = kênh A */
#define MCP4902_BUF_BIT          14u   /* không dùng/để 0 theo board hiện tại */
#define MCP4902_GA_BIT           13u   /* 1 = Gain 1x, 0 = 2x */
#define MCP4902_SHDN_BIT         12u   /* 1 = bật (active), 0 = shutdown */

/* Thiết bị */
typedef struct {
    SPI_Io_t   *spi;                         /* bus SPI trừu tượng */
    do_t        cs;                          /* CS (digital output) */
    do_t        latch;                       /* LATCH (digital output) */
    uint8_t     dac_channel[MCP4902_NUM_CHANNEL]; /* cache giá trị DAC 8-bit cho A/B */
} MCP4902_Device_t;

/* --- API --- */
uint8_t  MCP4902_VoltageToCode(uint16_t mv);     /* mV -> 8-bit DAC */
uint16_t MCP4902_CodeToVoltage(uint8_t code);    /* 8-bit DAC -> mV   */

int MCP4902_Device_Init(MCP4902_Device_t *dev,
                        SPI_Io_t *spi,
                        const do_t *cs,
                        const do_t *latch);

int MCP4902_Shutdown(MCP4902_Device_t *dev, uint8_t channel);         /* tắt riêng kênh */
int MCP4902_Set_DAC  (MCP4902_Device_t *dev, uint8_t channel, uint8_t code);
int MCP4902_Set_Voltage(MCP4902_Device_t *dev, uint8_t channel, uint16_t mv);

/* Ghi cả hai kênh từ cache (nếu bạn muốn chủ động flush) */
int MCP4902_Flush(MCP4902_Device_t *dev);

#endif /* MCP4902_H_ */
