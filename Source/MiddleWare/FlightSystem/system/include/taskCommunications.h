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

#ifndef T_COMMUNICATIONS_H
#define T_COMMUNICATIONS_H

#include <stdlib.h>

#include "csp/csp.h"
#include "csp/csp_endian.h"
#include "csp/csp_types.h"
#include "osDelay.h"
#include "osQueue.h"
#include "globals.h"
#include <csp/csp.h>
#include <flight/config.h>
#include "repoCommand.h"


/**
 * Application specific function to process incoming packets
 * If SCH_HOOK_COMM is defined, the application must provide an taskCommunicationHook implementation
 * @param conn Current CSP connection
 * @param packet Received csp packet
 */
void taskCommunicationsHook(csp_conn_t *conn, csp_packet_t *packet);

void taskCommunications(void *param);

#endif //T_COMMUNICATIONS_H
