add_library(CommandExecute STATIC
    Src/CommandExecute.c
)

target_include_directories(CommandExecute
    PUBLIC
        Inc
)