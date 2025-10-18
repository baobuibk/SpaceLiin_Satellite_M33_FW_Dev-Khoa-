/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Task common includes. */
#include "task_common.h"

/* Component includes. */

/* USER include. */
#include "task_update_temp.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// #define TASK_TEST_CAN_PRINTF                bsp_debug_console_printf
// #define TASK_TEST_CAN_SEND_CHAR             bsp_debug_console_send_char
// #define TASK_TEST_CAN_SEND_STRING           bsp_debug_console_send_string

// #define TASK_TEST_CAN_GET_FRAME(p_frame)    bsp_libcsp_can_get_frame((p_frame))
// #define TASK_TEST_CAN_RX_EMPTY              bsp_libcsp_RX_buffer_empty

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* :::::::::: Test CAN Task ::::::::::::: */
void Task_Update_Temp(void *pvParameters)
{
    // uint8_t time_out;
    const TickType_t delay_period = pdMS_TO_TICKS(1000);   // 50 ms
          TickType_t last_delay;

    // flexcan_frame_t RX_frame;

for(;;)
{
    last_delay = xTaskGetTickCount();

    bsp_adc0_update();

    // wake up exactly every 100 ms
    vTaskDelayUntil(&last_delay, delay_period);
}
}

//static void CMD_send_splash()
//{
//    for(uint8_t i = 0 ; i < 21 ; i++)
//    {
//		TASK_CMD_LINE_SEND_STRING((&SPLASH[i][0]);
//	}
//	TASK_CMD_LINE_SEND_STRING(("> ");
//}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
