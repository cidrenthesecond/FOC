add_library(gpio_mock STATIC
    GPIO_Mock.c
)

target_include_directories(gpio_mock
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(gpio_mock 
    PUBLIC
        GPIO_Interface
)

add_executable(test_gpio_mock
    AllTests.c
    GPIO_Mock_Test.c
    GPIO_Mock_TestRunner.c
)

target_link_libraries(test_gpio_mock PRIVATE
    test_harness
    gpio_mock
)

add_custom_command(TARGET test_gpio_mock POST_BUILD

    COMMAND test_gpio_mock -v

    COMMENT "Running GPIO_Mock tests..."

)