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

#include "taskDispatcher.h"
#include "printf.h"
#include "error_codes.h"
static const char *tag = "Dispatcher";

void taskDispatcher(void *param)
{
	LOGI(tag, "Started");

    int status; /* Status of cmd reading operation */

    cmd_t new_cmd ; /* The new cmd read */
    cmd_result_t cmd_result;

    while(1)
    {
        /* Read new_cmd from Queue - Blocking */
        status = osQueueReceive(dispatcher_queue, &new_cmd, portMAX_DELAY);

        if(status == pdPASS)
        {
            /* Check if command is executable */
            if (check_if_executable(&new_cmd))
            {

                /* Send the command to executer Queue - BLOCKING */
                int executed_cmds = dat_get_system_var(dat_exp_executed_cmds); //Count executed commands
                executed_cmds = executed_cmds + 1; //Add last command executed to count
                dat_set_system_var(dat_exp_executed_cmds, executed_cmds); //Set new count
 //               osQueueSend(executer_cmd_queue, &new_cmd, portMAX_DELAY);
                LOGD(tag, "Cmd: %d, :%s", new_cmd.id, new_cmd.args.args);
                 new_cmd.function(new_cmd.args.nargs, new_cmd.args.argv, &cmd_result);
                LOGI(tag, "Command: %d, error: %d", new_cmd.id, cmd_result.error);
 //send the result to source
                if (CMD_SRC_CONSOLE == new_cmd.cmdSrc)
                {
                	osQueueSend(console_status_queue, &cmd_result, 100);
                }
                else osQueueSend(comm_status_queue, &cmd_result, 100);
                /* Get the result from Executer Stat Queue - BLOCKING */
 //               osQueueReceive(executer_stat_queue, &cmd_result, portMAX_DELAY);
                if (cmd_result.error != CMDLINE_OK)
                {
                    int failed_cmds = dat_get_system_var(dat_exp_failed_cmds);
                    failed_cmds = failed_cmds + 1;
                    dat_set_system_var(dat_exp_failed_cmds, failed_cmds);
                }
            }
        }
    }
}

int check_if_executable(cmd_t *new_cmd)
{
    return 1;
}
