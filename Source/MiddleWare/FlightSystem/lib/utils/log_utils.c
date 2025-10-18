/*
 * Copyright 2025 SUCHAI Flight Software v2 project and contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "log_utils.h"

#include "printf.h"

int64_t dat_get_time(void);  // Forward declaration


osSemaphore log_mutex;  ///< Sync logging functions, require initialization
void (*log_function)(const char *lvl, const char *tag, const char *msg, ...);
log_level_t log_lvl;
uint8_t log_node;
static char log_file_name[SCH_BUFF_MAX_LEN];

void log_print(const char *lvl, const char *tag, const char *msg, ...)
{
//    va_list args;
//    va_start(args, msg);
//    fprintf(LOGOUT,"[%s][%lu][%s] ", lvl, (unsigned long)dat_get_time(), tag);
//    vfprintf(LOGOUT, msg, args);
//    fprintf(LOGOUT,CRLF); fflush(LOGOUT);
//    va_end(args);


	va_list args;                    // Khai báo ở đầu
	    va_start(args, msg);             // Khởi tạo va_list

	    printf("[%s][%lu][%s] ", lvl, (unsigned long)dat_get_time(), tag);
	    vprintf(msg, args);              // DÙNG vprintf thay vì printf(msg, args)
	    printf(CRLF);                    // Hoặc CRLF nếu define

	    va_end(args);                    // BẮT BUỘC dọn dẹp

}

void log_send(const char *lvl, const char *tag, const char *msg, ...)
{
#if SCH_COMM_ENABLE
    // Create a packet for the log message
    csp_packet_t *packet = csp_buffer_get(SCH_BUFF_MAX_LEN);
    if(packet == NULL)
        return;

    // Clean the buffer (csp buffers are reused and contain trash)
    memset(packet->data, 0, SCH_BUFF_MAX_LEN);

    // Format message with variadic arguments
    va_list args;
    va_start(args, msg);
    int len = vsnprintf((char *)(packet->data), SCH_BUFF_MAX_LEN, msg, args);
    va_end(args);

    // Make sure its is a null terminating string
    packet->length = (uint16_t)len;
    packet->data[len] = '\0';

    // Sending message without connection nor reply.
    int rc = csp_sendto(CSP_PRIO_NORM, (uint8_t)log_node, SCH_TRX_PORT_DBG,
                        SCH_TRX_PORT_DBG, CSP_O_NONE, packet, 100);
    if(rc != 0)
        csp_buffer_free((void *)packet);
#endif
}

void log_file(const char *lvl, const char *tag, const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    char *fname = log_file_name;
    FILE *fptr = fopen(fname, "a");
    if(fptr != NULL)
    {
        fprintf(fptr,"[%s][%lu][%s] ", lvl, (unsigned long)dat_get_time(), tag);
        vfprintf(fptr, msg, args);
        fprintf(fptr, CRLF); fflush(fptr);
        fclose(fptr);
    }
    va_end(args);
}

void log_set(log_level_t level, int node)
{
    osSemaphoreTake(&log_mutex, portMAX_DELAY);
    log_lvl = level;
    if(node == 0)
        log_function = log_print;
    else if(node > 0)
    {
        log_function = log_send;
        log_node = (uint8_t) node;
    }
    else
    {
        snprintf(log_file_name, SCH_BUFF_MAX_LEN, "suchai_%d.log", -node);
        log_function = log_file;
    }

    osSemaphoreGiven(&log_mutex);
}

int log_init(log_level_t level, int node)
{
    int rc = osSemaphoreCreate(&log_mutex);
    log_set(level, node);
    return rc;
}
