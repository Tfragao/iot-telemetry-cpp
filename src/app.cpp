#include "../include/iot/app.hpp"
#include "../include/iot/logger.hpp"
#include "../include/iot/sensor.hpp"
#include "../include/iot/telemetry.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

namespace iot::app {
    namespace {

        void print_verbose_config(const config::AppConfig& config) {
            std::cout << "Configuration:\n";
            std::cout << "  Device ID: " << config.device_id << "\n";
            std::cout << "  Mode: " << config::sensor_mode_to_text(config.mode) << "\n";
            std::cout << "  Samples: " << config.samples << "\n";

            if (config.mode == config::SensorMode::Uart) {
                std::cout << "  Port: " << config.port << "\n";
                std::cout << "  Baud rate: " << config.baud_raute << "\n";
            }
            std::cout << "\n";
        }

        sensor::SensorReading read_sensor_by_mode( const config::AppConfig& config) {
            switch (config.mode) {
                case config::SensorMode::Fake:
                    return sensor::generate_fake_reading();
                case config::SensorMode::System:
                    return sensor::read_system_reading();
                case config::SensorMode::Uart:
                    return sensor::read_uart_placeholder_reading(config.port, config.baud_raute);
            }

            return sensor::generate_fake_reading();
        }
    }

    int run(const config::AppConfig& config){
        if (config.verbose) {
            print_verbose_config(config);
        }

        const std::filesystem::path log_path{config.log_file_path};
        if (log_path.has_parent_path()) {
            std::filesystem::create_directories(log_path.parent_path());
        }
        
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
            const sensor::SensorReading reading{read_sensor_by_mode(config)};

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