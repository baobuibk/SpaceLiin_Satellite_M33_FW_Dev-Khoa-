/*
 * bsp_laser.h
 *
 *  Created on: Oct 17, 2025
 *      Author: HTSANG
 */

#ifndef BSP_BSP_LASER_H_
#define BSP_BSP_LASER_H_

#include "adg1414.h"
#include "mcp4902.h"

void bsp_laser_init(void);
void bsp_laser_int_sw_on(uint8_t channel);
void bsp_laser_int_sw_off(uint8_t channel);
void bsp_laser_int_all_sw_off(void);

void bsp_laser_int_set_dac(uint8_t code);
uint8_t bsp_laser_int_get_dac(void);
void bsp_laser_ext_set_dac(uint8_t code);
uint8_t bsp_laser_ext_get_dac(void);

#endif /* BSP_BSP_LASER_H_ */
