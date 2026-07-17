#include "../include/iot/uart_protocol.hpp"
#include <sstream>
#include <stdexcept>
#include <string>


namespace iot::sensor {
    namespace {
        bool parse_bool_value(const std::string& value) {
            return value == "1" || value == "true" || value == "HIGH";
        }

        std::string trim(const std::string& text) {
            const std::size_t first{text.find_first_not_of(" \t\r\n")};

            if(first == std::string::npos) {
                return {};
            }

            const std::size_t last{text.find_last_not_of(" \t\r\n")};

            return text.substr(first, last - first + 1);
        }

    }

    SensorReading parse_uart_line(const std::string& line) {
            SensorReading reading{};

            std::istringstream stream{line};
            std::string token;

            bool has_temperature{false};
            bool has_humidity{false};
            bool has_voltage{false};

            while (std::getline(stream, token, ',')) {
                const std::size_t separator_pos{token.find('=')};
                if (separator_pos == std::string::npos) {
                    continue;
                }

                const std::string key{trim(token.substr(0, separator_pos))};
                const std::string value{trim(token.substr(separator_pos + 1))};

                if (key == "TEMP") {
                    reading.temperature_celsius = std::stod(value);
                    has_temperature = true;
                } else if (key == "HUM") {
                    reading.humidity_percent = std::stod(value);
                    has_humidity = true;
                } else if (key == "VOLT") {
                    reading.voltage_volts = std::stod(value);
                    has_voltage = true;
                } else if (key == "DI0") {
                    reading.digital_inputs.at(0) = parse_bool_value(value);
                } else if (key == "DI1") {
                    reading.digital_inputs.at(1) = parse_bool_value(value);
                } else if (key == "DI2") {
                    reading.digital_inputs.at(2) = parse_bool_value(value);
                } else if(key == "DI3") {
                    reading.digital_inputs.at(3) = parse_bool_value(value);
                }
            }

            if (!has_temperature || !has_humidity || !has_voltage) {
                throw std::runtime_error{
                    "Invalid UART line. Expected: TEMP=25.4,HUM=51.2,VOLT=3.3,DI0=1,DI1=0,DI2=1,DI3=0"
                };
            }
            return reading;
        }
}