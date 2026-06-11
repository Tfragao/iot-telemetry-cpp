#include "../include/iot/sensor.hpp" 
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

    SensorReading generate_fake_reading() {
        SensorReading reading {
            .temperature_celsius = random_double(18.0, 45.0),
            .humidity_percent = random_double(20.0, 90.0),
            .voltage_volts = random_double(3.0, 5.2),
            .digital_inputs = {random_bool(), random_bool(), random_bool(), random_bool()}
        };
        return reading;
    }

    bool is_temperature_valid(double temperature_celsius) {
        return temperature_celsius >= -20.0 && temperature_celsius <= 85.0;
    }

    bool is_humidity_valid(double humidity_percent) {
        return humidity_percent >= 0.0 && humidity_percent <= 100.0;
    }

    bool is_voltage_valid(double voltage_volts) {
        return voltage_volts >= 3.0 && voltage_volts <= 5.5;
    }

    std::string digital_input_to_text(bool state) {
       return state ? "HIGH" : "LOW";
    }
}