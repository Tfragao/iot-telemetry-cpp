#include "../include/iot/telemetry.hpp"
#include <gtest/gtest.h>

namespace {
    iot::sensor::SensorReading make_reading(
        double temperature_celsius,
        double humidity_percent,
        double voltage_volts
    ) {
        return iot::sensor::SensorReading{
            .temperature_celsius = temperature_celsius,
            .humidity_percent = humidity_percent,
            .voltage_volts = voltage_volts,
            .digital_inputs = {false, false, false, false}
        };
    }
}

TEST(TelemetryStatusTest, ReturnsOkForValidNormalReading) {
    const iot::sensor::SensorReading reading{
        make_reading(25.0, 50.0, 3.7)
    };

    const iot::telemetry::DeviceStatus status{
        iot::telemetry::evaluate_status(reading)
    };

    EXPECT_EQ(status, iot::telemetry::DeviceStatus::Ok);
}

TEST(TelemetryStatusTest, ReturnsWarningForHighTemperature) {
    const iot::sensor::SensorReading reading{
        make_reading(42.0, 50.0, 3.7)
    };

    const iot::telemetry::DeviceStatus status{
        iot::telemetry::evaluate_status(reading)
    };

    EXPECT_EQ(status, iot::telemetry::DeviceStatus::Warning);
}

TEST(TelemetryStatusTest, ReturnsWarningForLowVoltage) {
    const iot::sensor::SensorReading reading{
        make_reading(25.0, 50.0, 3.2)
    };

    const iot::telemetry::DeviceStatus status{
        iot::telemetry::evaluate_status(reading)
    };

    EXPECT_EQ(status, iot::telemetry::DeviceStatus::Warning);
}

TEST(TelemetryStatusTest, ReturnsErrorForInvalidTemperature) {
    const iot::sensor::SensorReading reading{
        make_reading(100.0, 50.0, 3.7)
    };
    const iot::telemetry::DeviceStatus status{
        iot::telemetry::evaluate_status(reading)
    };

    EXPECT_EQ(status, iot::telemetry::DeviceStatus::Error);
}

TEST(TelemetryStatusTest, ReturnsErrorForInvalidHumidity) {
    const iot::sensor::SensorReading reading{
        make_reading(25.0, 150.0, 3.7)
    };
    const iot::telemetry::DeviceStatus status{
        iot::telemetry::evaluate_status(reading)
    };

    EXPECT_EQ(status, iot::telemetry::DeviceStatus::Error);
}

TEST(TelemetryStatisticsTest, ComputeAverageTemperature) {
    const std::vector<iot::telemetry::TelemetryPacket> packets{
        iot::telemetry::create_packet(
            "test-device",
            make_reading(20.0, 50.0, 3.7)
        ),
        iot::telemetry::create_packet(
            "test-device",
            make_reading(30.0, 50.0, 3.7)
        ),
        iot::telemetry::create_packet(
            "test-device",
            make_reading(40.0, 50.0, 3.7)
        )
    };

    EXPECT_DOUBLE_EQ(iot::telemetry::average_temperature(packets), 30.0);
}

TEST(TelemetryStatisticsTest, CountsPacketsWithSpecificStatus) {
    const std::vector<iot::telemetry::TelemetryPacket> packets{
        iot::telemetry::create_packet(
            "test-device",
            make_reading(25.0, 50.0, 3.7)
        ),
        iot::telemetry::create_packet(
            "test-device",
            make_reading(42.0, 50.0, 3.7)
        ),
        iot::telemetry::create_packet(
            "test-device",
            make_reading(100.0, 50.0, 3.7)
        )
    };

    EXPECT_EQ(iot::telemetry::count_packets_with_status(packets, iot::telemetry::DeviceStatus::Ok), 1U);
    EXPECT_EQ(iot::telemetry::count_packets_with_status(packets, iot::telemetry::DeviceStatus::Warning), 1U);
    EXPECT_EQ(iot::telemetry::count_packets_with_status(packets, iot::telemetry::DeviceStatus::Error), 1U);
}