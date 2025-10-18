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

#ifndef SUCHAI_FLIGHT_SOFTWARE_TASKINIT_H
#define SUCHAI_FLIGHT_SOFTWARE_TASKINIT_H

#include <flight/config.h>
#include "init.h"
#include "drivers.h"
#include "log_utils.h"
#include "osDelay.h"
#include "osQueue.h"
#include "osThread.h"
#include "osThread.h"
#include "taskConsole.h"
#if SCH_COMM_ENABLE
#include "taskCommunications.h"
#endif



void csp_add_zmq_iface(int node);
int init_setup_libcsp(int node);


void taskInit(void *param);

/**
 * This function is called at the end of taskInit to execute
 * specific app initialization routines
 *
 * @note APP MUST PROVIDE AN IMPLEMENTATION OF THIS FUNCTION
 *
 * @param params pointer to taskInit params
 */
extern void initAppHook(void *params);

#endif //SUCHAI_FLIGHT_SOFTWARE_TASKINIT_H
