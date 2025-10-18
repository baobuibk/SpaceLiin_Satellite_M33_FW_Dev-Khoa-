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
#ifndef GLOBALS_H
#define GLOBALS_H

#include "osQueue.h"
#include "osSemphr.h"

#define SCH_OK 0
#define SCH_ERROR -1
#define SCH_SYNTAX_ERROR -2

extern osQueue dispatcher_queue;         ///< Commands queue
extern osQueue console_status_queue;       ///< result queue for consloe
extern osQueue comm_status_queue;      ///< result queue for csp
extern osSemaphore repo_data_sem;        ///< Data repository mutex
extern osSemaphore repo_data_fp_sem;     ///< Flight plan repository mutex
extern osSemaphore repo_machine_sem;     ///< State status_machine repository mutex
extern osSemaphore repo_cmd_sem;         ///< Command repository mutex

#endif //GLOBALS_H
