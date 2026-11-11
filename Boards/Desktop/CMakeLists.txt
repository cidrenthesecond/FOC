cmake_minimum_required(VERSION 3.10)
project(Desktop_Manual_Tests)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -Wextra)

set(COMMON_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Src)
set(COMMON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Inc)

add_subdirectory(ScalarProfile)