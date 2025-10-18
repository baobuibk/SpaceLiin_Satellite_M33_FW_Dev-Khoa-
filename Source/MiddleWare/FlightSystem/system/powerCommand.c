/*
 * powerCommand.c
 *
 *  Created on: Oct 10, 2025
 *      Author: Admin
 */
#include "powerCommand.h"
#include "printf.h"
#include "error_codes.h"
#include "log_utils.h"
#include "storage.h"
#include "repoCommand.h"
#include "repoData.h"

static const char *tag = "power";


//cmd_result_t cmd_set_logger(int argc, char *argv[])
//{
//
//}


void  power_heater_control(uint32_t argc, uint8_t *argv[], cmd_result_t * result)
{
    value32_t v;
    result->error = CMDLINE_OK;
    if((argc < 2) )
    {
        result->error =  CMDLINE_TOO_FEW_ARGS;
        return;
    }
    if((argc > 2) )
    {
        result->error =  CMDLINE_TOO_MANY_ARGS;
        return;
    }

    if (!strcmp((char *)argv[1],"on"))
		{
    	//power on heater
    	v.i = POWER_ON;
    	memcpy(result->output, "heater power on", sizeof("heater power on"));
    	result->outputSize = sizeof("heater power on");
    	LOGI(tag,"heater power on");
		}
    else if (!strcmp((char *)argv[1],"off"))
		{
    	//power off heater
    	v.i = POWER_OFF;
    	memcpy(result->output, "heater power off", sizeof("heater power off"));
    	result->outputSize = sizeof("heater power off");
    	LOGI(tag,"heater power off");
		}
    else { //wrong argument
    	LOGI(tag,"wrong argument");
    	result->error = CMDLINE_INVALID_ARG;
    }
    dat_set_status_var(data_exp_12V_heater_status,v);
    return;

}
void power_heater_status(uint32_t argc, uint8_t *argv[], cmd_result_t * result)
{
	value32_t v;
    if((argc != 1) )
    {
    	result->error =  CMDLINE_TOO_MANY_ARGS;
    	return;
    }
    v = dat_get_status_var(data_exp_12V_heater_status);
    if (POWER_OFF == v.i ) {

    	memcpy(result->output, "heater power off", sizeof("heater power off"));
    	result->outputSize = sizeof("heater power off");
    	LOGI(tag,"12V_heater off");
    }
    else
    	{
    		memcpy(result->output, "heater power on", sizeof("heater power on"));
    		result->outputSize = sizeof("heater power on");
    		LOGI(tag,"12V_heater on");
    	}
    return ;

}
