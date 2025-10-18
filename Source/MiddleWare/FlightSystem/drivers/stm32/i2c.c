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

#include "i2c.h"

#include <stdint.h>


/**************************************************************************/
/*!
    @brief  Write n bytes over I2C
*/
/**************************************************************************/
int8_t __attribute__((weak)) i2c_write_n(uint8_t addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    return 0;
}

/**************************************************************************/
/*!
    @brief  Reads n bytes over I2C
*/
/**************************************************************************/
int8_t __attribute__((weak)) i2c_read_n(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len, uint8_t delay_ms)
{
    return 0;
}

/**************************************************************************/
/*!
    @brief  Reads n bytes over I2C
*/
/**************************************************************************/
int8_t __attribute__((weak)) i2c_read_from_n(uint8_t dev_id, uint8_t *reg_data, uint8_t len)
{
    return 0;
}

/**************************************************************************/
/*!
    @brief  write i2c addr and 1 byte data over I2C
*/
/**************************************************************************/
int8_t __attribute__((weak)) i2c_write_addr(uint8_t dev_id, uint8_t addr, uint8_t data)
{
    return 0;
}
