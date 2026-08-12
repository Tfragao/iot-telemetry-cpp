#pragma once 
#include "sensor.hpp"
#include <string>
#include <optional>

namespace iot::sensor {
    std::optional<SensorReading> try_parse_uart_line(const std::string& line);
    SensorReading parse_uart_line(const std::string& line);
}