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

#ifndef SUCHAI_CONFIG_H
#define	SUCHAI_CONFIG_H

/* Select one operating system */
/* #undef SCH_OS_LINUX */
#define SCH_OS_FREERTOS             2
/* #undef SCH_OS_SIM */
/* Select the correct architecture */
/* #undef X86 */
/* #undef RPI */
/* #undef ESP32 */
/* #undef NANOMIND */
#define STM32                       14
#define SCH_ARCH                    STM32  ///< Hardware port X86 | RPI | ESP32 | NANOMIND
//#define SCH_FP_STATIC
#define SCH_HAVE_MALLOC
/* Select the correct app */
/* #undef SCH_APP */

/* System debug configurations */
#define SCH_LOG_LEVEL               LOG_LVL_DEBUG  ///< LOG_LVL_INFO |  LOG_LVL_DEBUG
#define SCH_NAME                    "SUCHAI-FS"  ///< Project code name
#define SCH_DEVICE_ID               1  ///< Device unique ID
#define SCH_SW_VERSION              "0.0.3"  ///< Software version

/* General system settings, enable/disable core tasks */
#define SCH_CON_ENABLED 1
#define SCH_COMM_ENABLE 1
#define SCH_FP_ENABLED 1
#define SCH_HK_ENABLED 1
//#define SCH_HOOK_INIT
/* #undef SCH_HOOK_COMM */
#define SCH_WDT_PERIOD_MS           500  ///< taskWatchdog period in ms
#define SCH_MAX_WDT_TIMER           2  ///< Periods (SCH_WDT_PERIOD_MS) to send wdt_reset command
#define SCH_MAX_GND_WDT_TIMER       48*3600*2  ///< Periods (SCH_WDT_PERIOD_MS) to reset the OBC if the software watchdog was not clear by the groundstation
#define SCH_UART_BAUDRATE           (500000)  ///< UART baud rate for serial console
#define SCH_KISS_UART_BAUDRATE      (500000)  ///< UART baud rate for kiss communication
#define SCH_KISS_DEVICE             "/dev/ttyUSB0"  ///< Kiss device path

/* Communications system settings */
#define SCH_TRX_PORT_FILE            (9)   ///< Files port
#define SCH_TRX_PORT_TC              (10)  ///< Telecommands port
#define SCH_TRX_PORT_RPT             (11)  ///< Digirepeater port (resend packets)
#define SCH_TRX_PORT_CMD             (12)  ///< Commands port (execute console commands)
#define SCH_TRX_PORT_DBG             (13)  ///< Debug port, logs output
#define SCH_TRX_PORT_DBG_TM          (14)  ///< Debug telemetry port, logs frames
#define SCH_TRX_PORT_TM              (15)  ///< Telemetry port
#define SCH_TRX_PORT_APP             15  ///< Apps telemetries starting port
#define SCH_COMM_NODE                1  ///< Node address
#define SCH_COM_MAX_PACKETS          10  /// Max number of packets to transmit in a row before a small pause
#define SCH_COM_TX_DELAY_MS          3000  /// Delay (ms) between continuous transmissions
#define SCH_CSP_BUFFERS              100  ///< Number of available CSP buffers
#define SCH_CSP_SOCK_LEN             100  ///< Max number of packets in a connection queue
#define SCH_COMM_ZMQ_IN              "tcp://127.0.0.1:8001"  ///< CSP ZMQ In socket URI
#define SCH_COMM_ZMQ_OUT             "tcp://127.0.0.1:8002"  ///< CSP ZMQ Out socket URI
#define SCH_CSP_CONN_TIMEOUT         1000  ///< CSP connection accept timeout
#define SCH_CSP_READ_TIMEOUT         100  ///< CSP connection read timeout
#define SCH_CSP_ID			         9  ///< CSP connection read timeout

