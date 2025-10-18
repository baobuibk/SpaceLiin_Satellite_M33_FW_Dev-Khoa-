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

#ifndef SCH_I2C_H
#define SCH_I2C_H

#include <stdint.h>

#include "../../../../FlightSystem/drivers/stm32/include/drivers.h"


int8_t i2c_write_n(uint8_t addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);

/*!
 *    @brief  Low level generic i2c write
 *    @param  addr, reg_addr, reg_data, len
 *    @return err_code
 */

int8_t i2c_read_n(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len, uint8_t delay_ms);

/*!
 *    @brief  Low level generic i2c read
 *    @param  addr, reg_addr, reg_data, len
 *    @return err_code
 */

int8_t i2c_read_from_n(uint8_t dev_id, uint8_t *reg_data, uint8_t len);

/*!
 *    @brief  Low level specific i2c write
 *    @param  addr, data
 *    @return err_code
 */

int8_t i2c_write_addr(uint8_t dev_id, uint8_t addr, uint8_t data);

#endif /* SCH_I2C_H */
