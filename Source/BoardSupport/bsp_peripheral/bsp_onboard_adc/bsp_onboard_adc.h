#ifndef _BSP_ONBOARD_ADC_H_
#define _BSP_ONBOARD_ADC_H_

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Include ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "stdint.h"

#include "fsl_flexcan.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
typedef enum
{
    TEC_CHANNEL_1,
    TEC_CHANNEL_3,
    TEC_CHANNEL_4,
    TEC_CHANNEL_2,
    NTC_CHANNEL_12,
    NTC_CHANNEL_10,
    NTC_CHANNEL_5,
    NTC_CHANNEL_4,
    NTC_CHANNEL_1,
    NTC_CHANNEL_8,
    NTC_CHANNEL_2,
    NTC_CHANNEL_3,
    NTC_CHANNEL_7,
    NTC_CHANNEL_6,
    NTC_CHANNEL_11,
    NTC_CHANNEL_9,
    ONBOARD_ADC1_0,
    ONBOARD_ADC1_1,
    EFUSE_12V_PHOTO,
    EFUSE_5V_CAM,
    TEMP_SENSOR,
    EFUSE_12V_LASER,
    EFUSE_5V_HD4,
    EFUSE_12V_HEADER,
    EFUSE_12V, //EFUSE_12V_IN
    EFUSE_5V_TEC,
    EFUSE_5V_IO,
    EFUSE_12V_SOLENOID,
} Onboard_ADC_Channel;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
uint32_t bsp_onboard_adc_init();

uint32_t bsp_onboard_adc_update_all();

void bsp_get_TEC(Onboard_ADC_Channel TEC_Index);

int16_t bsp_get_NTC(Onboard_ADC_Channel NTC_Index);

void bsp_get_eFUSE_ILIM(Onboard_ADC_Channel eFUSE_Index);

int32_t bsp_get_temp();

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* _BSP_ONBOARD_ADC_H_ */