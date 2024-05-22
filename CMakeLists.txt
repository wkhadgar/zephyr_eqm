cmake_minimum_required(VERSION 3.20.0)

set(BOARD_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(BOARD "stm32_min_dev@blue")

find_package(Zephyr)
project(zephyr_eqm)
add_link_options(--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)

include_directories(BEFORE src/)

target_sources(app PRIVATE
        src/main.c
        src/app/target_controller.c
        src/driver/stepper.c
        src/infra/gnss.c
        src/infra/lst.c
)
