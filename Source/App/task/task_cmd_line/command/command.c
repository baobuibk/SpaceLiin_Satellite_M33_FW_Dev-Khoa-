/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Task common includes. */
#include "task_common.h"

/* Component includes. */
#include "cmd_line.h"

/* USER include. */
#include "command.h"
#include "cmd_test_can.h"
#include "cmd_test_spi.h"

#include "system_data.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static void double_to_string(double value, char *buffer, uint8_t precision);
static void float_to_string(float value, char *buffer, uint8_t precision);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
tCmdLineEntry g_psCmdTable[] =
{
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Power Control Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "pwr_sol_en", 			CMD_PWR_SOL_EN,				" : Solenoid power control" },
	{ "pwr_htr_en", 			CMD_PWR_HTR_EN,				" : Heater power control" },
	{ "pwr_las_en", 			CMD_PWR_LAS_EN,				" : Laser power control" },
	{ "pwr_phot_en", 			CMD_PWR_PHOT_EN,			" : Photo power control" },
	{ "pwr_tec_en", 			CMD_PWR_TEC_EN,				" : Laser power control" },
	{ "pwr_pump_en", 			CMD_PWR_PUMP_EN,			" : Photo power control" },

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Heater Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "htr_set", 				CMD_HTR_SET,				" : Set heater n duty cycle" },

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Solenoid Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "sol_ctl", 				CMD_SOL_CTL,				" : Clear/Set Sol n output" },
	{ "valve_set", 				CMD_VALVE_SET,				" : Set Sol valve direction" },
	{ "flow_get", 				CMD_FLOW_GET,				" : Get flow sensor data" },
	{ "bmp_get", 				CMD_BMP_GET,				" : Get BMP sensor data" },

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Pump Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "pump_enable", 			CMD_PUMP_ENABLE,			" : Enable pump" },
	{ "pump_freq", 				CMD_PUMP_FREQ,				" : Set pump freq" },
	{ "pump_volt", 				CMD_PUMP_VOLT,				" : Set pump volt" },

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ NTC Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "adc_id", 				CMD_GET_ADC_ID,				" : Get onboard adc id" },
	{ "temp_ntc", 				CMD_GET_TEMP_NTC,			" : Get NTC temp value" },

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Test EXP Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "exp_start", 				CMD_EXP_START,				" : Start EXP sequence" },

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Test LASER Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "laser_set", 				CMD_LASER_SET,				" : Switch on/off a channel" },
	{ "laser_dac", 				CMD_LASER_DAC,				" : Set DAC output volt" },
	{ "laser_current", 			CMD_LASER_CURRENT,			" : Switch on/off a channel" },

	{ "ext_set", 				CMD_EXT_SET,				" : Switch on/off a channel" },
	{ "ext_dac", 				CMD_EXT_DAC,				" : Set DAC output volt" },
	// { "ext_current", 			CMD_EXT_CURRENT,			" : Switch on/off a channel" },

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Test PHOTO Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "photo_set", 				CMD_PHOTO_SET,				" : Switch on/off a channel" },

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ultility Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "help", 					CMD_HELP,					" : Display list of commands" },
	{ "clear",           		CMD_CLEAR_SCREEN,           " : Clear console screen" },

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Test CAN Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "can_send_demo", 			CMD_CAN_SEND_DEMO,			" : Send demo can frame" },
	{ "can_send_2byte", 		CMD_CAN_SEND_2BYTE,			" : Send 2 byte of user input" },

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Test IO Expander Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "io_set", 				CMD_IO_CTRL,				" : Turn on specific io" },
	{ "heat_set", 				CMD_HEATER_CTRL,			" : Turn on specific heater" },
	{ "read_temp", 				CMD_READ_TEMP,				" : Read onboard temp" },

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Test IO Expander Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "io_set", 				CMD_IO_CTRL,				" : Turn on specific io" },
	{ "heat_set", 				CMD_HEATER_CTRL,			" : Turn on specific heater" },
	{ "read_temp", 				CMD_READ_TEMP,				" : Read onboard temp" },

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End Of Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ 0, 0, 0 }
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* :::::::::: Power Control Command :::::::: */
int CMD_PWR_SOL_EN(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm;

	receive_argm = atoi(argv[1]);

	if ((receive_argm < 0) || (receive_argm > 1))
		return CMDLINE_INVALID_ARG;

	// bsp_expander_ctrl(POW_ONOFF_SOLENOID, receive_argm);
	bsp_expander_ctrl(POW_ONOFF_TEC, receive_argm);

	// Return success.
	return CMDLINE_OK;
}

