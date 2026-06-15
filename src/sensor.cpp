#include "../include/iot/sensor.hpp" 
#include <random>
#include <fstream>
#include <string>
#include <iostream>
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

    SensorReading generate_fake_reading() {
        SensorReading reading {
            .temperature_celsius = random_double(18.0, 45.0),
            .humidity_percent = random_double(20.0, 90.0),
            .voltage_volts = random_double(3.0, 5.2),
            .digital_inputs = {random_bool(), random_bool(), random_bool(), random_bool()}
        };
        return reading;
    }

    SensorReading read_system_reading() {
        const double cpu_temperature{read_linux_cpu_temperature()};

        SensorReading reading{
            .temperature_celsius = cpu_temperature,
            .humidity_percent = random_double(35.0, 70.0),
            .voltage_volts = random_double(4.7, 5.1),
            .digital_inputs = {false, true, false, true}
        };

        return reading;
    }

    SensorReading read_uart_placeholder_reading(const std::string& port, int baud_rate) {
        std::cout << "UART mode selected, but real serial reading is not implemented yet.\n";
        std::cout << "Using simulated UART-like telemetry for now.\n";
        std::cout << "  Configured port: " << port << '\n';
        std::cout << "  Configured baud rate: " << baud_rate << '\n';

        SensorReading reading {
            .temperature_celsius = random_double(22.0, 38.0),
            .humidity_percent = random_double(30.0, 80.0),
            .voltage_volts = random_double(3.1, 3.4),
            .digital_inputs = {true, false, true, false}
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