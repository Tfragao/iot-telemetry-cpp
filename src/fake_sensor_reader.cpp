#include "../include/iot/fake_sensor_reader.hpp"
#include <random>

namespace iot::sensor {
    namespace {
        double random_double(double min_value, double max_value) {
            static std::random_device random_device;
            static std::mt19937 generator {
                random_device()
            };

            std::uniform_real_distribution<double> distribution {
                min_value, max_value
            };
            return distribution(generator);
        }

        bool random_bool() {
            static std::random_device random_device;
            static std::mt19937 generator {
                random_device()
            };
            std::bernoulli_distribution distribution{0.5};
            return distribution(generator);
        }
    }

    SensorReading FakeSensorReader::read() {
        return SensorReading{
            .temperature_celsius = random_double(18.0, 45.0),
            .humidity_percent = random_double(20.0, 90.0),
            .voltage_volts = random_double(3.0, 5.2),
            .digital_inputs = {random_bool(), random_bool(), random_bool(), random_bool()}
        };
    }
}