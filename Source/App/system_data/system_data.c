/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "system_data.h"

#include "task_common.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define TEC_CHANNEL_NUM 4U
#define NTC_CHANNEL_NUM	12U
#define EFUSE_CHANNEL_NUM 9U

#define SEMAPHOR_TIMEOUT 2000

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static int16_t  NTC_temp_C[NTC_CHANNEL_NUM] = {0};
static uint16_t eFUSE_current_ma[EFUSE_CHANNEL_NUM] = {0};
static int32_t  onboard_temp_C = 0;

static exp_profile_t s_exp_profile;

static osSemaphore system_data_sem;
static osSemaphore exp_profile_sem;

static uint8_t is_start_exp = 0;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
uint32_t system_data_init()
{
    int sem_ret = osSemaphoreCreate(&system_data_sem);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }

    sem_ret = osSemaphoreCreate(&exp_profile_sem);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
    
    return (uint32_t)sem_ret;
}

uint32_t system_data_update_NTC(int16_t* p_data)
{
    int sem_ret = osSemaphoreTake(&system_data_sem, SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }

    memcpy((void*)NTC_temp_C, (void*)p_data, sizeof(NTC_temp_C));

    osSemaphoreGiven(&system_data_sem);

    return sem_ret;
}

uint32_t system_data_get_NTC(NTC_channel channel, int16_t* p_data)
{
    int sem_ret = osSemaphoreTake(&system_data_sem, SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }

    if (channel == NTC_CHANNEL_ALL)
    {
        memcpy((void*)p_data, (void*)NTC_temp_C, sizeof(NTC_temp_C));

        osSemaphoreGiven(&system_data_sem);

        return sem_ret;
    }
    
    *p_data = NTC_temp_C[channel];

    osSemaphoreGiven(&system_data_sem);

    return sem_ret;
}

uint32_t system_data_update_exp_profile(exp_profile_t* p_data)
{
    int sem_ret = osSemaphoreTake(&exp_profile_sem, SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }

    memcpy((void*)&s_exp_profile, (void*)p_data, sizeof(exp_profile_t));

    osSemaphoreGiven(&exp_profile_sem);

    return sem_ret;
}

uint32_t system_data_get_exp_profile(exp_profile_t* p_data)
{
    int sem_ret = osSemaphoreTake(&exp_profile_sem, SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
    
    *p_data = s_exp_profile;

    osSemaphoreGiven(&exp_profile_sem);

    return sem_ret;
}

uint32_t system_data_update_is_start_exp(uint8_t _is_start_exp)
{
    int sem_ret = osSemaphoreTake(&exp_profile_sem, SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
    
    is_start_exp = _is_start_exp;

    osSemaphoreGiven(&exp_profile_sem);

    return sem_ret;
}

uint32_t system_data_get_is_start_exp(uint8_t* p_buffer)
{
    int sem_ret = osSemaphoreTake(&exp_profile_sem, SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
    
    *p_buffer = is_start_exp;

    osSemaphoreGiven(&exp_profile_sem);

    return sem_ret;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
