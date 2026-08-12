#pragma once 
#include "sensor_reader.hpp"
#include "scoped_file_descriptor.hpp"
#include <string>

namespace iot::sensor {
        class UartSensorReader final : public ISensorReader {
            public: 
                UartSensorReader(const std::string& port, int baud_rate);
               
                UartSensorReader(const UartSensorReader&) = delete;
                UartSensorReader& operator=(const UartSensorReader&) = delete;

                UartSensorReader(UartSensorReader&&) = delete;
                UartSensorReader&& operator=(const UartSensorReader&&) = delete;

                SensorReading read() override;
            
            private:
                void configure(int baud_rate);
                std::string read_line();
                platform::ScopedFileDescriptor fd_;
        };
}