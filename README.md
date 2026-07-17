# IoT Telemetry Simulator in Modern C++20

A small embedded/IoT-style telemetry simulator written in Modern C++20.

The goal of this project is to practice professional C++ development for embedded Linux and IoT systems, including modular design, command-line configuration, UART-style data parsing, telemetry generation, CSV logging, unit testing, and CI automation.

## Project Overview

The application simulates an embedded IoT gateway that collects sensor data, evaluates device status, and logs telemetry data.

It currently supports multiple sensor input modes:

- **Fake mode**: generates simulated sensor readings
- **System mode**: reads system information from Linux when available
- **UART mode**: reads telemetry lines from a serial-like interface

The project is designed to evolve gradually toward a more complete embedded Linux telemetry gateway.

## Features

- Modern C++20 project structure
- CMake-based build system
- Source/header separation
- Layered project architecture
- Command-line argument parsing
- Multiple sensor reader implementations
- Polymorphic sensor reader interface
- UART protocol parsing
- CSV telemetry logging
- Basic device status evaluation
- Unit tests with GoogleTest
- Test execution with CTest
- GitHub Actions CI build and test workflow

## Technologies Used

- C++20
- CMake
- Ninja
- GoogleTest
- CTest
- GitHub Actions
- Linux/POSIX APIs
- UART-style text protocol
- CSV logging

## Project Structure

```text
iot-telemetry-cpp/
├── .github/
│   └── workflows/
│       └── cmake.yml
├── docs/
│   └── debugging-walkthrough.md
├── include/
│   └── iot/
│       ├── app.hpp
│       ├── config.hpp
│       ├── fake_sensor_reader.hpp
│       ├── logger.hpp
│       ├── sensor.hpp
│       ├── sensor_reader.hpp
│       ├── system_sensor_reader.hpp
│       ├── telemetry.hpp
│       ├── uart_protocol.hpp
│       └── uart_sensor_reader.hpp
├── src/
│   ├── app.cpp
│   ├── config.cpp
│   ├── fake_sensor_reader.cpp
│   ├── logger.cpp
│   ├── main.cpp
│   ├── sensor.cpp
│   ├── system_sensor_reader.cpp
│   ├── telemetry.cpp
│   ├── uart_protocol.cpp
│   └── uart_sensor_reader.cpp
├── tests/
│   ├── CMakeLists.txt
│   ├── test_telemetry.cpp
│   └── test_uart_protocol.cpp
├── CMakeLists.txt
├── CMakePresets.json
└── README.md
```

## Build

Configure the project:

```bash
cmake --preset debug
```

Build the project:

```bash
cmake --build --preset debug
```

The application binary is generated at:

```text
build/debug/iot_telemetry_app
```

## Run

### Fake Sensor Mode

```bash
./build/debug/iot_telemetry_app \
  --mode fake \
  --samples 5 \
  --log data/fake.csv \
  --verbose
```

### System Sensor Mode

```bash
./build/debug/iot_telemetry_app \
  --mode system \
  --samples 5 \
  --log data/system.csv \
  --verbose
```

### UART Mode

```bash
./build/debug/iot_telemetry_app \
  --mode uart \
  --port /dev/ttyUSB0 \
  --baud 115200 \
  --samples 5 \
  --log data/uart.csv \
  --verbose
```

## UART Protocol

UART mode expects one telemetry message per line.

Example:

```text
TEMP=25.4,HUM=51.2,VOLT=3.3,DI0=1,DI1=0,DI2=1,DI3=0
```

The parser also supports spaces around fields:

```text
TEMP=25.4, HUM=51.2, VOLT=3.3, DI0=1, DI1=0, DI2=1, DI3=0
```

Supported fields:

| Field | Description |
|---|---|
| `TEMP` | Temperature in Celsius |
| `HUM` | Humidity percentage |
| `VOLT` | Voltage in volts |
| `DI0` | Digital input 0 |
| `DI1` | Digital input 1 |
| `DI2` | Digital input 2 |
| `DI3` | Digital input 3 |

## Testing

This project uses GoogleTest and CTest.

Build the project first:

```bash
cmake --preset debug
cmake --build --preset debug
```

Run all tests:

```bash
ctest --test-dir build/debug/tests --output-on-failure
```

Run the test binary directly:

```bash
./build/debug/tests/iot_tests
```

Current test coverage includes:

- Telemetry status evaluation
- UART protocol parsing
- Valid UART lines
- UART lines with spaces
- Invalid UART lines with missing required fields

## CI

GitHub Actions is used to validate the project automatically.

The CI workflow currently checks:

- CMake configuration
- Project build
- Unit tests with CTest
- Fake mode smoke test
- System mode smoke test

## Architecture

The project follows a simple layered architecture:

```text
Application Layer
    main.cpp
    app.cpp

Configuration Layer
    config.hpp
    config.cpp

Domain / Core Layer
    sensor.hpp / sensor.cpp
    telemetry.hpp / telemetry.cpp

Interface Layer
    sensor_reader.hpp

Adapter Layer
    fake_sensor_reader.hpp / .cpp
    system_sensor_reader.hpp / .cpp
    uart_sensor_reader.hpp / .cpp
    uart_protocol.hpp / .cpp
    logger.hpp / logger.cpp

Platform Layer
    Linux filesystem
    POSIX UART APIs
    termios
    CSV files
```

The sensor acquisition logic is based on a common interface:

```cpp
class ISensorReader
{
public:
    virtual ~ISensorReader() = default;
    virtual SensorReading read() = 0;
};
```

Concrete implementations include:

- `FakeSensorReader`
- `SystemSensorReader`
- `UartSensorReader`

This keeps the application logic independent from the specific sensor data source.

## Debugging

A debugging walkthrough is available at:

```text
docs/debugging-walkthrough.md
```

It explains how to debug the application using VS Code, GDB, and virtual UART ports.

## Current Learning Goals

This project is used to practice:

- Modern C++20 fundamentals
- CMake project organization
- Clean source/header separation
- RAII and resource management
- Basic embedded Linux concepts
- UART-style protocol parsing
- Polymorphism through interfaces
- Unit testing with GoogleTest
- Test automation with CTest
- CI with GitHub Actions
- Professional Git workflow with branches, pull requests, and releases

## Roadmap

Planned next steps:

- Add JSON telemetry output
- Add configuration file support
- Add logging levels
- Add MQTT publisher
- Add systemd service example
- Add Raspberry Pi deployment notes
- Add real UART integration with STM32 or another MCU
- Add more unit tests for configuration parsing and telemetry formatting

## Status

Current milestone:

```text
v0.6.0 - Testing Foundation
```

This version introduces the first unit testing foundation with GoogleTest, CTest, UART parser tests, telemetry status tests, and CI test execution.
