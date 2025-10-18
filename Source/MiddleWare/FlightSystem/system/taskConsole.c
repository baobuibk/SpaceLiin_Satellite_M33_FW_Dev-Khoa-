/*
 * Copyright 2025 SUCHAI Flight Software v2 project and contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "taskConsole.h"
#include "repoData.h"
#include "repoCommand.h"
#include "repoDataSchema.h"
#include "globals.h"
#include "bsp_uart.h"
#include "printf.h"
#include "error_codes.h"
static const char *tag = "Console";
static char console_prompt[] = "SUCHAI-v2> ";
static const char console_banner[] =
"\n"
"______________________________________________________________________________\r\n"
"                    ______  _________ _____   ____       ___ \r\n"
"                   / __/ / / / ___/ // / _ | /  _/ _  __|_  |\r\n"
"                  _\\ \\/ /_/ / /__/ _  / __ |_/ /  | |/ / __/ \r\n"
"                 /___/\\____/\\___/_//_/_/ |_/___/  |___/____/ \r\n"
"______________________________________________________________________________\r\n";



//"\n\n====== WELCOME TO THE SUCHAI CONSOLE - PRESS ANY KEY TO START ======\n\r";

void taskConsole(void *param)
{
    LOGI(tag, "Started");
    uint32_t retVal;
    cmd_t 		new_cmd;
    uint8_t 	buffer[SCH_BUFF_MAX_LEN];
    uint32_t 	commandBufferIndex = 0;
    uint8_t	rxData;
    cmd_result_t console_command_ret;
    /* Initializing console */
    memset(buffer, '\0', SCH_BUFF_MAX_LEN);
    console_init();

    LOGI(tag, console_banner);

    while(1)
    {
        osDelay(10);


        while(_getchar(&rxData) == 0)
        {
			_putchar(rxData);
			if ((rxData == '\r') || (rxData == '\n'))		//got a return or new line
			{
				if (commandBufferIndex > 0)		//if we got the CR or LF at the begining, discard
				{
					buffer[commandBufferIndex] = 0;
					commandBufferIndex = 0;
					retVal = cmd_build_from_str(buffer,&new_cmd);
					if (CMDLINE_OK == retVal)
					{

				            LOGI(tag, "sent command %s\n\r", new_cmd.args.args);

				            /* Queue NewCmd - Blocking */
				            value32_t cmds_id =  dat_get_status_var(dat_exp_cmds);
				            new_cmd.id = cmds_id.i;
				            cmds_id.i = cmds_id.i + 1;
				            dat_set_status_var(dat_exp_cmds, cmds_id);

				            new_cmd.cmdSrc = CMD_SRC_CONSOLE;
				            cmd_send(new_cmd);	//sendout the command
				            if (pdPASS == osQueueReceive(console_status_queue, &console_command_ret, 2000))
				            {
					            if (ERROR_OK == console_command_ret.error)
					            {
					            	printf((char *)console_command_ret.output);
					            }
					            else printf("ERROR\r\n");

				            }

				        }
				        else
				        {
				            LOGW(tag, "An invalid command was read!");
				        }

					}

				printf("\r\n%s",console_prompt);
			}
			else if ((rxData == 8) || (rxData == 127))
			{
				if (commandBufferIndex > 0) commandBufferIndex--;
			}
			else
			{
				buffer[commandBufferIndex] = rxData;
				commandBufferIndex ++;
				if (commandBufferIndex >= (SCH_BUFF_MAX_LEN -1)) commandBufferIndex= 0;
			}
		}
    }
}

int console_init(void)
{
    LOGD(tag, "Init...\n");

//    cmd_print_all();
    return 0;
}



void console_set_prompt(char *prompt)
{
    if(prompt != NULL) {
        strncpy(console_prompt, prompt, 12);
        strcat(console_prompt, "> ");
    }
}