int CMD_PWR_HTR_EN(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm;

	receive_argm = atoi(argv[1]);

	if ((receive_argm < 0) || (receive_argm > 1))
		return CMDLINE_INVALID_ARG;

	bsp_expander_ctrl(POW_ONOFF_HEATER, receive_argm);

	// Return success.
	return CMDLINE_OK;
}

int CMD_PWR_LAS_EN(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm;

	receive_argm = atoi(argv[1]);

	if ((receive_argm < 0) || (receive_argm > 1))
		return CMDLINE_INVALID_ARG;

	bsp_expander_ctrl(POW_ONOFF_LASER, receive_argm);

	// Return success.
	return CMDLINE_OK;
}

int CMD_PWR_PHOT_EN(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm;

	receive_argm = atoi(argv[1]);

	if ((receive_argm < 0) || (receive_argm > 1))
		return CMDLINE_INVALID_ARG;

	bsp_expander_ctrl(POW_ONOFF_PHOTO, receive_argm);

	// Return success.
	return CMDLINE_OK;
}

int CMD_PWR_TEC_EN(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm;

	receive_argm = atoi(argv[1]);

	if ((receive_argm < 0) || (receive_argm > 1))
		return CMDLINE_INVALID_ARG;

	bsp_expander_ctrl(POW_ONOFF_TEC, receive_argm);

	// Return success.
	return CMDLINE_OK;
}

int CMD_PWR_PUMP_EN(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm;

	receive_argm = atoi(argv[1]);

	if ((receive_argm < 0) || (receive_argm > 1))
		return CMDLINE_INVALID_ARG;

	bsp_expander_ctrl(POW_ONOFF_HD4, receive_argm);

	// Return success.
	return CMDLINE_OK;
}

/* :::::::::: Heater Command :::::::: */
int CMD_HTR_SET(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 3)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 3)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[2];

	receive_argm[0] = atoi(argv[1]);
	receive_argm[1] = atoi(argv[2]);

	if ((receive_argm[0] < 1) || (receive_argm[0] > 8))
		return CMDLINE_INVALID_ARG;
	if ((receive_argm[1] < 0) || (receive_argm[1] > 100))
		return CMDLINE_INVALID_ARG;

	uint8_t heater_map[8] =
	{
		HEATER_PWM_1, HEATER_PWM_2, HEATER_PWM_3, HEATER_PWM_4,
		HEATER_PWM_5, HEATER_PWM_6, HEATER_PWM_7, HEATER_PWM_8
	};

	bsp_heater_turnon(heater_map[receive_argm[0] - 1], receive_argm[1]);

	// Return success.
	return CMDLINE_OK;
}

/* :::::::::: Solenoid Command :::::::: */
int CMD_SOL_CTL(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 3)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 3)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[2];

	receive_argm[0] = atoi(argv[1]);
	receive_argm[1] = atoi(argv[2]);

	if ((receive_argm[0] < 1) || (receive_argm[0] > 12))
		return CMDLINE_INVALID_ARG;
	if ((receive_argm[1] < 0) || (receive_argm[1] > 1))
		return CMDLINE_INVALID_ARG;

	uint8_t sol_map[12] =
	{
		SOLENOID_CTR_1, SOLENOID_CTR_2, SOLENOID_CTR_3, SOLENOID_CTR_4,
		SOLENOID_CTR_5, SOLENOID_CTR_6, SOLENOID_CTR_7, SOLENOID_CTR_8,
		SOLENOID_CTR_9, SOLENOID_CTR_10, SOLENOID_CTR_11, SOLENOID_CTR_12
	};

	bsp_expander_ctrl(sol_map[receive_argm[0] - 1], receive_argm[1]);

	// Return success.
	return CMDLINE_OK;
}

