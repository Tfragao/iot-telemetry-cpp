#include "../include/iot/telemetry.hpp"
#include <iomanip>
#include <iostream>

namespace iot::telemetry {
    TelemetryPacket create_packet(const std::string& device_id, const sensor::SensorReading& reading){
        const DeviceStatus status{evaluate_status(reading)};
        TelemetryPacket packet {
            .device_id = device_id,
            .reading = reading,
            .status = status
        };
        return packet;
    }

    DeviceStatus evaluate_status(const sensor::SensorReading& reading) {
        if (!sensor::is_temperature_valid(reading.temperature_celsius) ||
            !sensor::is_humidity_valid(reading.humidity_percent) ||
            !sensor::is_voltage_valid(reading.voltage_volts)) {
                return DeviceStatus::Error;
        }
        if (reading.temperature_celsius > 40.0 || reading.voltage_volts < 3.3) {
            return DeviceStatus::Warning;
        }
        return DeviceStatus::Ok;
    }

    std::string status_to_text(DeviceStatus status) {
        switch (status) {
            case DeviceStatus::Ok:
                return "OK";
            case DeviceStatus::Warning:
                return "WARNING";
            case DeviceStatus::Error:
                return "ERROR";
        }
        return "UNKNOWN";
    }

    void print_packet(const TelemetryPacket& packet) {
        std::cout << "Device: " << packet.device_id << "\n";
        std::cout << "Temperature: "
                  << std::fixed << std::setprecision(2)
                  << packet.reading.temperature_celsius << " C\n";
        std::cout << "Humidity: "
                  << std::fixed << std::setprecision(2) 
                  << packet.reading.humidity_percent << " %\n";
        std::cout << "Voltage: "
                  << std::fixed << std::setprecision(2)
                  << packet.reading.voltage_volts << " V\n";
        std::cout << "Digital inputs:\n";
        
        for (std::size_t index{}; index < packet.reading.digital_inputs.size(); ++index) {
            std::cout << "DI" << index << ": "
                      << sensor::digital_input_to_text(packet.reading.digital_inputs.at(index))
                      <<"\n";
        }
        std::cout << "Status: " << status_to_text(packet.status) << "\n\n";
    }

    double average_temperature(const std::vector<TelemetryPacket>& packets) {
         if (packets.empty()) {
                return 0.0;
            }
            double sum{};
            for (const auto& packet : packets) {
                sum +=  packet.reading.temperature_celsius;
            }
            return sum / static_cast<double>(packets.size());
           
    }
}