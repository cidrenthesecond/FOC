add_library(FakeConfig STATIC
    Src/FakeConfig.c
)

target_include_directories(FakeConfig
    PUBLIC
        inc
)

target_link_libraries(FakeConfig
    PUBLIC
        ConfigInterface
)