int CMD_VALVE_SET(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm;

	receive_argm = atoi(argv[1]);

	if ((receive_argm < 1) || (receive_argm > 2))
		return CMDLINE_INVALID_ARG;

	Valve_switch(receive_argm);

	if (receive_argm == 1)
	{
		bsp_debug_console_send_string("> PIN A = 1, PIN B = 0\n> ");
	}
	else
	{
		bsp_debug_console_send_string("> PIN A = 0, PIN B = 1\n> ");
	}

	// Return success.
	return CMDLINE_OK;
}

int CMD_FLOW_GET(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;

	slf3s_readings_t flow_data;

	Flow_sensor_read(&flow_data);

	char flow_string[16];
	char temp_string[16];
	double_to_string((double)flow_data.flow, flow_string, 3);
	double_to_string((double)flow_data.temp, temp_string, 3);

	bsp_debug_console_printf("> FLOW: %s µl/min, TEMP: %s C\n> ", flow_string, temp_string);

	// Return success.
	return CMDLINE_OK;
}

int CMD_BMP_GET(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;

	bmp390_data_t bmp_data;

	BMP390_sensor_read(&bmp_data);

	char pressure_string[16] = {0};
	char temperature_string[16] = {0};

	float_to_string((bmp_data.Pressure / 100.0), pressure_string, 3);
	float_to_string(bmp_data.Temp, temperature_string, 3);
	
	bsp_debug_console_printf("> BMP P: %s hPa, T: %s C\n\r", pressure_string, temperature_string);

	// Return success.
	return CMDLINE_OK;
}

int CMD_PUMP_ENABLE(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm;

	receive_argm = atoi(argv[1]);

	if ((receive_argm < 0) || (receive_argm > 1))
		return CMDLINE_INVALID_ARG;

	I2C_HD_Pump_set_enable(receive_argm);

	// Return success.
	return CMDLINE_OK;
}

int CMD_PUMP_FREQ(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm;

	receive_argm = atoi(argv[1]);

	if ((receive_argm < 50) || (receive_argm > 800))
		return CMDLINE_INVALID_ARG;

	I2C_HD_Pump_Set_Freq(receive_argm);

	// Return success.
	return CMDLINE_OK;
}

int CMD_PUMP_VOLT(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm;

	receive_argm = atoi(argv[1]);

	if ((receive_argm < 0) || (receive_argm > 250))
		return CMDLINE_INVALID_ARG;

	I2C_HD_Pump_set_Voltage(receive_argm);

	// Return success.
	return CMDLINE_OK;
}

/* :::::::::: NTC Command :::::::: */
int CMD_GET_ADC_ID(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm = atoi(argv[1]);

	if ((receive_argm < 0) || (receive_argm > 1))
		return CMDLINE_INVALID_ARG;

	uint32_t ret = 0;
	// int32_t temp = 0;
	uint16_t id = 0;

	bsp_onboard_adc_chip_id(receive_argm, &id);

	bsp_debug_console_printf("> UPDATE RETURN: %d\n", ret);
	bsp_debug_console_printf("> CHIP ID: %x\n", id);

	// Return success.
	return CMDLINE_OK;
}

