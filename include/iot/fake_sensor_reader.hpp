#pragma once 

#include "sensor_reader.hpp"

namespace iot::sensor {
    class FakeSensorReader final : public ISensorReader {
        public: 
            SensorReading read() override;
    };
}