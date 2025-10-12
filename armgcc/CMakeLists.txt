# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)

# THE VERSION NUMBER
SET (MCUXPRESSO_CMAKE_FORMAT_MAJOR_VERSION 2)
SET (MCUXPRESSO_CMAKE_FORMAT_MINOR_VERSION 0)

if(CMAKE_SCRIPT_MODE_FILE)
  message("${MCUXPRESSO_CMAKE_FORMAT_MAJOR_VERSION}")
  return()
endif()


# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})

SET(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})


project(satelite_exp_v100)

set(MCUX_BUILD_TYPES debug release)

set(MCUX_SDK_PROJECT_NAME satelite_exp_v100.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

# USER DEFINE DEV DIRECTORY
set(UserSourceDirPath ${ProjDirPath}/../Source)

add_executable(${MCUX_SDK_PROJECT_NAME}
    # main.c
    "${UserSourceDirPath}/main.c"

    # FreeRTOS
    "${UserSourceDirPath}/MiddleWare/FreeRTOS/FreeRTOSConfig.h"

    # MCUXpresso Autogen
    "${UserSourceDirPath}/BoardSupport/autogen/board.c"
    "${UserSourceDirPath}/BoardSupport/autogen/board.h"
    "${UserSourceDirPath}/BoardSupport/autogen/pin_mux.c"
    "${UserSourceDirPath}/BoardSupport/autogen/pin_mux.h"
    "${UserSourceDirPath}/BoardSupport/autogen/clock_config.c"
    "${UserSourceDirPath}/BoardSupport/autogen/clock_config.h"

    # User define
    # App
    "${UserSourceDirPath}/App/app.c"
    "${UserSourceDirPath}/App/app.h"

    "${UserSourceDirPath}/App/task/task_common.h"

    "${UserSourceDirPath}/App/task/task_cmd_line/task_cmd_line.c"
    "${UserSourceDirPath}/App/task/task_cmd_line/task_cmd_line.h"

    "${UserSourceDirPath}/App/task/task_cmd_line/command/command.c"
    "${UserSourceDirPath}/App/task/task_cmd_line/command/command.h"

    "${UserSourceDirPath}/App/task/task_cmd_line/command/cmd_test_can/cmd_test_can.c"
    "${UserSourceDirPath}/App/task/task_cmd_line/command/cmd_test_can/cmd_test_can.h"

    "${UserSourceDirPath}/App/task/task_cmd_line/command/cmd_test_spi/cmd_test_spi.c"
    "${UserSourceDirPath}/App/task/task_cmd_line/command/cmd_test_spi/cmd_test_spi.h"

    "${UserSourceDirPath}/App/task/task_test_can/task_test_can.c"
    "${UserSourceDirPath}/App/task/task_test_can/task_test_can.h"

    # BoardSupport
    "${UserSourceDirPath}/BoardSupport/bsp.c"
    "${UserSourceDirPath}/BoardSupport/bsp.h"
    
    "${UserSourceDirPath}/BoardSupport/bsp_board/bsp_board.h"

    "${UserSourceDirPath}/BoardSupport/bsp_core/bsp_core.c"
    "${UserSourceDirPath}/BoardSupport/bsp_core/bsp_core.h"

    "${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_debug_console/bsp_debug_console.c"
    "${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_debug_console/bsp_debug_console.h"

    "${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_libcsp_can/bsp_libcsp_can.c"
    "${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_libcsp_can/bsp_libcsp_can.h"

    "${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_spi_sensor/bsp_spi_sensor.c"
    "${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_spi_sensor/bsp_spi_sensor.h"

    # Component
    "${UserSourceDirPath}/Component/error_code/error_code.h"

    "${UserSourceDirPath}/Component/ring_buffer/ring_buffer.c"
    "${UserSourceDirPath}/Component/ring_buffer/ring_buffer.h"

    "${UserSourceDirPath}/Component/ring_char_buffer/ring_char_buffer.c"
    "${UserSourceDirPath}/Component/ring_char_buffer/ring_char_buffer.h"

    "${UserSourceDirPath}/Component/sp_gpio/sp_gpio.h"
    "${UserSourceDirPath}/Component/sp_gpio/port/sp_gpio_api.h"
    "${UserSourceDirPath}/Component/sp_gpio/port/NXP/sp_gpio_port.c"
    "${UserSourceDirPath}/Component/sp_gpio/port/NXP/sp_gpio_port.h"

    "${UserSourceDirPath}/Component/uart_stdio/uart_stdio.c"
    "${UserSourceDirPath}/Component/uart_stdio/uart_stdio.h"

    "${UserSourceDirPath}/Component/sp_uart/sp_uart.h"
    "${UserSourceDirPath}/Component/sp_uart/port/sp_uart_api.h"
    "${UserSourceDirPath}/Component/sp_uart/port/NXP/sp_uart_port.c"
    "${UserSourceDirPath}/Component/sp_uart/port/NXP/sp_uart_port.h"

    "${UserSourceDirPath}/Component/sp_spi/sp_spi.h"
    "${UserSourceDirPath}/Component/sp_spi/port/sp_spi_api.h"
    "${UserSourceDirPath}/Component/sp_spi/port/NXP/sp_spi_port.c"
    "${UserSourceDirPath}/Component/sp_spi/port/NXP/sp_spi_port.h"

    "${UserSourceDirPath}/Component/can_stdio/can_stdio.c"
    "${UserSourceDirPath}/Component/can_stdio/can_stdio.h"

    "${UserSourceDirPath}/Component/cmd_line/cmd_line.c"
    "${UserSourceDirPath}/Component/cmd_line/cmd_line.h"

    # "${UserSourceDirPath}/Component/devices/lt8722/lt8722.c"
    # "${UserSourceDirPath}/Component/devices/lt8722/lt8722.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..

    # User define
    # App
    ${UserSourceDirPath}/App
    ${UserSourceDirPath}/App/task
    ${UserSourceDirPath}/App/task/task_cmd_line
    ${UserSourceDirPath}/App/task/task_cmd_line/command
    ${UserSourceDirPath}/App/task/task_cmd_line/command/cmd_test_can
    ${UserSourceDirPath}/App/task/task_cmd_line/command/cmd_test_spi

    ${UserSourceDirPath}/App/task/task_test_can

    # BoardSupport
    ${UserSourceDirPath}/BoardSupport
    ${UserSourceDirPath}/BoardSupport/autogen

    ${UserSourceDirPath}/BoardSupport/bsp_board

    ${UserSourceDirPath}/BoardSupport/bsp_core

    ${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_debug_console
    ${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_libcsp_can
    ${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_spi_sensor

    # Component
    ${UserSourceDirPath}/Component/error_code
    ${UserSourceDirPath}/Component/ring_buffer
    ${UserSourceDirPath}/Component/ring_char_buffer

    ${UserSourceDirPath}/Component/sp_gpio
    ${UserSourceDirPath}/Component/sp_gpio/port
    ${UserSourceDirPath}/Component/sp_gpio/port/NXP

    ${UserSourceDirPath}/Component/sp_spi
    ${UserSourceDirPath}/Component/sp_spi/port
    ${UserSourceDirPath}/Component/sp_spi/port/NXP

    ${UserSourceDirPath}/Component/uart_stdio
    ${UserSourceDirPath}/Component/can_stdio
    ${UserSourceDirPath}/Component/cmd_line

    ${UserSourceDirPath}/Component/sp_uart
    ${UserSourceDirPath}/Component/sp_uart/port
    ${UserSourceDirPath}/Component/sp_uart/port/NXP

    # ${UserSourceDirPath}/Component/devices/lt8722

    # MiddleWare
    ${UserSourceDirPath}/MiddleWare/FreeRTOS
)

set_source_files_properties("${UserSourceDirPath}/MiddleWare/FreeRTOS/FreeRTOSConfig.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_freertos-kernel_template")

include(${SdkRootDirPath}/devices/MIMX9352/all_lib_device.cmake)

IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)

