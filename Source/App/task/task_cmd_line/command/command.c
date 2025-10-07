/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Task common includes. */
#include "task_common.h"

/* Component includes. */
#include "cmd_line.h"

/* USER include. */
#include "command.h"
#include "cmd_test_can.h"

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
