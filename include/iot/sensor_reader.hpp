#pragma once 

#include "sensor.hpp"

namespace iot::sensor {
    class ISensorReader {
        public:
            virtual ~ISensorReader() = default;
            virtual SensorReading read() = 0;
    };
}