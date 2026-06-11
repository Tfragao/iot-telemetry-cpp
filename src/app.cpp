#include "../include/iot/app.hpp"
#include "../include/iot/logger.hpp"
#include "../include/iot/sensor.hpp"
#include "../include/iot/telemetry.hpp"

#include <iostream>
#include <vector>
#include <string>

namespace iot::app {
    namespace {
        constexpr int reading_per_batch{5};

        telemetry::TelemetryPacket collect_packet(const std::string& device_id) {
            const sensor::SensorReading reading{
                sensor::generate_fake_reading()
            };
            return telemetry::create_packet(device_id, reading);
        }

        void collect_and_print(const std::string& device_id,
                               std::vector<telemetry::TelemetryPacket>& history,
                               logger::CsvLogger& csv_logger) {
            const telemetry::TelemetryPacket packet {
                collect_packet(device_id)
            };

            telemetry::print_packet(packet);
            csv_logger.write_packet(packet);

            history.push_back(packet);

        }  
    }

    int run(const config::AppConfig& config){
        logger::CsvLogger csv_logger{config.log_file_path};

        if (!csv_logger.is_open())
        {
            std::cerr << "Failed to open telemetry log file\n";
            return 1;
        }

        csv_logger.write_header();

        std::vector<telemetry::TelemetryPacket> history;

        for (int count{}; count < config.samples; ++count)
        {
            const sensor::SensorReading reading{sensor::generate_fake_reading()};

            const telemetry::TelemetryPacket packet{
                telemetry::create_packet(config.device_id, reading)
            };

            telemetry::print_packet(packet);
            csv_logger.write_packet(packet);
            history.push_back(packet);
        }

        std::cout << "Average temperature: "
                << telemetry::average_temperature(history)
                << " C\n";

        return 0;
    }
}