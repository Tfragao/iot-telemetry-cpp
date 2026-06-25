#include "../include/iot/app.hpp"

#include "../include/iot/sensor_reader.hpp"
#include "../include/iot/fake_sensor_reader.hpp"
#include "../include/iot/system_sensor_reader.hpp"
#include "../include/iot/uart_sensor_reader.hpp"

#include "../include/iot/logger.hpp"
#include "../include/iot/telemetry.hpp"

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>

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

        std::unique_ptr<sensor::ISensorReader> create_sensor_reader(const config::AppConfig& config) {
            switch (config.mode) {
                case config::SensorMode::Fake:
                    return std::make_unique<sensor::FakeSensorReader>();
               
                case config::SensorMode::System:
                    return std::make_unique<sensor::SystemSensorReader>();
                
                case config::SensorMode::Uart:
                    return std::make_unique<sensor::UartSensorReader> (
                        config.port,
                        config.baud_raute
                    );   
            }
            throw std::runtime_error{"Unsupported sensor mode"};
        }

        bool prepare_log_directory(const std::string& log_file_path) {
            const std::filesystem::path log_path{log_file_path};
            if (!log_path.has_parent_path()) {
                return true;
            }

            std::error_code error_code;
            std::filesystem::create_directories(log_path.parent_path(), error_code);
            if (error_code) {
                std::cerr << "Failed to create log directory: "
                          << log_path.parent_path()
                          << "\n";
                std::cerr << "Reason: "
                          << error_code.message()
                          << "\n";
                return false;
            }
           
            return true;
        }
    }

    int run(const config::AppConfig& config){
        if (config.verbose) {
            print_verbose_config(config);
        }

        if (!prepare_log_directory(config.log_file_path)) {
            return 1;
        }

        logger::CsvLogger csv_logger{config.log_file_path};
        if (!csv_logger.is_open())
        {
            std::cerr << "Failed to open telemetry log file\n"
                      << config.log_file_path
                      << "\n";
            return 1;
        }

        csv_logger.write_header();

        std::vector<telemetry::TelemetryPacket> history;

        try {
                const std::unique_ptr<sensor::ISensorReader> sensor_reader {
                    create_sensor_reader(config)
                };
                for (int count{}; count < config.samples; ++count) {
                    
                    const sensor::SensorReading reading {sensor_reader->read()};

                    const telemetry::TelemetryPacket packet {
                        telemetry::create_packet(config.device_id, reading)
                    };

                    telemetry::print_packet(packet);
                    csv_logger.write_packet(packet);
                    history.push_back(packet);

                }
        } catch (const std::exception& exception) {
            std::cerr << "Failed to collect telemetry: "
                      << exception.what()
                      << "\n";
            return 1;
        }
        
        std::cout << "Average temperature: "
                << telemetry::average_temperature(history)
                << " C\n";
        return 0;
    }
}