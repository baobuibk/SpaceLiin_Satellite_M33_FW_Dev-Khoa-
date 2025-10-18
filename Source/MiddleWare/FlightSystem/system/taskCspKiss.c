/*
 * taskCspKiss.c
 *
 *  Created on: Oct 13, 2025
 *      Author: Admin
 */


#include "board.h"
#include "config.h"
#include "bsp_uart.h"
#include "csp/csp.h"
#include "osDelay.h"
#include "csp/csp_buffer.h"
#include "csp_init.h"
#include "csp/interfaces/csp_if_kiss.h"
#include "log_utils.h"



csp_kiss_interface_data_t uart_ifdata;
csp_iface_t uart_iface;

static const char *tag = "taskCspKiss";



static void uart_csp_init(void) {

	uart_ifdata.tx_func        = csp_uart_tx_frame;
	uart_iface.name            = "uart";
	uart_iface.driver_data     = USART6;
	uart_iface.interface_data  = &uart_ifdata;

	csp_kiss_add_interface(&uart_iface);
 }

int init_setup_libcsp(int node)
{
    /* Init communications */
    LOGI(tag, "Initialising CSP...");

    csp_debug_set_level(CSP_ERROR, 1);
    csp_debug_set_level(CSP_WARN, 1);
    csp_debug_set_level(CSP_INFO, 1);
    csp_debug_set_level(CSP_BUFFER, 0);
    csp_debug_set_level(CSP_PACKET, 1);
    csp_debug_set_level(CSP_PROTOCOL, 1);
    csp_debug_set_level(CSP_LOCK, 0);

    /* Init buffer system */
    int t_ok;


    /* Init CSP */
    csp_conf_t conf;
    csp_conf_get_defaults(&conf);
    conf.address = SCH_CSP_ID;
    t_ok = csp_init(&conf);
    if(t_ok != 0) LOGE(tag, "CSP init failed!");

    t_ok = csp_route_start_task(500, 6);

    csp_rtable_set(1, CSP_ID_HOST_SIZE, &uart_iface, CSP_NO_VIA_ADDRESS);

    if(t_ok != 0) LOGE(tag, "Task router not created!");

    /**
     * Add CSP INTERFACES in initAppHook, for example call to
     * csp_add_zmq_iface(SCH_COMM_NODE)
     */

    uart_csp_init();
    LOGI(tag, "CSP Node: %d", csp_get_address());
    return 0;
}

void taskCspKiss(void *param)
{
	uint8_t rxData;

    while(1)
    {
        osDelay(10);


        while(csp_getchar(&rxData) == 0)
        {
        	csp_kiss_rx(&uart_iface, &rxData, 1, NULL);
        }

}
}

