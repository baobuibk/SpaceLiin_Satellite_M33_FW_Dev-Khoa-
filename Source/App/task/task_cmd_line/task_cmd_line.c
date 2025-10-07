/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Task common includes. */
#include "task_common.h"

/* Component includes. */
#include "cmd_line.h"

/* USER include. */
#include "task_cmd_line.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define TASK_CMD_LINE_LPUART_IRQHandler LPUART2_IRQHandler

#define TASK_CMD_LINE_PRINTF            bsp_debug_console_printf
#define TASK_CMD_LINE_SEND_CHAR         bsp_debug_console_send_char
#define TASK_CMD_LINE_SEND_STRING       bsp_debug_console_send_string
#define TASK_CMD_LINE_GET_CHAR          bsp_debug_console_get_char
#define TASK_CMD_LINE_RX_EMPTY          bsp_debug_console_RX_buffer_empty

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
typedef struct _cmd_line_t_
{
                uint16_t    buffer_size;
                char*       p_buffer;

    volatile    uint16_t    write_index;
    volatile    char        RX_char;
} cmd_line_t;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
cmd_line_t          task_cmd_line;
char                g_task_cmd_line_buffer[64];

// static bool         is_warned_user = false;

static const char * ErrorCode[7] = 
{
    "OK\n",
    "CMDLINE_BAD_CMD\n",
    "CMDLINE_TOO_MANY_ARGS\n",
    "CMDLINE_TOO_FEW_ARGS\n",
    "CMDLINE_INVALID_ARG\n",
    "CMDLINE_INVALID_CMD\n",
    "CALIB IS RUNNING, USE CALIB_EXIT TO EXIT CALIB\n",
};

const char SPLASH[][65] = 
{
{"\r\n"},
{".........................................................\r\n"},
{".........................................................\r\n"},
{"..    ____                       _     _               ..\r\n"},
{"..   / ___| _ __   __ _  ___ ___| |   (_)_ __  _ __    ..\r\n"},
{"..   \\___ \\| '_ \\ / _` |/ __/ _ \\ |   | | '_ \\| '_ \\   ..\r\n"},
{"..    ___) | |_) | (_| | (_|  __/ |___| | | | | | | |  ..\r\n"},
{"..   |____/| .__/ \\__,_|\\___\\___|_____|_|_| |_|_| |_|  ..\r\n"},
{"..         |_|    _   _ _____ _____                    ..\r\n"},
{"..               | | | | ____| ____|                   ..\r\n"},
{"..               | |_| |  _| |  _|                     ..\r\n"},
{"..               |  _  | |___| |___                    ..\r\n"},
{"..               |_| |_|_____|_____|                   ..\r\n"},
{"..            __     _____   ___   ___                 ..\r\n"},
{"..            \\ \\   / / _ \\ / _ \\ / _ \\                ..\r\n"},
{"..             \\ \\ / / | | | | | | | | |               ..\r\n"},
{"..              \\ V /| |_| | |_| | |_| |               ..\r\n"},
{"..               \\_/  \\___(_)___(_)___/                ..\r\n"},
{".........................................................\r\n"},
{".........................................................\r\n"},                                                   
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//static void CMD_send_splash();

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
uint8_t g_RF_CMD_line_return = CMDLINE_OK;

tCmdLineEntry g_psCmdTable[] =
{
    { 0, 0, 0 }
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* :::::::::: CMD Line Task Init :::::::: */
void Task_CMD_Line_Init(void)
{
    task_cmd_line.p_buffer         = g_task_cmd_line_buffer;
    task_cmd_line.buffer_size      = 64;
    task_cmd_line.write_index      = 0;

    if(task_cmd_line.buffer_size != 0)
    {
        memset((void *)task_cmd_line.p_buffer, 0, sizeof(task_cmd_line.p_buffer));
    }
}

/* :::::::::: CMD Line Task ::::::::::::: */
void Task_CMD_Line(void *pvParameters)
{
    uint8_t time_out;
    const TickType_t delay_period = pdMS_TO_TICKS(50);   // 50 ms
          TickType_t last_delay;

for(;;)
{
    last_delay = xTaskGetTickCount();

    for(time_out = 50; (!TASK_CMD_LINE_RX_EMPTY()) && (time_out != 0); time_out--)
    {
        task_cmd_line.RX_char = (char)TASK_CMD_LINE_GET_CHAR();
        
        if(((task_cmd_line.RX_char == 8) || (task_cmd_line.RX_char == 127)))
        {
            if (task_cmd_line.write_index == 0)
                break;

            task_cmd_line.write_index--;
            TASK_CMD_LINE_SEND_CHAR(task_cmd_line.RX_char);
            break;
        }

        TASK_CMD_LINE_SEND_CHAR(task_cmd_line.RX_char);

        if((task_cmd_line.RX_char == '\r') || (task_cmd_line.RX_char == '\n'))
        {
            if(task_cmd_line.write_index > 0)
            {
                // Add a NUL char at the end of the CMD
                task_cmd_line.p_buffer[task_cmd_line.write_index] = 0;
                //task_cmd_line.write_index++;

                // CMD_line_handle   = &RF_UART;
                g_RF_CMD_line_return = CmdLineProcess(task_cmd_line.p_buffer);

                // if (g_RF_CMD_line_return == CMDLINE_IS_PROCESSING)
                // {
				// 	return;
				// }

                task_cmd_line.write_index = 0;

                TASK_CMD_LINE_SEND_STRING("> ");
				TASK_CMD_LINE_SEND_STRING(ErrorCode[g_RF_CMD_line_return]);

                TASK_CMD_LINE_SEND_STRING("> ");
            }
            else
            {
                TASK_CMD_LINE_SEND_STRING("> ");
            }
        }
        else
        {
            task_cmd_line.p_buffer[task_cmd_line.write_index] = task_cmd_line.RX_char;
            task_cmd_line.write_index++;

            if (task_cmd_line.write_index > task_cmd_line.buffer_size)
            {
                // SDKLFJSDFKS
                // > CMD too long!
                // > 
                TASK_CMD_LINE_SEND_STRING("\n> CMD too long!\n> ");
                //task_cmd_line.write_index = task_cmd_line.read_index;
                task_cmd_line.write_index    = 0;
            }
        }
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