int CMD_GET_TEMP_NTC(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm;

	bsp_onboard_adc_update_raw();
	bsp_onboard_adc_update_volt();

	bsp_convert_NTC();

	if (!strcmp(argv[1], "all"))
	{
		int16_t temp_ntc[12] = {0};
		system_data_get_NTC(NTC_CHANNEL_ALL, temp_ntc);

		for (uint8_t i = 0; i < 12; i+=3)
		{
			bsp_debug_console_printf("> NTC TEMP %d: %d, NTC TEMP %d: %d, NTC TEMP %d: %d\n", i + 1, temp_ntc[i], i + 2, temp_ntc[i + 1], i + 3, temp_ntc[i + 2]);
		}
		
		// Return success.
		return CMDLINE_OK;
	}

	receive_argm = atoi(argv[1]);

	if ((receive_argm < 1) || (receive_argm > 12))
		return CMDLINE_INVALID_ARG;

	int16_t temp_ntc = 0;

	system_data_get_NTC((receive_argm - 1), &temp_ntc);

	// bsp_debug_console_printf("> CURRENT: %d mA\n", current_ma);
	bsp_debug_console_printf("> NTC TEMP: %d C\n", temp_ntc);

	// Return success.
	return CMDLINE_OK;
}

int CMD_EXP_START(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;

	// int receive_argm;

	// receive_argm = atoi(argv[1]);

	// if ((receive_argm < 0) || (receive_argm > 1))
	// 	return CMDLINE_INVALID_ARG;

	system_data_update_is_start_exp(1);

	// Return success.
	return CMDLINE_OK;
}

int CMD_LASER_SET(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 3)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 3)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[2];

	receive_argm[0] = atoi(argv[1]);
	receive_argm[1] = atoi(argv[2]);

	if ((receive_argm[1] < 0) || (receive_argm[1] > 1))
		return CMDLINE_INVALID_ARG;

	if (!strcmp(argv[1], "all"))
	{
		if (receive_argm[1] == 1)
		{
			for (uint8_t i = 1; i < 25; i++)
			{
				bsp_laser_int_sw_on(i);
			}
			
		}
		else
		{
			bsp_laser_int_all_sw_off();
		}
		
		// Return success.
		return CMDLINE_OK;
	}
	else if ((receive_argm[0] < 1) || (receive_argm[0] > 24))
	{
		return CMDLINE_INVALID_ARG;
	}
		
	bsp_laser_int_all_sw_off();
	
	if (receive_argm[1] == 1)
	{
		bsp_laser_int_sw_on(receive_argm[0]);
	}
	else
	{
		bsp_laser_int_sw_off(receive_argm[0]);
	}

	// Return success.
	return CMDLINE_OK;
}

int CMD_LASER_DAC(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm;

	receive_argm = atoi(argv[1]);

	if ((receive_argm < 0) || (receive_argm > UINT8_MAX))
	{
		return CMDLINE_INVALID_ARG;
	}
		
	bsp_laser_int_set_dac(receive_argm);

	// Return success.
	return CMDLINE_OK;
}

int CMD_LASER_CURRENT(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;

	uint16_t current = bsp_laser_int_current_adc_polling();
		
	bsp_debug_console_printf("> CURRENT: %duA\n", current);

	// Return success.
	return CMDLINE_OK;
}

int CMD_EXT_SET(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 3)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 3)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[2];

	receive_argm[0] = atoi(argv[1]);
	receive_argm[1] = atoi(argv[2]);

	if ((receive_argm[1] < 0) || (receive_argm[1] > 1))
		return CMDLINE_INVALID_ARG;

	if (!strcmp(argv[1], "all"))
	{
		if (receive_argm[1] == 1)
		{
			for (uint8_t i = 1; i < 9; i++)
			{
				bsp_laser_ext_sw_on(i);
			}
			
		}
		else
		{
			bsp_laser_ext_all_sw_off();
		}
		
		// Return success.
		return CMDLINE_OK;
	}
	else if ((receive_argm[0] < 1) || (receive_argm[0] > 8))
	{
		return CMDLINE_INVALID_ARG;
	}
		
	bsp_laser_ext_all_sw_off();
	
	if (receive_argm[1] == 1)
	{
		bsp_laser_ext_sw_on(receive_argm[0]);
	}
	else
	{
		bsp_laser_ext_sw_off(receive_argm[0]);
	}

	// Return success.
	return CMDLINE_OK;
}

