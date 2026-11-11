add_library(PID STATIC
    Src/PID.c
)

target_include_directories(PID
    PUBLIC
        Inc
)