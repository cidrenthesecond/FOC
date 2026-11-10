add_library(ConfigManager STATIC
    Src/ConfigManager.c
)

target_include_directories(ConfigManager
PUBLIC
    Inc
)