int CMD_EXT_DAC(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm;

	receive_argm = atoi(argv[1]);

	if ((receive_argm < 0) || (receive_argm > UINT8_MAX))
	{
		return CMDLINE_INVALID_ARG;
	}
		
	bsp_laser_ext_set_dac(receive_argm);

	// Return success.
	return CMDLINE_OK;
}

int CMD_EXT_CURRENT(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;

	uint16_t current = bsp_laser_ext_current_adc_polling();
		
	bsp_debug_console_printf("> EXT CURRENT: %duA\n", current);

	// Return success.
	return CMDLINE_OK;
}

int CMD_PHOTO_SET(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 3)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 3)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[2];

	receive_argm[0] = atoi(argv[1]);
	receive_argm[1] = atoi(argv[2]);

	if ((receive_argm[1] < 0) || (receive_argm[1] > 1))
		return CMDLINE_INVALID_ARG;

	if (!strcmp(argv[1], "all"))
	{
		if (receive_argm[1] == 1)
		{
			for (uint8_t i = 1; i < 25; i++)
			{
				bsp_photo_int_sw_on(i);
			}
			
		}
		else
		{
			bsp_photo_int_all_sw_off();
		}
		
		// Return success.
		return CMDLINE_OK;
	}
	else if ((receive_argm[0] < 1) || (receive_argm[0] > 24))
	{
		return CMDLINE_INVALID_ARG;
	}
		
	bsp_photo_int_all_sw_off();
	
	if (receive_argm[1] == 1)
	{
		bsp_photo_int_sw_on(receive_argm[0]);
	}
	else
	{
		bsp_photo_int_sw_off(receive_argm[0]);
	}

	// Return success.
	return CMDLINE_OK;
}

/* :::::::::: Ultility Command :::::::: */
int CMD_HELP(int argc, char *argv[])
{
	tCmdLineEntry *pEntry;

	bsp_debug_console_send_string("\n> Available commands\r\n");
	bsp_debug_console_send_string("> ------------------\r\n");

	// Point at the beginning of the command table.
	pEntry = &g_psCmdTable[0];

	// Enter a loop to read each entry from the command table.  The
	// end of the table has been reached when the command name is NULL.
	while (pEntry->pcCmd)
    {
		// Print the command name and the brief description.
        bsp_debug_console_send_string("> ");
		bsp_debug_console_send_string(pEntry->pcCmd);
		bsp_debug_console_send_string(pEntry->pcHelp);
		bsp_debug_console_send_string("\r\n");

		// Advance to the next entry in the table.
		pEntry++;
	}

    bsp_debug_console_send_string("\r\n");

	// Return success.
	return CMDLINE_OK;
}

int CMD_CLEAR_SCREEN(int argc, char *argv[])
{
    bsp_debug_console_send_string("\033[2J");
	bsp_debug_console_send_string("\033[1;1H");

	return CMDLINE_CLEAR_SCREEN;
    // return CMDLINE_NO_RESPONSE;
}

int CMD_IO_CTRL(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 3)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 3)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[2];

	receive_argm[0] = atoi(argv[1]);
	receive_argm[1] = atoi(argv[2]);

	if ((receive_argm[0] < 0) || (receive_argm[1] < 0))
		return CMDLINE_INVALID_ARG;
	else if ((receive_argm[0] > 30) || (receive_argm[1] > 1))
		return CMDLINE_INVALID_ARG;

    bsp_expander_ctrl(receive_argm[0], receive_argm[1]);

	// Return success.
	return CMDLINE_OK;
}

