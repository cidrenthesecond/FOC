add_library(SysTickDispatcher STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/SysTickDispatcher.c
)

target_include_directories(SysTickDispatcher
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)