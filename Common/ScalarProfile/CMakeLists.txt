add_library(ScalarProfile STATIC
    Src/ScalarProfile.c
)

target_include_directories(ScalarProfile
    PUBLIC
        Inc
)