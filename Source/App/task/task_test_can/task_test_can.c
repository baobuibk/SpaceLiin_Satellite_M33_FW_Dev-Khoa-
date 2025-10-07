/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Task common includes. */
#include "task_common.h"

/* Component includes. */

/* USER include. */
#include "task_test_can.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define TASK_TEST_CAN_PRINTF                bsp_debug_console_printf
#define TASK_TEST_CAN_SEND_CHAR             bsp_debug_console_send_char
#define TASK_TEST_CAN_SEND_STRING           bsp_debug_console_send_string

#define TASK_TEST_CAN_GET_FRAME(p_frame)    bsp_libcsp_can_get_frame((p_frame))
#define TASK_TEST_CAN_RX_EMPTY              bsp_libcsp_RX_buffer_empty

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
void Task_Test_CAN(void *pvParameters)
{
    uint8_t time_out;
    const TickType_t delay_period = pdMS_TO_TICKS(50);   // 50 ms
          TickType_t last_delay;

    flexcan_frame_t RX_frame;

for(;;)
{
    last_delay = xTaskGetTickCount();

    for(time_out = 50; (!TASK_TEST_CAN_RX_EMPTY()) && (time_out != 0); time_out--)
    {
        TASK_TEST_CAN_GET_FRAME(&RX_frame);

        TASK_TEST_CAN_PRINTF("%x %x %x %x %x %x %x %x\n", RX_frame.dataByte0, RX_frame.dataByte1, RX_frame.dataByte2, RX_frame.dataByte3, RX_frame.dataByte4, RX_frame.dataByte5, RX_frame.dataByte6, RX_frame.dataByte7);
    }

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
