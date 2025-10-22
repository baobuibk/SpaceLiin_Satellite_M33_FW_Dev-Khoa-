/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "stdint.h"

#include "bsp_onboard_adc.h"
#include "bsp_debug_console.h"

#include "spi_io.h"
#include "do.h"
#include "ntc.h"

#include "ad4114.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define TEC_CHANNEL_NUM 4U
#define NTC_CHANNEL_NUM	12U
#define EFUSE_CHANNEL_NUM 9U

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static  uint32_t adc0_raw[16] = {0};
static  uint32_t adc1_raw[16] = {0};

static  int16_t  NTC_temperature[NTC_CHANNEL_NUM] = {0};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static uint32_t bsp_onboard_adc_config(ad4114_t* p_dev, uint16_t enable_mask);
static uint32_t bsp_onboard_adc_get_vin_mv(uint32_t raw24, float vref_mv, float* vin_out_mv);

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
uint32_t bsp_onboard_adc_init()
{
	uint32_t ret;
	ret = ad4114_init(&onboard_adc_dev0, &onboard_adc_spi, &onboard_adc0_cs);

	if (ret != ERROR_OK)
	{
		return ret;
	}

	// 0xFFFF Enable 16 pin
	ret = bsp_onboard_adc_config(&onboard_adc_dev0, 0xFFFF);

	if (ret != ERROR_OK)
	{
		return ret;
	}

	ret = ad4114_init(&onboard_adc_dev1, &onboard_adc_spi, &onboard_adc1_cs);

	if (ret != ERROR_OK)
	{
		return ret;
	}

	// ((1 << 10) - 1) << 2: Enable 10 pin, start at pin 2
	ret = bsp_onboard_adc_config(&onboard_adc_dev1, (((1 << 10) - 1) << 2));

	if (ret != ERROR_OK)
	{
		return ret;
	}

    return ERROR_OK;
}

uint32_t bsp_onboard_adc_update_all()
{
	uint16_t out_mask_adc0, out_mask_adc1 = 0;
	uint32_t ret;

    ret = ad4114_read_all(&onboard_adc_dev0, 5000u, &out_mask_adc0, adc0_raw);

	if (ret != ERROR_OK)
	{
		return ret;
	}
	
	ret = ad4114_read_all(&onboard_adc_dev1, 5000u, &out_mask_adc1, adc1_raw);

	if (ret != ERROR_OK)
	{
		return ret;
	}

    return ERROR_OK;
}

void bsp_get_TEC(Onboard_ADC0_Channel TEC_Index)
{
	if ((TEC_Index < TEC_CHANNEL_1) || (TEC_Index > TEC_CHANNEL_2))
	{
		return;
	}
}

int16_t bsp_get_NTC(Onboard_ADC0_Channel NTC_Index)
{
	if ((NTC_Index < NTC_CHANNEL_12) || (NTC_Index > NTC_CHANNEL_9))
	{
		return ERROR_INVALID_PARAM;
	}

	float NTC_V = 0.0;
	bsp_onboard_adc_get_vin_mv(adc0_raw[NTC_Index], 5000.0, &NTC_V);
	
	// return NTC_temperature[NTC_Index] = ntc_convert_from_adc(adc0_raw[NTC_Index]);
	return NTC_temperature[NTC_Index] = ntc_convert_from_volt(NTC_V, 5000.0);
}

uint16_t bsp_get_eFUSE_Current(Onboard_ADC1_Channel eFUSE_Index)
{
	if ((eFUSE_Index < EFUSE_12V_PHOTO) || (eFUSE_Index == TEMP_SENSOR) || (eFUSE_Index > EFUSE_12V_SOLENOID))
	{
		return ERROR_INVALID_PARAM;
	}

	float efuse_volt_mv = 0.0;

	bsp_onboard_adc_get_vin_mv(adc1_raw[eFUSE_Index], 1800.0, &efuse_volt_mv);

	float efuse_current_ma = efuse_volt_mv * 3.343;

	return (uint16_t)efuse_current_ma;
}

int32_t bsp_get_temp()
{
	float vin = 0.0;

	bsp_onboard_adc_get_vin_mv(adc1_raw[TEMP_SENSOR], 1800.0, &vin);

	int32_t t_dC = (uint16_t)vin - 500;

    return t_dC;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Use this once right after ad4114_init() for each ADC.
static uint32_t bsp_onboard_adc_config(ad4114_t* p_dev, uint16_t enable_mask)
{
    if (!p_dev)
	{
		return (uint32_t)ERROR_INVALID_PARAM;
	}

    uint32_t rc;

	// 1) Disable all channel
	ad4114_channels_disable_mask(p_dev, 0xFFFF);

    // 2) SETUPx:
    uint16_t setupcon = 0 | AD4114_SETUP_BI_UNIPOLAR(0) | AD4114_SETUP_REFBUF_PN(3) | AD4114_SETUP_INBUF(3) | AD4114_SETUP_REF_SEL(0);
    rc = ad4114_setup_write(p_dev, 0, setupcon);
    if (rc) return rc;

	// 3) GAINx:
	ad4114_gain_write(p_dev, 0, (uint32_t)0x400000);
	if (rc) return rc;

	// 4) FILTERx:

    // 5) Link channel, pin map to the specific setup
	for (uint8_t i = 0; i < 16; i++)
	{
		// (i << 5): pin i as positive
		// (1 << 4): VINCOM as negative
		uint16_t input_map = (1 << 4) | (i << 5);
		rc = ad4114_config_channel(p_dev, i, false, input_map, 0);
    	if (rc) return rc;
	}

    // 6) Continuous conversions, REF_EN = 0 (because external reference)
    rc = ad4114_set_mode_continuous(p_dev, /*ref_en=*/false);
    if (rc) return rc;

	// 7) Enable channel follow enable mask
	ad4114_channels_enable_mask(p_dev, enable_mask);
	if (rc) return rc;

    return (uint32_t)ERROR_OK;
}

// Simplify version of it to make thing easier to calculate, this only use
// for channel that has the same setup
static uint32_t bsp_onboard_adc_get_vin_mv(uint32_t raw24, float vref_mv, float* vin_out_mv)
{
	if (!vin_out_mv || vref_mv <= 0.0f)
	{
		return (uint32_t)ERROR_INVALID_PARAM;
	}

    /* chặn 24-bit, tránh tràn */
    raw24    &= 0xFFFFFFu;

    /* Hằng số theo datasheet */
    const float K_ATT   = 0.075f;           /* hệ số suy giảm nội trước hiệu chuẩn */
    // const float TWO23   = 8388608.0f;       /* 2^23 */
	const float TWO24   = 16777216.0f;       /* 2^24 = TWO23 * 2.0 */
    // const float G_NORM  = 4194304.0f;       /* 0x400000 = 2^22 */

    /* Chuẩn hoá gain */
	/* Do gain = 0x400000 nên cho G = 1 */
    // float G = 1.0;

	// term_off = 0 do offset = 0x800000
    // float term_off = 0.0;
    float vin;
	
	// This board í uinipolar, if yours not change accordingly
	/* Unipolar: VIN = vref_mv/0.075 * [ Data/(G*2*2^23) + (OFFSET-0x800000)/2^23 ] */
	// float term_data = ((float)raw24) / (G * 2.0f * TWO23);
	// vin = (vref_mv / K_ATT) * (term_data + term_off);

	float term_data = ((float)raw24) / (TWO24);
	vin = (vref_mv / K_ATT) * (term_data);

    *vin_out_mv = vin;
    return (uint32_t)ERROR_OK;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */