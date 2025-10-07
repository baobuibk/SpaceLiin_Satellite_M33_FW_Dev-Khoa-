/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* BoardSupport includes. */
#include "bsp.h"

/* Task includes. */
#include "task_cmd_line.h"
#include "task_test_can.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Task priorities. */
#define Task_CMD_Line_PRIORITY (configMAX_PRIORITIES - 1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
int app_main(void)
{
    /* Init board hardware. */
    BSP_Init();

    xTaskCreate(Task_Test_CAN, "Task_Test_CAN", configMINIMAL_STACK_SIZE + 38, NULL, Task_CMD_Line_PRIORITY, NULL);
    xTaskCreate(Task_CMD_Line, "Task_CMD_Line", configMINIMAL_STACK_SIZE + 38, NULL, Task_CMD_Line_PRIORITY - 1, NULL);

    /* Init task */
    Task_CMD_Line_Init();

    vTaskStartScheduler();
    for (;;)
        ;
}