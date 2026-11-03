set(TARGET_PLATFORM a53_renode)
set(OPENA64_DIR ${CMAKE_CURRENT_LIST_DIR}/../../.. CACHE STRING "")
set(OPENA64_BSP_DIR ${OPENA64_DIR}/bsp)
set(OPENA64_DRIVERS_DIR ${OPENA64_DIR}/drivers)
set(LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/linker.ld")

cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE ${OPENA64_DIR}/cmake/arm-none-eabi.cmake CACHE STRING "")

project(armv8_32bit_exec LANGUAGES C ASM)

add_executable(firmware_arm32
    ./main.c
    ./startup.S
    ${OPENA64_BSP_DIR}/common/syscalls.c
    ${OPENA64_DRIVERS_DIR}/opena64/uart_pl011.c
)

target_include_directories(firmware_arm32
  PRIVATE
    ${OPENA64_DRIVERS_DIR}
)

set_target_properties(firmware_arm32 PROPERTIES
    OUTPUT_NAME firmware_arm32
    SUFFIX ".elf"
)

target_create_bin(firmware_arm32)

target_objdump(firmware_arm32)
