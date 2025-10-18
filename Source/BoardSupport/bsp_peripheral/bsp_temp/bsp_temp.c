/*
 * bsp_temp.c
 *
 *  Created on: Oct 15, 2025
 *      Author: Admin
 */
#include "stdint.h"
#include "bsp_temp.h"
#include "ad4114.h"
#include "spi_io.h"
#include "do.h"
#include "ntc.h"

extern SPI_Io_t onboard_adc_spi;

extern do_t 	onboard_adc0_cs;
extern do_t 	onboard_adc1_cs;

ad4114_t onboard_adc_dev0 = {
		.spi = &onboard_adc_spi,
		.cs  = &onboard_adc0_cs
};
ad4114_t onboard_adc_dev1 = {
		.spi = &onboard_adc_spi,
		.cs  = &onboard_adc1_cs
};
#define NTC_CHANNEL_NUM	8

static  uint32_t adc0Result[16] = {0};
static  uint32_t adc1Result[16] = {0};


int32_t	NTC_temperature[NTC_CHANNEL_NUM] = {0};


uint32_t bsp_adc_init()
{
	uint32_t ret;
	ret =  ad4114_init(&onboard_adc_dev0, &onboard_adc_spi, &onboard_adc0_cs);
	ret +=  ad4114_init(&onboard_adc_dev1, &onboard_adc_spi, &onboard_adc0_cs);
}

uint32_t bsp_adc0_update()
{
	uint16_t out_mask = 0;
	uint32_t ret;
    ret = ad4114_read_all(&onboard_adc_dev0, 50000u, &out_mask, adc0Result);
    if (!ret)
    {
    	for (uint32_t i = 4; i < 16; i++)	NTC_temperature[i - 4] = ntc_convert(adc0Result[i]);
    }
    return ret;
}
/*
 * get value from system variable
 */
int32_t bsp_temp_get(uint32_t NTCIndex)
{
	return NTC_temperature[NTCIndex];
}