/* Data repository settings */
#define SCH_ST_RAM                   0
/* #undef SCH_ST_SQLITE */
/* #undef SCH_ST_POSTGRES */
/* #undef SCH_ST_FLASH */
/* #undef SCH_ST_FILE */
#define SCH_STORAGE_MODE             SCH_ST_RAM  ///< Status repository location. (0) RAM, (1) Single external.
#define SCH_STORAGE_TRIPLE_WR        0  ///< Status variable tripled writing enabled (0 | 1)
#define SCH_STORAGE_FILE             "/tmp/suchai.db"  ///< File to store the database, only if @SCH_STORAGE_MODE is SCH_ST_SQLITE
#define SCH_STORAGE_PGUSER           0  ///< PSQL User, only if @SCH_STORAGE_MODE is SCH_ST_POSTGRES
#define SCH_STORAGE_PGPASS           0  ///< PSQL password, only if @SCH_STORAGE_MODE is SCH_ST_POSTGRES
#define SCH_STORAGE_PGHOST           0  ///< PSQL host, only if @SCH_STORAGE_MODE is SCH_ST_POSTGRES
#define SCH_STORAGE_FLASH_SIZE       2621440  ///< Total size of the flash memory
#define SCH_STORAGE_BLOCK_SIZE       512  ///< Size of each memory block in flash storage (min. erase unit)
#define SCH_STORAGE_PAGE_SIZE        512  ///< Size of each memory page in flash storage (write unit)
#define SCH_FLASH_INIT_BLOCK         0  ///< Block from which to start writing in flash storage, allows to skip the first blocks
#define SCH_FLASH_INIT_MEMORY        SCH_FLASH_INIT_BLOCK*SCH_STORAGE_BLOCK_SIZE  ///< Initial address in flash storage
#define SCH_ST_FRAM_SIZE             32768  ///< Size of the FRAM memory
#define SCH_ST_FP_TLB_FRAM      ///< Flight Plan TLB is stored in FRAM (else stored in flash)

#define SCH_CMD_QUEUE_LEN            10  ///< Commands queue length
#define SCH_FP_MAX_ENTRIES           255  ///< Max number of flight plan entries
#define SCH_CMD_MAX_ENTRIES          255  ///< Max number of commands in the repository
#define SCH_SECTIONS_PER_PAYLOAD     10  ///< Memory blocks for storing each payload type TODO: Make configurable per payload

/**
 * Memory settings.
 *
 * Control the memory used by task stacks, static allocated buffers, etc.
 * Note that in FreeRTOS the stack size is measured in words not bytes, so the
 * final stack size depends on the architecture stack wide
 * (@see https://www.freertos.org/a00125.html)
 */
#define SCH_TASK_DEF_STACK            512  ///< Default task stack size in words
#define SCH_TASK_DIS_STACK            (128)  ///< Dispatcher task stack size in words
#define SCH_TASK_EXE_STACK            (5*SCH_TASK_DEF_STACK)  ///< Executer task stack size in words
#define SCH_TASK_WDT_STACK            (128)  ///< Watchdog task stack size in words
#define SCH_TASK_INI_STACK            (256)  ///< Init task stack size in words
#define SCH_TASK_COM_STACK            (512)  ///< Communications task stack size in words
#define SCH_TASK_FPL_STACK            (1024)  ///< Flight plan task stack size in words
#define SCH_TASK_CON_STACK            (1024)  ///< Console task stack size in words
#define SCH_TASK_CSP_STACK            (512)  ///< CSP route task stack size in words

#define SCH_BUFF_MAX_LEN              (256)  ///< General buffers max length in bytes
#define SCH_CMD_MAX_STR_PARAMS        (248)  ///< Limit for the parameters length
#define SCH_CMD_MAX_STR_NAME          (248)  ///< Limit for the length of the name of a command
#define SCH_CMD_MAX_STR_FORMAT        (248)  ///< Limit for the length of the format field of a command
#define SCH_CMD_MAX_ARG        (248)  ///< Limit for the length of the format field of a command

#define TEMPERATURE_DELTA			  50	// different temperature between pri and sec NTC
#define HOUSE_KEEPING_INTERVAL	1000

#endif //SUCHAI_CONFIG_H
