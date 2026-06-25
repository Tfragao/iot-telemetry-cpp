#include "../include/iot/system_sensor_reader.hpp"
#include <fstream>
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

        double read_linux_cpu_temperature() {
            std::ifstream temperature_file{"/sys/class/thermal/thermal_zone0/temp"};

            if (!temperature_file) {
                return random_double(30.0, 55.0);
            }

            int temperature_milli_celsius{};
            temperature_file >> temperature_milli_celsius;

            if (!temperature_file) {
                return random_double(30.0, 55.0);
            }

            return static_cast<double> (temperature_milli_celsius) / 1000.0;
        }

    }

    SensorReading SystemSensorReader::read() {
        return SensorReading{
            .temperature_celsius = read_linux_cpu_temperature(),
            .humidity_percent = random_double(35.0, 70.0),
            .voltage_volts = random_double(4.7, 5.1),
            .digital_inputs = {false, true, false, true}
        };
    }
}