cmake_minimum_required(VERSION 3.20)

project(
    iot_telemetry_cpp 
    VERSION 1.0 
    DESCRIPTION "Embedded IoT telemetry simulator using modern C++20"
    LANGUAGES CXX 
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_INCLUDE_DIR ${PORJECT_SOURCE_DIR}/include)

set(COMMON_WARNINGS
    -Wall 
    -Wextra 
    -Wpedantic 
    -Wconversion 
    -Wshadow
)

add_library(iot_core 
    src/app.cpp 
    src/sensor.cpp 
    src/telemetry.cpp 
    src/logger.cpp 
    src/menu.cpp
)

target_include_directories(iot_core
    PUBLIC 
        ${PROJECT_INCLUDE_DIR}
)

target_compile_options(iot_core
    PRIVATE 
        ${COMMON_WARNINGS}
)

add_executable(iot_telemetry_app
    src/main.cpp 
)

target_link_libraries(iot_telemetry_app
    PRIVATE 
        iot_core 
)

target_compile_options(iot_telemetry_app 
    PRIVATE 
        ${COMMON_WARNINGS}
) 

enable_testing()
