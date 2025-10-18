/*
 * cmdComm.c
 *
 *  Created on: Oct 14, 2025
 *      Author: Admin
 */


/*
 * ping a server by CSP
 *
 */
#include "repoCommand.h"
#include "csp/csp.h"
#include "error_codes.h"
void cmd_comm_ping(uint32_t argc, uint8_t *argv[],  cmd_result_t * result) {
	uint8_t server_address = atoi((char *)argv[1]);

	int ret = csp_ping(server_address, 500, 100, CSP_O_NONE);
	if (ret > 0)
	{
		result->error = ERROR_OK;
		result->result.u = ret;
		memcpy(result->output, "replied\r\n", sizeof("replied\r\n"));
		result->output[sizeof("replied\r\n")] = 0;
	}
	else
	{
		result->error = ERROR_FAIL;
		memcpy(result->output, "no replied\r\n", sizeof("no replied\r\n"));
		result->output[sizeof("no replied\r\n")] = 0;
	}
}


