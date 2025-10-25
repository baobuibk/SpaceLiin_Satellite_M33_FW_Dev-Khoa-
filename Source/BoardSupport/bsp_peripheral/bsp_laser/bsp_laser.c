/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "bsp_laser.h"
#include "bsp_core.h"
#include "do.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
adg1414_dev_t laser_int_dev =
{
		.spi = &onboard_adc_spi,
		.cs  = { .port = 4, .pin = 25, .bStatus = true },
		.num_of_sw = 3
};

adg1414_dev_t laser_ext_dev =
{
		.spi = &onboard_adc_spi,
		.cs  = { .port = 4, .pin = 26, .bStatus = true },
		.num_of_sw = 1
};

mcp4902_dev_t laser_dac_dev =
{
		.spi = &onboard_adc_spi,
		.cs = { .port = 4, .pin = 27, .bStatus = true },
		.latch = { .port = 4, .pin = 24, .bStatus = true },
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static int8_t map_int_LD_position(int x);
static int8_t map_ext_LD_position(int x);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void bsp_laser_init(void)
{
	adg1414_chain_init(&laser_int_dev, &onboard_adc_spi, &laser_int_dev.cs, laser_int_dev.num_of_sw);
	adg1414_chain_init(&laser_ext_dev, &onboard_adc_spi, &laser_ext_dev.cs, laser_ext_dev.num_of_sw);

	mcp4902_dev_init(&laser_dac_dev, &onboard_adc_spi, &laser_dac_dev.cs, &laser_dac_dev.latch);
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

void bsp_laser_int_sw_on(uint8_t channel)
{
	uint8_t real_channel = map_int_LD_position(channel);
	spi_io_set_mode(laser_int_dev.spi, 1);
	adg1414_chain_sw_on(&laser_int_dev, real_channel);
}

void bsp_laser_int_sw_off(uint8_t channel)
{
	uint8_t real_channel = map_int_LD_position(channel);
	spi_io_set_mode(laser_int_dev.spi, 1);
	adg1414_chain_sw_off(&laser_int_dev, real_channel);
}

void bsp_laser_int_all_sw_off(void)
{
	spi_io_set_mode(laser_int_dev.spi, 1);
	adg1414_chain_all_sw_off(&laser_int_dev);
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

void bsp_laser_int_sw_on(uint8_t channel)
{
	uint8_t real_channel = map_ext_LD_position(channel);
	spi_io_set_mode(laser_ext_dev.spi, 1);
	adg1414_chain_sw_on(&laser_ext_dev, real_channel);
}

void bsp_laser_int_sw_off(uint8_t channel)
{
	uint8_t real_channel = map_ext_LD_position(channel);
	spi_io_set_mode(laser_ext_dev.spi, 1);
	adg1414_chain_sw_off(&laser_ext_dev, real_channel);
}

void bsp_laser_int_all_sw_off(void)
{
	spi_io_set_mode(laser_ext_dev.spi, 1);
	adg1414_chain_all_sw_off(&laser_ext_dev);
}

void bsp_laser_read_current()
{
	
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static int8_t map_int_LD_position(int x)
{
    static const uint8_t map[] =
	{
        0,
        1, 2, 3, 4, 8, 7, 6, 5,
        9, 10, 11, 12, 16, 15, 14, 13,
        17, 18, 19, 20, 24, 23, 22, 21
    };

    if (x < 1 || x > 24)
	{
		return -1;
	}

    return map[x];
}

static int8_t map_ext_LD_position(int x)
{
    static const uint8_t map[] =
	{
        0,
        1, 2, 3, 4, 5, 6, 7, 8
    };

    if (x < 1 || x > 8)
	{
		return -1;
	}

    return map[x];
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */