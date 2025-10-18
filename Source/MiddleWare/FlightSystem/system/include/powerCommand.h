/*
 * powerCommand.h
 *
 *  Created on: Oct 10, 2025
 *      Author: Admin
 */

#ifndef _POWERCOMMAND_H_
#define _POWERCOMMAND_H_
#include "log_utils.h"
#include "repoCommand.h"

void  power_heater_control(uint32_t argc, uint8_t *argv[], cmd_result_t * result);
void power_heater_status(uint32_t argc, uint8_t *argv[], cmd_result_t * result);

#endif /* MIDDLEWARE_FLIGHTSYSTEM_SYSTEM_INCLUDE_POWERCOMMAND_H_ */
