cmake_minimum_required(VERSION 3.28.3)

project(gantt_chart C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

add_executable(gantt_chart
    src/main.c
    src/draw.c
    src/read.c
    src/task.c
)

target_include_directories(gantt_chart
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
