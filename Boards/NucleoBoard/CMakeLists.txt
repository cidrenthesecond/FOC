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

set(MAIN_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(BSP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bsp)
set(COMMON_DIR      ${MAIN_DIR}/Common)
set(DRIVERS_DIR     ${MAIN_DIR}/Drivers)
set(APP_DIR         ${MAIN_DIR}/App)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

#build mx generated files
add_subdirectory(cmake/stm32cubemx)
#build OS
add_subdirectory(${MAIN_DIR}/QuarkTS-7.3.3 quarkts_build)

#short workaround
add_library(PWM_Schemes STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/TODO/PWM_Schemes.c
)

target_include_directories(PWM_Schemes
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/TODO
)

target_link_libraries(PWM_Schemes
    PRIVATE
        stm32cubemx
)

#build common files
add_subdirectory(${COMMON_DIR} common_build)
#build drivers
add_subdirectory(${DRIVERS_DIR} drivers_build)
#build app related
add_subdirectory(${APP_DIR} app_build)

target_link_libraries(Relay PRIVATE GPIO_Driver)
# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
 #   ${CMAKE_CURRENT_SOURCE_DIR}/Tests/FOC_SCHEME/FOC_Scheme_Test.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
   ${CMAKE_CURRENT_SOURCE_DIR}/Tests/FOC_SCHEME
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    quarkts
    MainSM
    SysTickWrapper
    Logger
    ADC_Service
    GPIO_Driver
)

