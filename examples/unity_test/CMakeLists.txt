cmake_minimum_required(VERSION 3.20)

set(TARGET_PLATFORM a53_renode)
set(OPENA64_DIR ${CMAKE_CURRENT_LIST_DIR}/../.. CACHE STRING "")
set(OPENA64_BSP_DIR ${OPENA64_DIR}/bsp)
set(OPENA64_DRIVERS_DIR ${OPENA64_DIR}/drivers)
set(LINKER_SCRIPT "${OPENA64_BSP_DIR}/${TARGET_PLATFORM}/aarch64/linker.ld")

set(CMAKE_TOOLCHAIN_FILE ${OPENA64_DIR}/cmake/aarch64-none-elf.cmake CACHE STRING "")

project(hello_world LANGUAGES C ASM)

message(STATUS "fetch unity from git")
FetchContent_Declare(
  unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG v2.7.0
)
message(STATUS "make unity available")
FetchContent_MakeAvailable(unity)

add_executable(firmware
    main.c
    # bsp
    ${OPENA64_BSP_DIR}/a53_renode/aarch64/el_exceptions.c
    ${OPENA64_BSP_DIR}/a53_renode/aarch64/startup.S
    ${OPENA64_BSP_DIR}/a53_renode/aarch64/irq_vector_tbl.c
    ${OPENA64_BSP_DIR}/common/syscalls.c
    # drivers
    ${OPENA64_DRIVERS_DIR}/opena64/uart_pl011.c
    ${OPENA64_DRIVERS_DIR}/opena64/gic_v3.c
    ${OPENA64_DRIVERS_DIR}/opena64/timer.c
)

target_compile_definitions(firmware
  PUBLIC
  GIC_V3_ENABLE=1
)

target_include_directories(firmware
  PRIVATE
    ${OPENA64_DRIVERS_DIR}
    ${OPENA64_BSP_DIR}/a53_renode/aarch64
)

set_target_properties(firmware PROPERTIES
    OUTPUT_NAME firmware
    SUFFIX ".elf"
)

target_create_bin(firmware)

target_objdump(firmware)
