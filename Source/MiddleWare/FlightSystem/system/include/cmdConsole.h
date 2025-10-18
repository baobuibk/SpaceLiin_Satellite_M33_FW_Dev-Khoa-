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

#ifndef CMD_CONSOLE_H
#define CMD_CONSOLE_H


#include "log_utils.h"
#include "repoCommand.h"



/**
 * Set the log verbosity level and current node to send logs
 *  - level can be 0 to 5 @see log_level_t
 *  - node can be -1 to use stdout, or > 0 to to send log using CSP to <node>
 *
 * @param fmt Str. Parameters format "%d %d"
 * @param params Str. Parameters as string "<level> <node>"
 * @param nparams Int. Number of parameters 2
 * @return  CMD_OK if executed correctly, CMD_ERROR in case of failures, or CMD_ERROR_SYNTAX in case of parameters errors
 *
 * Example
 * @code
 * //Set log level to DEBUG using STDOUT
 * log_set 4 -1
 * con_set_logger("%d %d", "4 -1", 2);
 *
 * #Set log level to INFO and send log to CSP node 10
 * log_set 3 10
 * con_set_logger("%d %d", "3 10", 2);
 * @endcode
 */
void cmd_set_logger(int lvl, int node);

void Cmd_help(uint32_t argc, uint8_t *argv[],  cmd_result_t * result) ;

#endif /* CMD_CONSOLE_H */
