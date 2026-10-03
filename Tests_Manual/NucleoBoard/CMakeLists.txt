cmake_minimum_required(VERSION 3.22)

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

add_compile_options(-Wall -Wextra)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME PWM_TEST)
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(MAIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(MAIN_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

add_subdirectory(cmake/stm32cubemx)
add_subdirectory(App/Logger)
add_subdirectory(App/CMD)
add_subdirectory(App/FPU)
add_subdirectory(App/WaveGen)
add_subdirectory(App/ScalarProfile)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${MAIN_DIR}/Common/Src/PID.c
    ${MAIN_DIR}/Common/Src/Relay.c
    ${MAIN_DIR}/Common/Src/Thermistor.c
    ${MAIN_DIR}/Common/Src/SysTickDispatcher.c

    ${CMAKE_CURRENT_SOURCE_DIR}/TODO/GPIO_Driver.c
    ${CMAKE_CURRENT_SOURCE_DIR}/TODO/ADC_Service.c
    ${CMAKE_CURRENT_SOURCE_DIR}/TODO/Moving_Avarage_Filter.c

    ${CMAKE_CURRENT_SOURCE_DIR}/TODO/SysTickWrapper.c

    ${CMAKE_CURRENT_SOURCE_DIR}/TODO/System_Config.c
    ${CMAKE_CURRENT_SOURCE_DIR}/TODO/Timer_Driver.c

    ${CMAKE_CURRENT_SOURCE_DIR}/Tests/FOC_SCHEME/FOC_Scheme_Test.c
    ${CMAKE_CURRENT_SOURCE_DIR}/TODO/PWM_Schemes.c

    ${CMAKE_CURRENT_SOURCE_DIR}/Schemes/ScalarControl.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${MAIN_DIR}/Common/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Tests/FOC_SCHEME
    ${CMAKE_CURRENT_SOURCE_DIR}/TODO
    ${CMAKE_CURRENT_SOURCE_DIR}/Schemes
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    logger
    commandLine
    FPU
    waveGen
    scalarProfile
)