int CMD_HEATER_CTRL(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 3)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 3)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[2];

	receive_argm[0] = atoi(argv[1]);
	receive_argm[1] = atoi(argv[2]);

	if ((receive_argm[0] < 0) || (receive_argm[1] < 0))
		return CMDLINE_INVALID_ARG;
	else if ((receive_argm[0] > 15) || (receive_argm[1] > 100))
		return CMDLINE_INVALID_ARG;

    bsp_heater_turnon(receive_argm[0], receive_argm[1]);

	// Return success.
	return CMDLINE_OK;
}

int CMD_READ_TEMP(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;

	int16_t temp_ic = 0;
	system_data_get_NTC(NTC_CHANNEL_1, &temp_ic);

	// bsp_debug_console_printf("> CURRENT: %d mA\n", current_ma);
	bsp_debug_console_printf("> NTC TEMP: %d C\n", temp_ic);

	// Return success.
	return CMDLINE_OK;
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static void double_to_string(double value, char *buffer, uint8_t precision)
{
    /* Sign */
    if (value < 0.0)
    {
        *buffer++ = '-';
        value = -value;
    }

    /* Split into integer and fractional parts */
    uint32_t integer_part  = (uint32_t)value;          /* NOTE: clips to 32-bit range */
    double   fractional_part = value - (double)integer_part;
    if (fractional_part < 0.0) fractional_part = 0.0;  /* guard tiny FP negatives */

    /* Integer -> string (in place, no sprintf) */
    if (integer_part == 0U)
    {
        *buffer++ = '0';
    }
    else
    {
        char rev[10];  /* max 10 digits for uint32_t */
        uint8_t n = 0;
        while (integer_part != 0U)
        {
            rev[n++] = (char)('0' + (integer_part % 10U));
            integer_part /= 10U;
        }
        while (n--)
        {
            *buffer++ = rev[n];
        }
    }

    /* Fractional part (truncate, no rounding) */
    if (precision > 0U)
    {
        *buffer++ = '.';
        for (uint8_t i = 0; i < precision; i++)
        {
            fractional_part *= 10.0;
            uint8_t digit = (uint8_t)fractional_part;

            /* Clamp against rare FP edge (e.g., 9.999999 -> 10) */
            if (digit > 9U) digit = 9U;

            *buffer++ = (char)('0' + digit);
            fractional_part -= (double)digit;
            if (fractional_part < 0.0) fractional_part = 0.0; /* guard drift */
        }
    }

    *buffer = '\0';
}

static void float_to_string(float value, char *buffer, uint8_t precision)
{
    /* Sign */
    if (value < 0.0)
    {
        *buffer++ = '-';
        value = -value;
    }

    /* Split into integer and fractional parts */
    uint32_t integer_part  = (uint32_t)value;          /* NOTE: clips to 32-bit range */
    float   fractional_part = value - (float)integer_part;
    if (fractional_part < 0.0) fractional_part = 0.0;  /* guard tiny FP negatives */

    /* Integer -> string (in place, no sprintf) */
    if (integer_part == 0U)
    {
        *buffer++ = '0';
    }
    else
    {
        char rev[10];  /* max 10 digits for uint32_t */
        uint8_t n = 0;
        while (integer_part != 0U)
        {
            rev[n++] = (char)('0' + (integer_part % 10U));
            integer_part /= 10U;
        }
        while (n--)
        {
            *buffer++ = rev[n];
        }
    }

    /* Fractional part (truncate, no rounding) */
    if (precision > 0U)
    {
        *buffer++ = '.';
        for (uint8_t i = 0; i < precision; i++)
        {
            fractional_part *= 10.0;
            uint8_t digit = (uint8_t)fractional_part;

            /* Clamp against rare FP edge (e.g., 9.999999 -> 10) */
            if (digit > 9U) digit = 9U;

            *buffer++ = (char)('0' + digit);
            fractional_part -= (float)digit;
            if (fractional_part < 0.0) fractional_part = 0.0; /* guard drift */
        }
    }

    *buffer = '\0';
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
