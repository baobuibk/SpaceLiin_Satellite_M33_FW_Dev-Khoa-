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

#include "mem.h"

#include <assert.h>
#include <flight/config.h>
#include <stdint.h>
#include <string.h>


/**
 * Simulated flash memory and FRAM/MRAM in RAM for debug purposes.
 * These variables should be initialized as follows:
 *
 * #define STORAGE_BLOCK_NUMBER (SCH_STORAGE_FLASH_SIZE / SCH_STORAGE_BLOCK_SIZE)
 * static char flash[STORAGE_BLOCK_NUMBER][SCH_STORAGE_BLOCK_SIZE];
 * char fram[SCH_ST_FRAM_SIZE];
 */
extern char* flash;
extern char* fram;


int __attribute__((weak)) mem_read_flash(uint8_t partition, uint32_t addr, uint8_t* data, uint16_t len)
{
    assert(flash != NULL);
    assert(addr + len <= SCH_STORAGE_FLASH_SIZE);
    memcpy(data, flash + addr, len);
    return 0;
}

int __attribute__((weak)) mem_write_flash(uint8_t partition, uint32_t addr, const uint8_t* data, uint16_t len)
{
    assert(flash != NULL);
    assert(addr + len <= SCH_STORAGE_FLASH_SIZE);
    memcpy(flash + addr, data, len);
    return 0;
}

int __attribute__((weak)) mem_erase_flash(uint8_t partition, uint32_t addr)
{
    assert(flash != NULL);
    assert(addr + SCH_STORAGE_BLOCK_SIZE <= SCH_STORAGE_FLASH_SIZE);
    uint32_t section = addr / SCH_STORAGE_BLOCK_SIZE;
    addr = section * SCH_STORAGE_BLOCK_SIZE;
    memset(flash + addr, 0, SCH_STORAGE_BLOCK_SIZE);
    return 0;
}

int __attribute__((weak)) mem_write_fram(uint32_t add, void* value, size_t len)
{
    assert(fram != NULL);
    assert(add + len <= SCH_ST_FRAM_SIZE);
    memcpy(fram + add, value, len);
    return 0;
}

int __attribute__((weak)) mem_read_fram(uint32_t add, void* value, size_t len)
{
    assert(fram != NULL);
    assert(add + len <= SCH_ST_FRAM_SIZE);
    memcpy(value, fram + add, len);
    return 0;
}

int  __attribute__((weak)) mem_write_file_line(char* filename, uint8_t* buffer, size_t len)
{
    return -1;
}

int __attribute__((weak)) mem_read_file_line(char* filename, int index, uint8_t* buffer, size_t len)
{
    return -1;
}

int __attribute__((weak)) mem_erase_file(char* filename)
{
    return -1;
}
