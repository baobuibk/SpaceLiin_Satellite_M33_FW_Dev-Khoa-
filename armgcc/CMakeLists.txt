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

    # Component
    "${UserSourceDirPath}/Component/error_code/error_code.h"

    "${UserSourceDirPath}/Component/ring_buffer/ring_buffer.c"
    "${UserSourceDirPath}/Component/ring_buffer/ring_buffer.h"

    "${UserSourceDirPath}/Component/ring_char_buffer/ring_char_buffer.c"
    "${UserSourceDirPath}/Component/ring_char_buffer/ring_char_buffer.h"

    "${UserSourceDirPath}/Component/uart_stdio/uart_stdio.c"
    "${UserSourceDirPath}/Component/uart_stdio/uart_stdio.h"

    "${UserSourceDirPath}/Component/can_stdio/can_stdio.c"
    "${UserSourceDirPath}/Component/can_stdio/can_stdio.h"

    "${UserSourceDirPath}/Component/cmd_line/cmd_line.c"
    "${UserSourceDirPath}/Component/cmd_line/cmd_line.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..

    # User define
    # App
    ${UserSourceDirPath}/App
    ${UserSourceDirPath}/App/task
    ${UserSourceDirPath}/App/task/task_cmd_line

    # BoardSupport
    ${UserSourceDirPath}/BoardSupport
    ${UserSourceDirPath}/BoardSupport/autogen

    ${UserSourceDirPath}/BoardSupport/bsp_board

    ${UserSourceDirPath}/BoardSupport/bsp_core

    ${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_debug_console
    ${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_libcsp_can

    # Component
    ${UserSourceDirPath}/Component/error_code
    ${UserSourceDirPath}/Component/ring_buffer
    ${UserSourceDirPath}/Component/ring_char_buffer
    ${UserSourceDirPath}/Component/uart_stdio
    ${UserSourceDirPath}/Component/can_stdio
    ${UserSourceDirPath}/Component/cmd_line

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

