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

#include "mainFS.h"
#include "printf.h"
const char *tag = "fs_main";

int suchai_main(void)
{
    /* On reset */
    on_reset();

      int t_ini_ok = osCreateTask(taskInit, "init", SCH_TASK_INI_STACK, NULL, 2, NULL);
      if(t_ini_ok != 0)
      {
    	  while(1);
      }



    /* Start the scheduler. Should never return */
    osScheduler(NULL, 0);
    return 0;
}
