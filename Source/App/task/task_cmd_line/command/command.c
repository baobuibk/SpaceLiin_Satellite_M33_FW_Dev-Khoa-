/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Task common includes. */
#include "task_common.h"

/* Component includes. */
#include "cmd_line.h"

/* USER include. */
#include "command.h"
#include "cmd_test_can.h"
#include "cmd_test_spi.h"

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

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Test SPI Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "spi_read_adc", 			CMD_SPI_READ_ADC,			" : Test SPI read and write" },

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

	uint32_t temp_raw = bsp_adc0_update();

	bsp_debug_console_printf("> CURRENT RAW TEMP: %d", temp_raw);

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
