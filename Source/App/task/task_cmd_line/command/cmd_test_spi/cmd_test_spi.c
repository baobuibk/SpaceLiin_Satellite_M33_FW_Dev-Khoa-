/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// #include "stdio.h"

/* Task common includes. */
#include "task_common.h"

/* Component includes. */
#include "cmd_line.h"

/* USER include. */
#include "cmd_test_spi.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static void double_to_string(double value, char *buffer, uint8_t precision);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* :::::::::: Test CAN Command :::::::: */
int CMD_SPI_READ_SENSOR(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;

    bsp_i2c_sensor_read_value(SENSOR_READ_BMP390);

    char pressure_string[16] = {0};
	char temperature_string[16] = {0};

	double_to_string((Sensor_Pressure / 100.0), pressure_string, 3);
	double_to_string(Sensor_Temp, temperature_string, 3);
	
	bsp_debug_console_printf("> BMP P: %s hPa, T: %s C\n\r", pressure_string, temperature_string);

	// Return success.
	return CMDLINE_OK;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// static void double_to_string(double value, char *buffer, uint8_t precision)
// {
//     // Handle negative numbers
//     if (value < 0)
// 	{
//         *buffer++ = '-';
//         value = -value;
//     }

//     // Extract the integer part
//     uint32_t integer_part  = (uint32_t)value;
//     double fractional_part = value - integer_part;

//     // Convert integer part to string
//     sprintf(buffer, "%ld", integer_part);
//     while (*buffer) buffer++; // Move pointer to the end of the integer part

//     // Add decimal point
//     if (precision > 0)
// 	{
//         *buffer++ = '.';

//         // Extract and convert the fractional part
//         for (uint8_t i = 0; i < precision; i++)
// 		{
//             fractional_part *= 10;
//             uint8_t digit = (uint8_t)fractional_part;
//             *buffer++ = '0' + digit;
//             fractional_part -= digit;
//         }
//     }

//     // Null-terminate the string
//     *buffer = '\0';
// }

static void double_to_string(double value, char *buffer, uint8_t precision)
{
    /* Sign */
    if (value < 0.0)
    {
        *buffer++ = '-';
        value = -value;
    }

    /* Split into integer and fractional parts */
    uint32_t integer_part  = (uint32_t)value;          /* NOTE: clips to 32-bit range */
    double   fractional_part = value - (double)integer_part;
    if (fractional_part < 0.0) fractional_part = 0.0;  /* guard tiny FP negatives */

    /* Integer -> string (in place, no sprintf) */
    if (integer_part == 0U)
    {
        *buffer++ = '0';
    }
    else
    {
        char rev[10];  /* max 10 digits for uint32_t */
        uint8_t n = 0;
        while (integer_part != 0U)
        {
            rev[n++] = (char)('0' + (integer_part % 10U));
            integer_part /= 10U;
        }
        while (n--)
        {
            *buffer++ = rev[n];
        }
    }

    /* Fractional part (truncate, no rounding) */
    if (precision > 0U)
    {
        *buffer++ = '.';
        for (uint8_t i = 0; i < precision; i++)
        {
            fractional_part *= 10.0;
            uint8_t digit = (uint8_t)fractional_part;

            /* Clamp against rare FP edge (e.g., 9.999999 -> 10) */
            if (digit > 9U) digit = 9U;

            *buffer++ = (char)('0' + digit);
            fractional_part -= (double)digit;
            if (fractional_part < 0.0) fractional_part = 0.0; /* guard drift */
        }
    }

    *buffer = '\0';
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
