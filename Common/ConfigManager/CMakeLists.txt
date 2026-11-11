add_library(ConfigInterface INTERFACE)

target_include_directories(ConfigInterface
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/ConfigInterface
)

add_library(ConfigManager STATIC
    Src/ConfigManager.c
)

target_include_directories(ConfigManager
    PUBLIC
        Inc
)

target_link_libraries(ConfigManager
    PUBLIC
        ConfigInterface
)
