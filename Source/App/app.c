/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* BoardSupport includes. */
#include "bsp.h"

/* Task includes. */
#include "task_cmd_line.h"

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

    /* Create task. */
    if (xTaskCreate(Task_CMD_Line, "Task_CMD_Line", configMINIMAL_STACK_SIZE + 100, NULL, Task_CMD_Line_PRIORITY, NULL) != pdPASS)
    {
        bsp_debug_console_printf("Task creation failed!.\r\n");
        while (1)
            ;
    }

    /* Init task */
    Task_CMD_Line_Init();

    vTaskStartScheduler();
    for (;;)
        ;
}