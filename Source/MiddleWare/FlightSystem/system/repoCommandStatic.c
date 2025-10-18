/*
 * repoCommandStatic.c
 *
 *  Created on: Oct 7, 2025
 *      Author: Admin
 */

#include "repoCommand.h"
#include "error_codes.h"
#include "cmdConsole.h"
#include "powerCommand.h"
#include "cmdComm.h"


const char * ErrorCode[5] = {"OK\r\n", "CMDLINE_BAD_CMD\r\n", "CMDLINE_TOO_MANY_ARGS\r\n",
"CMDLINE_TOO_FEW_ARGS\r\n", "CMDLINE_INVALID_ARG\r\n" };

enum {
	HELP,
	TEC_INIT,
	TEC_ENA,
	TEC_SET_VOLT,
	TEC_SET_DIR,
	TEC_STAT,
	HEATER_SET,
	LASER_INT_SET,
	LASER_INT_GET_CURRENT,
	LASER_EXT_SET,
	LASER_EXT_GET_CURRENT,
	LASER_INT_ON,
	LASER_INT_OFF,
	LASER_EXT_ON,
	LASER_EXT_OFF,
	PHOTO_ON,
	PHOTO_OFF,
	PHOTO_GET,
	POWER_HEATER_CONTROL,
	POWER_HEATER_STATUS,
	POWER_SOLENOID_CONTROL,
	POWER_SOLENOID_STATUS,



	COMM_PING,
	MAX_COMMAND
};



cmd_table_entry_t g_psCmdTable[] = {
	{ HELP, "help", Cmd_help," : Display list of commands, format: help" },
	{TEC_INIT,"tec_init",Cmd_help, " : Initialize TEC driver, format: tec_init tec_num(0-3), a=all]"},
	{TEC_ENA,"tec_ena",Cmd_help, " : enable tec, format tec_ena tec_num (0-3)"},
	{TEC_SET_VOLT,"tec_set_volt",Cmd_help, " : set tec output, format: tec_set_volt tec_num(0-3) tec_volt(255 is 2.55)"},
	{TEC_SET_DIR,"tec_set_dir",Cmd_help, " : set tec direction, format: tec_set_dir tec_num(0-3) dir (C/H)"},
	{TEC_STAT,"tec_stat",Cmd_help, " : get tec status, format: tec_stat tec_num(0-3, a=all)"},

	{HEATER_SET,"heater_set",Cmd_help, " : set heater duty cycle, format: heater_set heater_num(0-7) duty (0-100)"},

	// Laser Photo

	{ LASER_INT_SET, "laser_int_set", Cmd_help, " : set the internal laser current in percent, format: laser_int_set [percent]"},
	{ LASER_INT_GET_CURRENT, "laser_int_get_current", Cmd_help, " : get the internal laser current in mA, format: laser_int_get_current"},
	{ LASER_EXT_SET, "laser_ext_set", Cmd_help, " : set the external laser current in percent, format: laser_ext_set [percent]"},
	{ LASER_EXT_GET_CURRENT, "laser_ext_get_current", Cmd_help, " : get the external laser current in mA, format: laser_ext_get_current"},

	{ LASER_INT_ON, "laser_int_on", Cmd_help,   " : switch internal laser on, format: laser_int_on pos(0-35)" },
	{ LASER_INT_OFF, "laser_int_off",Cmd_help,    " : switch internal laser off, format: laser_int_off pos(0-35, a=all)" },
	{ LASER_EXT_ON, "laser_ext_on", Cmd_help,   " : switch external laser on, format: laser_ext_on pos(0-35)" },
	{ LASER_EXT_OFF, "laser_ext_off",Cmd_help,    " : switch external laser off, format: laser_ext_off pos(0-35, a=all)" },

	{ PHOTO_ON, "photo_on",Cmd_help,    " : switch photo on, format: photo_on pos(0-35)" },
	{ PHOTO_OFF, "photo_off",Cmd_help,    " : switch photo off, format: photo_off pos(0-35, a = all)" },
	{ PHOTO_GET, "photo_get", Cmd_help,   " : get transimpedance ADC value, format: photo_get)" },
	{ POWER_HEATER_CONTROL,"power_heater_control",power_heater_control,  " : control 12V heater, format: power_heater_control status (on/off)" },
	{ POWER_HEATER_STATUS, "power_heater_status",power_heater_status, " : get 12V heater status, format: power_heater_status" },
	{ POWER_SOLENOID_CONTROL, "power_solenoid_control", Cmd_help,    " : control 12V solenoid, format: power_solenoid_control status (on/off)" },
	{ POWER_SOLENOID_STATUS, "power_solenoid_status",Cmd_help, " : get 12V solenoid status, format: power_solenoid_status" },

	{COMM_PING, "csp_ping",cmd_comm_ping, " : ping CSP server, format: csp_ping csp_id"},
	{MAX_COMMAND,0,0,0}
};


uint32_t repo_command_static_find_command(const uint8_t * function, cmd_t * cmd)
{

    //
    // Start at the beginning of the command table, to look for a matching
    // command.
    //
	cmd_table_entry_t * psCmdEntry = &g_psCmdTable[0];

    //
    // Search through the command table until a null command string is
    // found, which marks the end of the table.
    //
    while(psCmdEntry->pcCmd)
    {
        //
        // If this command entry command string matches argv[0], then call
        // the function for this command, passing the command line
        // arguments.
        //
        if(!strcmp((char *)function, (char *)psCmdEntry->pcCmd))
        {
        	cmd->function = (psCmdEntry->pfnCmd);
            cmd->id = psCmdEntry->id;
            return ERROR_OK;
        }
        //
        // Not found, so advance to the next entry.
        //
        psCmdEntry++;
    }
    return ERROR_NOT_SUPPORTED;
}

uint32_t cmd_build_from_str(uint8_t * buffer, cmd_t * command)
{

	bool foundArg = true; //get the first argument
	uint8_t * commandArg = command->args.args;
	uint32_t nArg = 0;
	uint32_t retVal;

	memset(commandArg, '\0', SCH_BUFF_MAX_LEN);
	while (*buffer)
	{
		if (*buffer == ' ')
		{
				*commandArg = 0;
				foundArg = true;

		}
		else // *buffer not zero
		{
			*commandArg = *buffer;
			if (foundArg)
			{
				foundArg = false;
				command->args.argv[nArg] = commandArg;
				nArg ++;
				if (nArg >= SCH_CMD_MAX_ARG) return CMDLINE_TOO_MANY_ARGS;
			}
		}
		buffer++;
		commandArg++;
	}
	*commandArg = 0; //make sure we have terminated string
	command->args.nargs = nArg;
	retVal = repo_command_static_find_command(command->args.argv[0], command);
	return retVal;
}



