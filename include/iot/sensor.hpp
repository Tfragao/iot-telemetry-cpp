#pragma once 

#include <array>
#include <cstdint>
#include <string>

namespace iot::sensor {
    constexpr std::size_t digital_input_count{4};

    struct SensorReading {
        double temperature_celsius{};
        double humidity_percent{};
        double voltage_volts{};
        std::array<bool, digital_input_count> digital_inputs{};
    };

    SensorReading generate_fake_reading();
    SensorReading read_system_reading();
    SensorReading read_uart_placeholder_reading(const std::string& port, int baud_rate);

    bool is_temperature_valid(double temperature_celsius);
    bool is_humidity_valid(double humidity_percent);
    bool is_voltage_valid(double voltage_volts);
    
    std::string digital_input_to_text(bool state);
}