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
#include "printf.h"
#include "repoDataSchema.h"
static const char *tag = "repoDataSchema";


dat_sys_var_t dat_get_status_var_def(dat_status_address_t address)
{
    dat_sys_var_t var = {0};
    int i;

    if(address < dat_status_last_address)
    {
        for (i = 0; i < dat_status_last_var; i++)
        {
            if (dat_status_list[i].address == address)
                return dat_status_list[i];
        }
    }

    LOGE(tag, "Status var not found! (%d)", address);
    return var;
}

dat_sys_var_t dat_get_status_var_def_name(char *name)
{
    dat_sys_var_t var;
    var.status = -1;
    int i;

    if(name != NULL)
    {
        for (i = 0; i < dat_status_last_var; i++)
        {
            if (strcmp(dat_status_list[i].name, name) == 0)
                return dat_status_list[i];
        }
    }

    LOGE(tag, "Status var not found! (%s)", name);
    return var;
}

//void dat_print_system_var(dat_sys_var_t *status)
//{
//    assert(status != NULL);
//
//    switch (status->type) {
//        case 'u':
//            printf("%3d, %-20s, %u, %d\r\n", status->address, status->name, status->value.u, status->status);
//            break;
//        case 'd':
//            printf("%3d, %-20s, %d, %d\r\n", status->address, status->name, status->value.i, status->status);
//            break;
//        case 'f':
//            printf("%3d, %-20s, %.6f, %d\r\n", status->address, status->name, status->value.f, status->status);
//            break;
//        default:
//            printf("%3d, %-20s, %#X, %d\r\n", status->address, status->name, status->value.u, status->status);
//    }
//}

