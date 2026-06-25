#pragma once
#include "sensor_reader.hpp"

namespace iot::sensor {
    class SystemSensorReader final : public ISensorReader {
        public: 
            SensorReading read() override;
    };
}