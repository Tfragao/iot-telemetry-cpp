include(FetchContent) 

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest) 

add_executable(iot_tests 
    test_telemetry.cpp 
    test_uart_protocol.cpp 
) 

target_link_libraries(iot_tests 
    PRIVATE 
        iot_core 
        GTest::gtest_main 
) 

target_link_libraries(iot_tests 
    PRIVATE 
        ${COMMON_WARNINGS}
) 

include(GoogleTest)
gtest_discover_tests(iot_tests)