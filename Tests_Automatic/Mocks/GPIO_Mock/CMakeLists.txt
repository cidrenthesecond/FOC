add_executable(test_gpio_mock

    AllTests.c

    GPIO_Mock_Test.c
    GPIO_Mock_TestRunner.c
    GPIO_Mock.c
)

target_include_directories(test_gpio_mock PRIVATE
    ${MAIN_DIR}/Common/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(test_gpio_mock PRIVATE
    unity_lib
)

add_custom_command(TARGET test_gpio_mock POST_BUILD

    COMMAND test_gpio_mock -v

    COMMENT "Running GPIO_Mock tests..."

)