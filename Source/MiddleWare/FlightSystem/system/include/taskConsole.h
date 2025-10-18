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

#ifndef T_COSOLE_H
#define T_CONSOLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <flight/config.h>
#include "globals.h"
#include "log_utils.h"
#ifdef SCH_OS_LINUX
#include "linenoise.h"
#endif

#include "osQueue.h"
#include "osDelay.h"

#include "repoCommand.h"

void taskConsole(void *param);
int console_init(void);
int console_read(char *buffer, int len);
void console_set_prompt(char *prompt);
uint32_t console_command_process(uint8_t * buffer, cmd_t * command);

#endif //T_CONSOLE_H
