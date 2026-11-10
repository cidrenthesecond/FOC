cmake_minimum_required(VERSION 3.10)
project(LightScheduler_TEST)

set(CMAKE_C_STANDARD 99)
add_definitions(-DUNITY_FIXTURE_NO_EXTERN)

add_compile_options(-Wall -Wextra)
add_link_options(--trace)

# Ścieżka bazowa do Twojego folderu Unity
set(MAIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(CMSIS_DIR ${MAIN_DIR}/lib/CMSIS-DSP/Source)
set(MOCK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Mocks)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(unity_lib STATIC 
    Unity/src/unity.c
    Unity/extras/fixture/src/unity_fixture.c
    Unity/extras/memory/src/unity_memory.c
)

target_include_directories(unity_lib PUBLIC 
    Unity/src
    Unity/extras/fixture/src 
    Unity/extras/memory/src
)

add_library(fff_lib INTERFACE)

target_include_directories(fff_lib
    INTERFACE
        fff
)

add_library(test_harness INTERFACE)

target_link_libraries(test_harness
    INTERFACE
        unity_lib
        fff_lib
)

# Add the CMSIS-DSP library from the relative path
add_subdirectory(
    "${MAIN_DIR}/lib/CMSIS-DSP/Source"
    "${CMAKE_BINARY_DIR}/CMSIS-DSP"
)

#UUT builds
add_subdirectory(${MAIN_DIR}/Absolete/SysTickDispatcher SysTickDispatcher_build)
add_subdirectory(${MAIN_DIR}/Common/FIFO FIFO_build)
add_subdirectory(${MAIN_DIR}/Common/Logger Logger_build)
add_subdirectory(${MAIN_DIR}/Common/PID PID_build)


add_subdirectory(Mocks/GPIO_Mock)
add_subdirectory(Mocks/LOG_Spy)
add_subdirectory(Relay)
add_subdirectory(PID)
add_subdirectory(FIFO)
add_subdirectory(Logger)
add_subdirectory(CMSIS)
add_subdirectory(Thermistor)
add_subdirectory(WaveGen)
add_subdirectory(ScalarProfile)
add_subdirectory(SysTickDispatcher)
add_subdirectory(CommandExecute)
add_subdirectory(MainStateMachine)
add_subdirectory(ConfigManager)
#add_subdirectory(QuarkTS)


