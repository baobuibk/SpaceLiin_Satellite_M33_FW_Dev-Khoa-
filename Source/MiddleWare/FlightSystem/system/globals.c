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

#include "globals.h"

osQueue dispatcher_queue;         ///< Commands queue
osQueue console_status_queue;       ///< console queue for command response output
osQueue comm_status_queue;      ///< CSP comm queue for command output
osSemaphore repo_data_sem;        ///< Data repository mutex
osSemaphore repo_data_fp_sem;     ///< Flight plan repository mutex
osSemaphore repo_machine_sem;     ///< State status_machine repository mutex
osSemaphore repo_cmd_sem;         ///< Command repository mutex
