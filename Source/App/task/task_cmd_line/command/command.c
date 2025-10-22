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
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Test IO Expander Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "io_set", 				CMD_IO_CTRL,				" : Turn on specific io" },
	{ "heat_set", 				CMD_HEATER_CTRL,			" : Turn on specific heater" },
	{ "read_temp", 				CMD_READ_TEMP,				" : Read onboard temp" },

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Test IO Expander Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "io_set", 				CMD_IO_CTRL,				" : Turn on specific io" },
	{ "heat_set", 				CMD_HEATER_CTRL,			" : Turn on specific heater" },
	{ "read_temp", 				CMD_READ_TEMP,				" : Read onboard temp" },

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ NTC Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "temp_ntc", 				CMD_GET_TEMP_NTC,			" : Get NTC temp value" },

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Test CAN Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "can_send_demo", 			CMD_CAN_SEND_DEMO,			" : Send demo can frame" },
	{ "can_send_2byte", 		CMD_CAN_SEND_2BYTE,			" : Send 2 byte of user input" },

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ultility Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "help", 					CMD_HELP,					" : Display list of commands" },
	{ "clear",           		CMD_CLEAR_SCREEN,           " : Clear console screen" },

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End Of Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ 0, 0, 0 }
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
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
			bsp_debug_console_printf("> NTC TEMP %d: %d, NTC TEMP %d: %d, NTC TEMP %d: %d\n", i, temp_ntc[i], i + 1, temp_ntc[i + 1], i + 2, temp_ntc[i + 2]);
		}
		
		// Return success.
		return CMDLINE_OK;
	}

	receive_argm = atoi(argv[1]);

	if ((receive_argm < 0) || (receive_argm > 12))
		return CMDLINE_INVALID_ARG;

	int16_t temp_ntc = 0;

	system_data_get_NTC(receive_argm, &temp_ntc);

	// bsp_debug_console_printf("> CURRENT: %d mA\n", current_ma);
	bsp_debug_console_printf("> NTC TEMP: %d C\n", temp_ntc);

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

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
