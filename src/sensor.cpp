#include "../include/iot/sensor.hpp" 
namespace iot::sensor {

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