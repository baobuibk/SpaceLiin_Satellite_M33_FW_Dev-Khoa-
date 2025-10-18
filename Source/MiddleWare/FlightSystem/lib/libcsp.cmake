##
# Configure and build the LibCSP library for different platforms
##

# Define libcsp path
set(LIBCSP_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libcsp)
# Set the install directory depending on the architecture
set(LIBCSP_PREFIX ${LIBCSP_PATH}/libcsp/${SCH_ARCH} CACHE INTERNAL "LibCSP install prefix")

# Default options for libcsp with GNU/Linux
set(LIBCSP_OS posix)
set(LIBCSP_INCLUDES ${SCH_LIBCSP_INCLUDES}) # Read include directories from the CMake options
set(LIBCSP_OPTIONS "--with-max-connections=100;--with-conn-queue-length=1000;--with-router-queue-length=1000;--enable-if-zmqhub;--enable-if-kiss;--with-driver-usart=linux")
# Options for FreeRTOS ports, usually microcontrollers, so build a lighter version.
if (${SCH_OS} STREQUAL "FREERTOS")
    set(LIBCSP_OS freertos)
    set(LIBCSP_OPTIONS "--with-max-connections=20;--with-conn-queue-length=20;--with-router-queue-length=20")
endif ()

# Define the command to build the csp with waf
set(LIBCSP_BUILD_CMD python3 waf configure --with-os=${LIBCSP_OS} --includes=${LIBCSP_INCLUDES} ${LIBCSP_OPTIONS} --enable-crc32 --with-rtable=cidr --install-csp --prefix=${LIBCSP_PREFIX} build install)

# Add a custom command to build the LibCSP library if it doesn't exist. (use target_link_libraries(<target> INTERFACE ${LIBCSP_PREFIX}/lib/libcsp.a))
add_custom_command(
        OUTPUT "${LIBCSP_PREFIX}/lib/libcsp.a"
        COMMAND ${CMAKE_COMMAND} -E env CC=${CMAKE_C_COMPILER} AR=${CMAKE_C_COMPILER_AR} CFLAGS=${CMAKE_C_FLAGS} ${LIBCSP_BUILD_CMD}
        WORKING_DIRECTORY "${LIBCSP_PATH}"
        COMMENT "Building LibCSP library..."
)

# Add a custom target to add as a dependency to the main project (use add_dependencies(<target> libcsp))
add_custom_target(libcsp ALL DEPENDS "${LIBCSP_PREFIX}/lib/libcsp.a")
