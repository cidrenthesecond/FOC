cmake_minimum_required(VERSION 3.22)

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_PROJECT_NAME HIL)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

set(COMMON_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Src)
set(COMMON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Inc)
set(COMMON_TESTS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(UNITY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../Unity)

add_definitions(-DUNITY_FIXTURE_NO_EXTERN)

add_library(unity_lib STATIC 
    ${UNITY_DIR}/src/unity.c
    ${UNITY_DIR}/extras/fixture/src/unity_fixture.c
    ${UNITY_DIR}/extras/memory/src/unity_memory.c
)

target_include_directories(unity_lib PUBLIC 
    ${UNITY_DIR}/src                         # Dla unity.h
    ${UNITY_DIR}/extras/fixture/src          # Dla unity_fixture.h
    ${UNITY_DIR}/extras/memory/src           # DLA UNITY_MEMORY.H - To naprawia Twój błąd
)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

add_executable(ScalarProfile_Test)

target_sources(ScalarProfile_Test PRIVATE
    ${COMMON_SOURCE_DIR}/ScalarProfile.c
    ${COMMON_TESTS_SOURCE_DIR}/ScalarProfile/ScalarProfile_Test.c
    ${COMMON_TESTS_SOURCE_DIR}/ScalarProfile/ScalarProfile_TestsRunner.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/sysmem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/syscalls.c
)

target_include_directories(ScalarProfile_Test PRIVATE
    ${COMMON_INCLUDE_DIR}
)

target_link_libraries(ScalarProfile_Test
    stm32cubemx
    unity_lib
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

