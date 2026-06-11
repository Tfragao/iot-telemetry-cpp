#include "../include/iot/app.hpp"
#include "../include/iot/logger.hpp"
#include "../include/iot/menu.hpp"
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

    int run() {
        const std::string device_id{"rpi4-gateway-001"};
        logger::CsvLogger csv_logger{"data/telemetry.csv"};

        if (!csv_logger.is_open()) {
            std::cerr << "Failed to open telemetry log file\n";
            return 1;
        }

        csv_logger.write_header();
        std::vector<telemetry::TelemetryPacket> history;
        bool running{true};

        while (running) {
            menu::print_menu();
            const menu::MenuChoice choice{menu::read_choice()};

            switch(choice) {
                case menu::MenuChoice::ReadOnce:
                    collect_and_print(device_id, history, csv_logger);
                    break;
                case menu::MenuChoice::ReadMultiple:
                    for (int count{}; count < reading_per_batch; ++count) {
                        collect_and_print(device_id, history, csv_logger);
                    }
                    break;
                case menu::MenuChoice::ShowAverage:
                    std::cout << "Average temperature: "
                              << telemetry::average_temperature(history)
                              << " C\n\n";
                    break;
                case menu::MenuChoice::Exit:
                    std::cout << "Exiting application...\n";
                    running = false;
                    break;
                case menu::MenuChoice::Invalid:
                    std::cout << "Invalide option. Please try again\n\n";
                    break;
            }
        }
        return 0;
    }
}