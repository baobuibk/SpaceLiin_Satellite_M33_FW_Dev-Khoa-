/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "stdint.h"
#include "bsp_temp.h"
#include "ad4114.h"
#include "spi_io.h"
#include "do.h"
#include "ntc.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define NTC_CHANNEL_NUM	12

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static  uint32_t adc0_result[16] = {0};
static  uint32_t adc1_result[16] = {0};

static  int32_t	NTC_temperature[NTC_CHANNEL_NUM] = {0};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
extern SPI_Io_t onboard_adc_spi;

extern do_t 	onboard_adc0_cs;
extern do_t 	onboard_adc1_cs;

ad4114_t onboard_adc_dev0 =
{
		.spi = &onboard_adc_spi,
		.cs  = &onboard_adc0_cs
};

ad4114_t onboard_adc_dev1 =
{
		.spi = &onboard_adc_spi,
		.cs  = &onboard_adc1_cs
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
uint32_t bsp_adc_init()
{
	uint32_t ret;
	ret =  ad4114_init(&onboard_adc_dev0, &onboard_adc_spi, &onboard_adc0_cs);
	ret +=  ad4114_init(&onboard_adc_dev1, &onboard_adc_spi, &onboard_adc1_cs);

	return ret;
}

uint32_t bsp_adc0_update()
{
	uint16_t out_mask = 0;
	uint32_t ret;
    ret = ad4114_read_all(&onboard_adc_dev0, 50000u, &out_mask, adc0_result);
    if (!ret)
    {
    	for (uint32_t i = 4; i < 16; i++)	NTC_temperature[i - 4] = ntc_convert(adc0_result[i]);
    }
    return ret;
}

uint32_t bsp_onboard_adc_update_all()
{
	uint16_t out_mask_adc0, out_mask_adc1 = 0;
	uint32_t ret;

    ret = ad4114_read_all(&onboard_adc_dev0, 50000u, &out_mask_adc0, adc0_result);

	if (ret)
	{
		return ret;
	}
	
	ret = ad4114_read_all(&onboard_adc_dev1, 50000u, &out_mask_adc1, adc1_result);

	if (ret)
	{
		return ret;
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

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

