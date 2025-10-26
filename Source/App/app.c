/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* BoardSupport includes. */
#include "app.h"
#include "bsp.h"

/* Task includes. */
#include "task_cmd_line.h"
// #include "task_test_can.h"
#include "task_update_onboard_adc.h"
#include "task_experiment.h"

/* System data includes. */
#include "system_data.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Task priorities. */
#define Task_CMD_Line_PRIORITY (configMAX_PRIORITIES - 1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void Task_Init(void *pvParameters);

static TaskHandle_t Task_Init_Handle = NULL;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
void app_main(void)
{
    /* Init board hardware. */
    BSP_Init();

    xTaskCreate(Task_Init, "Task_Init", configMINIMAL_STACK_SIZE + 38, NULL, Task_CMD_Line_PRIORITY, &Task_Init_Handle);

    vTaskStartScheduler();
    for (;;)
        ;
}

static void Task_Init(void *pvParameters)
{
    for(;;)
    {
        /* Init system data */
        system_data_init();

        /* IO core driver init */
        spi_io_init(&onboard_adc_spi);
        spi_io_init(&photo_adc_spi);

        i2c_io_init(&io_expander_i2c);

        /* Init board peripheral. */
        bsp_debug_console_init();
        // bsp_libcsp_can_init();
        bsp_expander_init();
        bsp_heater_init();
        bsp_onboard_adc_init();
        bsp_laser_init();
        bsp_photo_init();

        // Pull up RAM SPI nCS
        bsp_expander_ctrl(RAM_SPI_nCS, 1);

        /* Init task */
        Task_CMD_Line_Init();

        /* Task Create */
        xTaskCreate(Task_Experiment, "Task_Experiment", configMINIMAL_STACK_SIZE + 38, NULL, Task_CMD_Line_PRIORITY, NULL);
        // xTaskCreate(Task_Test_CAN, "Task_Test_CAN", configMINIMAL_STACK_SIZE + 38, NULL, Task_CMD_Line_PRIORITY, NULL);
        xTaskCreate(Task_CMD_Line, "Task_CMD_Line", configMINIMAL_STACK_SIZE + 38, NULL, Task_CMD_Line_PRIORITY - 1, NULL);
        // xTaskCreate(Task_Update_Onboard_ADC, "Task_Update_Onboard_ADC", configMINIMAL_STACK_SIZE + 38 + 128, NULL, Task_CMD_Line_PRIORITY - 2, NULL);

        vTaskDelete(Task_Init_Handle);
    }
}