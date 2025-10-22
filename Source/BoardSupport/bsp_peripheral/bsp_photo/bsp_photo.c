/*
 * bsp_photo.c
 *
 *  Created on: Oct 18, 2025
 *      Author: HTSANG
 */


#include "bsp_photo.h"
#include "board.h"

static int8_t map_PD_position(int x) {
    static const uint8_t map[] = {
        0,
        24, 23, 22, 21, 17, 18, 19, 20,
        16, 15, 14, 13, 9, 10, 11, 12,
        8, 7, 6, 5, 1, 2, 3, 4
    };

    if (x < 1 || x > 24)
        return -1;
    return map[x];
}

void bsp_photo_init(void)
{
	adg1414_chain_init(&photo_sw_dev, &photo_spi, &photo_sw_dev.cs, photo_sw_dev.num_of_sw);
}

void bsp_photo_int_sw_on(uint8_t channel)
{
	uint8_t real_channel = map_PD_position(channel);
	spi_io_set_mode(laser_int_dev.spi, 1);
	adg1414_chain_sw_on(&photo_sw_dev, real_channel);
}

void bsp_photo_int_sw_off(uint8_t channel)
{
	uint8_t real_channel = map_PD_position(channel);
	spi_io_set_mode(laser_int_dev.spi, 1);
	adg1414_chain_sw_off(&photo_sw_dev, real_channel);
}

void bsp_photo_int_all_sw_off(void)
{
	spi_io_set_mode(photo_sw_dev.spi, 1);
	adg1414_chain_all_sw_off(&photo_sw_dev);
}
