#include "../include/iot/config.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

namespace iot::config {
    namespace {
        bool is_option(const std::string& value) {
            return value.rfind("--", 0) == 0;
        }

        bool has_next_value(int current_index, int argc) {
            return current_index + 1 < argc;
        }

        int parse_positive_int(const std::string& text) {
            try {
                const int value{std::stoi(text)};
                return (value <= 0) ? -1 : value;
            } catch(const std::exception&) {
                return -1;
            }
        }
    }
    AppConfig parse_arguments(int argc, char* argv[]) {
        AppConfig config{};

        for (int index{1}; index < argc; ++index) {
            const std::string argument{argv[index]};
            
            if (argument == "--help" || argument == "--h") {
                config.show_help = true;
                return config;
            }

            if (argument == "--device") {
                if (!has_next_value(index, argc)) {
                    std::cerr << "Error: --device requires a value\n";
                    config.valid = false;
                    return config;
                }
                const std::string value{argv[++index]};
                if (value.empty() || is_option(value)) {
                    std::cerr << "Error: invalid device ID\n";
                    config.valid = false;
                    return config;
                }
                config.device_id = value;
            } else if (argument == "--samples") {
                if (!has_next_value(index, argc)) {
                    std::cerr << "Error: --samples requires  a value\n";
                    config.valid = false;
                    return config;
                }
                const std::string value{argv[++index]};
                const int samples{parse_positive_int(value)};
                if (samples <= 0) {
                    std::cerr << "Error: --samples must be a positive integer\n";
                    config.valid = false;
                    return config;
                }
                config.samples = samples;
            } else if (argument == "--log") {
                if (!(has_next_value(index, argc))) {
                    std::cerr << "Error: --log requires a file path\n";
                    config.valid = false;
                    return config;
                }
                const std::string value{argv[++index]};
                if (value.empty() || is_option(value)) {
                    std::cerr << "Error: invalid log file path\n";
                    config.valid = false;
                    return config;
                }
                config.log_file_path = value;
            } else {
                std::cerr << "Error: unknown argument: " << argument << "\n";
                config.valid = false;
                return config;
            }
        }
        return config;
    }

    void print_usage(const char* program_name) {
        std::cout << "Usage:\n";
        std::cout << "  " << program_name
                  << " [--device DEVICE_ID] [--samples COUNT] [--log FILE]\n\n";

        std::cout << "Options:\n";
        std::cout << "  --device DEVICE_ID   Set the device ID\n";
        std::cout << "  --samples COUNT      Number of telemetry samples to collect\n";
        std::cout << "  --log FILE           CSV log file path\n";
        std::cout << "  --help, -h           Show this help message\n\n";

        std::cout << "Examples:\n";
        std::cout << "  " << program_name << '\n';
        std::cout << "  " << program_name << " --device rpi4-gateway-001 --samples 5\n";
        std::cout << "  " << program_name << " --device bbb-gateway-001 --log data/bbb.csv\n";
    }
}