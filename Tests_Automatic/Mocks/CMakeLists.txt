add_subdirectory(GPIO_Mock)
add_subdirectory(LOG_Spy)
add_subdirectory(FakeConfig)

add_library(Mocks INTERFACE)

target_link_libraries(Mocks
    INTERFACE
        gpio_mock
        LOG_Spy
        FakeConfig
)