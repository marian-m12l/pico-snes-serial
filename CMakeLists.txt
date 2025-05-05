cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include(pico_sdk_import.cmake)

project(pico-snes-serial C CXX ASM)

pico_sdk_init()


add_executable(pico-snes-serial
    main.c
)

pico_generate_pio_header(pico-snes-serial ${CMAKE_CURRENT_LIST_DIR}/joypad.pio)

pico_set_program_name(pico-snes-serial "pico-snes-serial")
pico_set_program_version(pico-snes-serial "0.1")

pico_enable_stdio_uart(pico-snes-serial 0)
pico_enable_stdio_usb(pico-snes-serial 1)

target_link_libraries(pico-snes-serial pico_stdlib hardware_pio)

pico_add_extra_outputs(pico-snes-serial)
