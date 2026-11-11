add_subdirectory(ControlSchemes)

add_library(Control_Task STATIC
    Src/Control_Task.c
)

target_include_directories(Control_Task
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

target_link_libraries(Control_Task
    PUBLIC
        quarkts
    PRIVATE
        PWM_Driver
        ControlSchemes
)