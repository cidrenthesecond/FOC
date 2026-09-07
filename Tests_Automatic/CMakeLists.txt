cmake_minimum_required(VERSION 3.10)
project(LightScheduler_TEST)

set(CMAKE_C_STANDARD 99)
add_definitions(-DUNITY_FIXTURE_NO_EXTERN)

add_compile_options(-Wall -Wextra)

# Ścieżka bazowa do Twojego folderu Unity
set(MAIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(UNITY_DIR ${MAIN_DIR}/Unity)
set(CMSIS_DIR ${MAIN_DIR}/lib/CMSIS-DSP/Source)
set(MOCK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Mocks)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Definiujemy bibliotekę Unity
add_library(unity_lib STATIC 
    ${UNITY_DIR}/src/unity.c
    ${UNITY_DIR}/extras/fixture/src/unity_fixture.c
    ${UNITY_DIR}/extras/memory/src/unity_memory.c
)

# # 2. DODAJEMY WSZYSTKIE ŚCIEŻKI DO NAGŁÓWKÓW
target_include_directories(unity_lib PUBLIC 
    ${UNITY_DIR}/src                         # Dla unity.h
    ${UNITY_DIR}/extras/fixture/src          # Dla unity_fixture.h
    ${UNITY_DIR}/extras/memory/src           # DLA UNITY_MEMORY.H - To naprawia Twój błąd
)

# Add the CMSIS-DSP library from the relative path
add_subdirectory(
    "${MAIN_DIR}/lib/CMSIS-DSP/Source"
    "${CMAKE_BINARY_DIR}/CMSIS-DSP"
)

add_subdirectory(Mocks/LOG_Spy)
add_subdirectory(Mocks/GPIO_Mock)

add_subdirectory(Relay)
add_subdirectory(PID)
add_subdirectory(FIFO)
add_subdirectory(Logger)
add_subdirectory(CMSIS)
add_subdirectory(Thermistor)
add_subdirectory(WaveGen)
add_subdirectory(ScalarProfile)
add_subdirectory(Timer)

# Add the CMSIS-DSP library from the relative path
#add_subdirectory(${CMSIS_DIR})

# target_include_directories(run_tests PRIVATE
#     Inc
#     Mocks
#     ${CMAKE_SOURCE_DIR}/lib/CMSIS-DSP/Include
#     ${CMAKE_SOURCE_DIR}/lib/CMSIS-DSP/PrivateInclude
# )

# # Łączymy wszystko
# target_link_libraries(run_tests PRIVATE
#     unity_lib
#     CMSISDSP
# )


