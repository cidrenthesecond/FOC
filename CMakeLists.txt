cmake_minimum_required(VERSION 3.10)
project(LightScheduler_TEST)

set(CMAKE_C_STANDARD 99)
add_definitions(-DUNITY_FIXTURE_NO_EXTERN)

add_compile_options(-Wall -Wextra)

# Ścieżka bazowa do Twojego folderu Unity
set(UNITY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Unity)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Definiujemy bibliotekę Unity
add_library(unity_lib STATIC 
    ${UNITY_DIR}/src/unity.c
    ${UNITY_DIR}/extras/fixture/src/unity_fixture.c
    ${UNITY_DIR}/extras/memory/src/unity_memory.c
)

# 2. DODAJEMY WSZYSTKIE ŚCIEŻKI DO NAGŁÓWKÓW
target_include_directories(unity_lib PUBLIC 
    ${UNITY_DIR}/src                         # Dla unity.h
    ${UNITY_DIR}/extras/fixture/src          # Dla unity_fixture.h
    ${UNITY_DIR}/extras/memory/src           # DLA UNITY_MEMORY.H - To naprawia Twój błąd
)

# 4. Twoje testy
add_executable(run_tests
    Tests/AllTest.c

    Tests/Relay_Test.c
    Tests/Relay_TestRunner.c
    Tests/PID_Test.c
    Tests/PID_TestRunner.c
    Tests/CMSIS_Test.c
    Tests/CMSIS_TestRunner.c
    Tests/Logger_Test.c
    Tests/Logger_TestRunner.c
    Tests/LogCircularBuffer_Test.c
    Tests/LogCircularBuffer_TestRunner.c
    Tests/SVPWM_Test.c
    Tests/SVPWM_TestRunner.c

    Src/Relay.c
    Src/PID.c
    Src/Logger.c
    Src/LogCircularBuffer.c
    Src/SVPWM.c

    Mocks/FakeDcLink.c
    Mocks/FakeDCLink_Test.c
    Mocks/FakeDcLink_TestRunner.c

    Mocks/FakeMicroTime.c
    Mocks/FakeMicroTime_Test.c
    Mocks/FakeMicroTime_TestRunner.c
)

# Add the CMSIS-DSP library from the relative path
add_subdirectory(lib/CMSIS-DSP/Source)

target_include_directories(run_tests PRIVATE
    Inc
    Mocks
    ${CMAKE_SOURCE_DIR}/lib/CMSIS-DSP/Include
    ${CMAKE_SOURCE_DIR}/lib/CMSIS-DSP/PrivateInclude
)

# Łączymy wszystko
target_link_libraries(run_tests PRIVATE
    unity_lib
    CMSISDSP
)


# 5. Automatyczne uruchamianie testów po kompilacji
add_custom_command(TARGET run_tests POST_BUILD
    COMMAND ./run_tests -v
    COMMENT "Automatyczne uruchamianie testow..."
)

