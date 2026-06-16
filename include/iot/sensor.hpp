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

    class UartSensorReader {
        public:
            UartSensorReader(const std::string& port, int baud_rate);
            ~UartSensorReader();
            
            UartSensorReader(const UartSensorReader&) = delete;
            UartSensorReader& operator=(const UartSensorReader&) = delete;

            UartSensorReader(UartSensorReader&&) = delete;
            UartSensorReader&& operator=(const UartSensorReader&&) = delete;

            SensorReading read();

        private:
            void configure(int baud_rate);
            std::string read_line();

            int fd_{-1};
    };

    SensorReading generate_fake_reading();
    SensorReading read_system_reading();
    
    bool is_temperature_valid(double temperature_celsius);
    bool is_humidity_valid(double humidity_percent);
    bool is_voltage_valid(double voltage_volts);
    
    std::string digital_input_to_text(bool state);
}