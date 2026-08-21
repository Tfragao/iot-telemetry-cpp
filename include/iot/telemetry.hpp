#pragma once 

#include "./sensor.hpp"

#include <string>
#include <vector>
#include <cstddef>

namespace iot::telemetry {
    enum class DeviceStatus {
        Ok, 
        Warning,
        Error
    };

    struct TelemetryPacket {
        std::string device_id;
        sensor::SensorReading reading;
        DeviceStatus status{};
    };

    TelemetryPacket create_packet(
            const std::string& device_id, 
            const sensor::SensorReading& reading
    );

    DeviceStatus evaluate_status(const sensor::SensorReading& reading);
    std::string status_to_text(DeviceStatus status);
    void print_packet(const TelemetryPacket& packet);
    double average_temperature(const std::vector<TelemetryPacket>& packets);
    std::size_t count_packets_with_status(const std::vector<TelemetryPacket>& packets, DeviceStatus status);
}