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
static  uint32_t adc0_result[16] = {0};
static  uint32_t adc1_result[16] = {0};

static  int16_t  NTC_temperature[NTC_CHANNEL_NUM] = {0};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static uint32_t bsp_onboard_adc_config(ad4114_t* dev);

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
	// ret = ad4114_init(&onboard_adc_dev0, &onboard_adc_spi, &onboard_adc0_cs);

	// if (ret != ERROR_OK)
	// {
	// 	return ret;
	// }

	ret = ad4114_init(&onboard_adc_dev1, &onboard_adc_spi, &onboard_adc1_cs);

	if (ret != ERROR_OK)
	{
		return ret;
	}

	ret = bsp_onboard_adc_config(&onboard_adc_dev1);

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

    // ret = ad4114_read_all(&onboard_adc_dev0, 50000u, &out_mask_adc0, adc0_result);

	// if (ret != ERROR_OK)
	// {
	// 	return ret;
	// }
	
	ret = ad4114_read_all(&onboard_adc_dev1, 50000u, &out_mask_adc1, adc1_result);

	if (ret != ERROR_OK)
	{
		return ret;
	}

    return ERROR_OK;
}

void bsp_get_TEC(Onboard_ADC_Channel TEC_Index)
{
	if ((TEC_Index < TEC_CHANNEL_1) || (TEC_Index > TEC_CHANNEL_2))
	{
		return;
	}
}

int16_t bsp_get_NTC(Onboard_ADC_Channel NTC_Index)
{
	if ((NTC_Index < NTC_CHANNEL_12) || (NTC_Index > NTC_CHANNEL_9))
	{
		return ERROR_INVALID_PARAM;
	}
	
	return NTC_temperature[NTC_Index] = ntc_convert(adc0_result[NTC_Index]);
}

void bsp_get_eFUSE_ILIM(Onboard_ADC_Channel eFUSE_Index)
{
	if ((eFUSE_Index < EFUSE_12V_PHOTO) || (eFUSE_Index == TEMP_SENSOR) || (eFUSE_Index > EFUSE_12V_SOLENOID))
	{
		return;
	}
}

/* Tính VIN từ code 24-bit, GAINx, OFFSETx theo datasheet.
 * - raw24  : giá trị 24-bit từ thanh ghi DATA (0..0xFFFFFF)
 * - gain24 : giá trị 24-bit từ GAINx (0x000000..0xFFFFFF), chuẩn hóa theo 0x400000
 * - offset24: giá trị 24-bit từ OFFSETx (thường mặc định 0x800000)
 * - bipolar: true = bipolar (offset-binary), false = unipolar (straight-binary)
 * - vref   : điện áp tham chiếu (Volt)
 * - vin_out: kết quả (Volt)
 */
uint32_t ad4114_data_to_vin(uint32_t raw24,
                            uint32_t gain24,
                            uint32_t offset24,
                            bool     bipolar,
                            float    vref,
                            float   *vin_out)
{
    if (!vin_out || vref <= 0.0f) return (uint32_t)ERROR_INVALID_PARAM;

    /* chặn 24-bit, tránh tràn */
    raw24    &= 0xFFFFFFu;
    gain24   &= 0xFFFFFFu;
    offset24 &= 0xFFFFFFu;

    /* Hằng số theo datasheet */
    const float K_ATT   = 0.075f;           /* hệ số suy giảm nội trước hiệu chuẩn */
    const float TWO23   = 8388608.0f;       /* 2^23 */
    const float G_NORM  = 4194304.0f;       /* 0x400000 = 2^22 */

    /* Chuẩn hoá gain */
    float G = (gain24 > 0u) ? ((float)gain24 / G_NORM) : 1.0f;

    float term_off = ((float)((int32_t)(offset24) - (int32_t)0x800000)) / TWO23;
    float vin;

    if (!bipolar) {
        /* Unipolar: VIN = VREF/0.075 * [ Data/(G*2*2^23) + (OFFSET-0x800000)/2^23 ] */
        float term_data = ((float)raw24) / (G * 2.0f * TWO23);
        vin = (vref / K_ATT) * (term_data + term_off);
    } else {
        /* Bipolar: VIN = VREF/0.075 * [ (Data-0x800000)/(G*2^23) + (OFFSET-0x800000)/2^23 ] */
        float term_data = ((float)((int32_t)raw24 - (int32_t)0x800000)) / (G * TWO23);
        vin = (vref / K_ATT) * (term_data + term_off);
    }

    *vin_out = vin;
    return (uint32_t)ERROR_OK;
}

int32_t bsp_get_temp()
{
    /* 24-bit unipolar ADC, Vref = 1.8 V (1800 mV) */
    const uint32_t FS       = (1u << 24) - 1u;   /* 16,777,215 */
    const int32_t  VREF_mV  = 1800;
	const uint32_t ADC_CODE = adc1_result[0];
	float vin = 0.0;

	ad4114_data_to_vin(adc1_result[0], 0x400000, 0x800000, false, 1800.0, &vin);

	// bsp_debug_console_printf("> GET_TEMP_VIN %d\n", (uint16_t)vin);

    // /* Valid sensor output range: 0.10 … 1.75 V  ⇒ −40 … 125 °C */
    // const uint32_t CODE_MIN = (uint32_t)(( (uint64_t)FS * 100u ) / 1800u);  /* ~ FS * 0.1/1.8 */
    // const uint32_t CODE_MAX = (uint32_t)(( (uint64_t)FS * 1750u) / 1800u);  /* ~ FS * 1.75/1.8 */

    // if (ADC_CODE <= CODE_MIN)
	// {
	// 	return -400;  /* -40.0 °C */
	// } 

    // if (ADC_CODE >= CODE_MAX)
	// {
	// 	return  1250; /* 125.0 °C */
	// } 

    /* T_dC = (Vref_mV * code / FS) - 500, with rounding */
    // int32_t t_dC = (int32_t)(((uint64_t)VREF_mV * ADC_CODE + (FS/2)) / FS) - 500;
	int32_t t_dC = (uint16_t)vin - 500;

    return t_dC;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Use this once right after ad4114_init() for each ADC.
static uint32_t bsp_onboard_adc_config(ad4114_t* dev)
{
    if (!dev)
	{
		return (uint32_t)ERROR_INVALID_PARAM;
	}

    uint32_t rc;

	uint32_t temp = 0x400000;
	ad4114_gain_write(dev, 0, temp);

	ad4114_gain_read(dev, 0, &temp);

	// bsp_debug_console_printf("GAIN 0x%x%x\n", temp >> 16, temp);

    // 1) SETUPx:
    uint16_t setupcon = 0 | AD4114_SETUP_BI_UNIPOLAR(0) | AD4114_SETUP_REFBUF_PN(3) | AD4114_SETUP_INBUF(3) | AD4114_SETUP_REF_SEL(0);
    rc = ad4114_setup_write(dev, 0, setupcon);
    if (rc) return rc;

    // 2) Map & enable a channel to that setup
    rc = ad4114_config_channel(dev, 0, true, 0x90, 0);
    if (rc) return rc;

    // (Optional) Disable all other channels so the sequencer only hits 'ch'

    // 4) Continuous conversions, REF_EN = 0 (because external reference)
    rc = ad4114_set_mode_continuous(dev, /*ref_en=*/false);
    if (rc) return rc;

    return (uint32_t)ERROR_OK;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */