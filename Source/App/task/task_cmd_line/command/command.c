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
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
tCmdLineEntry g_psCmdTable[] =
{
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Power Control Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "pwr_sol_en", 			CMD_PWR_SOL_EN,				" : Solenoid power control" },
	{ "pwr_htr_en", 			CMD_PWR_HTR_EN,				" : Heater power control" },
	{ "pwr_las_en", 			CMD_PWR_LAS_EN,				" : Heater power control" },

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Heater Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "htr_set", 				CMD_HTR_SET,				" : Set heater n duty cycle" },

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Solenoid Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "sol_ctl", 				CMD_SOL_CTL,				" : Clear/Set Sol n output" },

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ NTC Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "temp_ntc", 				CMD_GET_TEMP_NTC,			" : Get NTC temp value" },

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Test LASER Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "laser_set", 				CMD_LASER_SET,				" : Switch on/off a channel" },
	{ "laser_dac", 				CMD_LASER_DAC,				" : Set DAC output volt" },

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
	if ((receive_argm[1] < 0) || (receive_argm[1] > 11))
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

/* :::::::::: NTC Command :::::::: */
int CMD_GET_TEMP_NTC(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm;

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

	if (!strcmp(argv[0], "all"))
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
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
