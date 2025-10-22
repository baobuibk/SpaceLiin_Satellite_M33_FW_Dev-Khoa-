/*
 * bsp_photo.h
 *
 *  Created on: Oct 18, 2025
 *      Author: HTSANG
 */

#ifndef BSP_BSP_PHOTO_H_
#define BSP_BSP_PHOTO_H_

#include "adg1414.h"

void bsp_photo_init(void);
void bsp_photo_int_sw_on(uint8_t channel);
void bsp_photo_int_sw_off(uint8_t channel);
void bsp_photo_int_all_sw_off(void);

#endif /* BSP_BSP_PHOTO_H_ */
