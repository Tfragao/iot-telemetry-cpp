#pragma once 
#include "sensor.hpp"
#include <string>

namespace iot::sensor {
    SensorReading parse_uart_line(const std::string& line);
}