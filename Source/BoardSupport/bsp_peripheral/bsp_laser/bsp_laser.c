/*
 * bsp_laser.c
 *
 *  Created on: Oct 17, 2025
 *      Author: HTSANG
 */

#include "bsp_laser.h"
#include "board.h"
#include "do.h"

static int8_t map_LD_position(int x) {
    static const uint8_t map[] = {
        0,
        1, 2, 3, 4, 8, 7, 6, 5,
        9, 10, 11, 12, 16, 15, 14, 13,
        17, 18, 19, 20, 24, 23, 22, 21
    };

    if (x < 1 || x > 24)
        return -1;
    return map[x];
}

void bsp_laser_init(void)
{
	adg1414_chain_init(&laser_int_dev, &laser_spi, &laser_int_dev.cs, laser_int_dev.num_of_sw);
	mcp4902_dev_init(&laser_dac_dev, &laser_spi, &laser_dac_dev.cs, &laser_dac_dev.latch);
}

void bsp_laser_int_sw_on(uint8_t channel)
{
	uint8_t real_channel = map_LD_position(channel);
	spi_io_set_mode(laser_int_dev.spi, 1);
	adg1414_chain_sw_on(&laser_int_dev, real_channel);
}

void bsp_laser_int_sw_off(uint8_t channel)
{
	uint8_t real_channel = map_LD_position(channel);
	spi_io_set_mode(laser_int_dev.spi, 1);
	adg1414_chain_sw_off(&laser_int_dev, real_channel);
}

void bsp_laser_int_all_sw_off(void)
{
	spi_io_set_mode(laser_int_dev.spi, 1);
	adg1414_chain_all_sw_off(&laser_int_dev);
}



void bsp_laser_int_set_dac(uint8_t code)
{
	spi_io_set_mode(laser_dac_dev.spi, 0);
	mcp4902_set_dac(&laser_dac_dev, MCP4902_CHA, code);
}

uint8_t bsp_laser_int_get_dac(void)
{
	return laser_dac_dev.dac_channel[MCP4902_CHA];
}

void bsp_laser_ext_set_dac(uint8_t code)
{
	spi_io_set_mode(laser_dac_dev.spi, 0);
	mcp4902_set_dac(&laser_dac_dev, MCP4902_CHB, code);
}

uint8_t bsp_laser_ext_get_dac(void)
{
	return laser_dac_dev.dac_channel[MCP4902_CHB];
}
