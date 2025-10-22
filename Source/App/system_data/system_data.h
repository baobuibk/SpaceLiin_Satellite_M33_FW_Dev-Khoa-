#ifndef _SYSTEM_DATA_H_
#define _SYSTEM_DATA_H_

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Include ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "stdint.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
typedef enum
{
    NTC_CHANNEL_1,
    NTC_CHANNEL_2,
    NTC_CHANNEL_3,
    NTC_CHANNEL_4,
    NTC_CHANNEL_5,
    NTC_CHANNEL_6,
    NTC_CHANNEL_7,
    NTC_CHANNEL_8,
    NTC_CHANNEL_9,
    NTC_CHANNEL_10,
    NTC_CHANNEL_11,
    NTC_CHANNEL_12,
    NTC_CHANNEL_ALL,
} NTC_channel;

// typedef enum
// {
//     TEC_CHANNEL_1,
//     TEC_CHANNEL_3,
//     TEC_CHANNEL_4,
//     TEC_CHANNEL_2,
//     NTC_CHANNEL_12,
//     NTC_CHANNEL_10,
//     NTC_CHANNEL_5,
//     NTC_CHANNEL_4,
//     NTC_CHANNEL_1,
//     NTC_CHANNEL_8,
//     NTC_CHANNEL_2,
//     NTC_CHANNEL_3,
//     NTC_CHANNEL_7,
//     NTC_CHANNEL_6,
//     NTC_CHANNEL_11,
//     NTC_CHANNEL_9,
// } Onboard_ADC0_Channel;

// typedef enum
// {
//     ONBOARD_ADC1_0,
//     ONBOARD_ADC1_1,
//     EFUSE_12V_PHOTO,
//     EFUSE_5V_CAM,
//     TEMP_SENSOR,
//     EFUSE_12V_LASER,
//     EFUSE_5V_HD4,
//     EFUSE_12V_HEADER,
//     EFUSE_12V, //EFUSE_12V_IN
//     EFUSE_5V_TEC,
//     EFUSE_5V_IO,
//     EFUSE_12V_SOLENOID,
// } Onboard_ADC1_Channel;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
uint32_t system_data_init();

uint32_t system_data_update_NTC(int16_t* p_data);

uint32_t system_data_get_NTC(NTC_channel channel, int16_t* p_data);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* _SYSTEM_DATA_H_ */