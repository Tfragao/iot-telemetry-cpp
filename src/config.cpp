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

        SensorMode parse_sensor_mode(const std::string& value, bool& valid) {
            if (value == "fake") {
                return SensorMode::Fake;
            }

            if (value == "system") {
                return SensorMode::System;
            }

            if (value == "uart") {
                return SensorMode::Uart;
            }

            valid = false;
            return SensorMode::Fake;
        }
    }

    AppConfig parse_arguments(int argc, char* argv[]) {
        AppConfig config{};

        for (int index{1}; index < argc; ++index) {
            const std::string argument{argv[index]};
            
            if (argument == "--help" || argument == "--h") {
                config.show_help = true;
                return config;
            }else if (argument == "--verbose") {
                config.verbose = true;
            }else if (argument == "--device") {
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

            } else if (argument == "--mode") {
                if (!has_next_value(index, argc)){
                    std::cerr << "Error: --mode requires a value: fake, system, uart\n";
                    config.valid = false;
                    return config;
                }

                const std::string value{argv[++index]};
                if (value.empty() || is_option(value)) {
                    std::cerr << "Error: invalid mode\n";
                    config.valid = false;
                    return config;
                }

                bool mode_is_valid{true};
                config.mode = parse_sensor_mode(value, mode_is_valid);

                if (!mode_is_valid) {
                    std::cerr << "Error: invalid mode: " << value << "\n";
                    std::cerr << "Valid modes are: fake, system, uart\n";
                    config.valid = false;
                    return config;
                }
            } else if (argument == "--port") {
                if (!has_next_value(index, argc)){
                    std::cerr << "Error: --port requires a value\n";
                    config.valid = false;
                    return config;
                }

                const std::string value{argv[++index]};
                if (value.empty() || is_option(value)) {
                    std::cerr << "Error: invalid port\n";
                    config.valid = false;
                    return config;
                }

                config.port = value;

            } else if (argument == "--baud") {
                if (!has_next_value(index, argc)) {
                    std::cerr << "Error: --baud requires a value\n";
                    config.valid = false;
                    return config;
                }

                const std::string value{argv[++index]};
                const int baud_rate{parse_positive_int(value)};
                if(baud_rate <= 0) {
                    std::cerr << "Error: --baud must be a positive integer\n";
                    config.valid = false;
                    return config;
                }

                config.baud_raute = baud_rate;

            } else {
                std::cerr << "Error: unknown argument: " << argument << "\n";
                config.valid = false;
                return config;
            }  
        }

        if (config.mode == SensorMode::Uart) {
            if (config.port.empty()) {
                std::cerr << "Error: uart mode requires --port\n";
                config.valid = false;
                return config;
            }

            if (config.baud_raute <= 0) {
                std::cerr << "Error: uart mode requires a valid --baud value\n";
                config.valid = false;
                return config;
            }
        }
        return config;
    }

    std::string sensor_mode_to_text(SensorMode mode) {
        switch (mode) {
            case SensorMode::Fake:
                return "fake";
            case SensorMode::System:
                return "system";
            case SensorMode::Uart:
                return "uart";
        }

        return "unknown";
    }

    void print_usage(const char* program_name)
    {
        std::cout << "Usage:\n";
        std::cout << "  " << program_name
                  << " [--device DEVICE_ID]"
                  << " [--samples COUNT]"
                  << " [--log FILE]"
                  << " [--mode fake|system|uart]"
                  << " [--port DEVICE]"
                  << " [--baud RATE]"
                  << " [--verbose]\n\n";

        std::cout << "Options:\n";
        std::cout << "  --device DEVICE_ID      Set the device ID\n";
        std::cout << "  --samples COUNT         Number of telemetry samples to collect\n";
        std::cout << "  --log FILE              CSV log file path\n";
        std::cout << "  --mode MODE             Sensor mode: fake, system, or uart\n";
        std::cout << "  --port DEVICE           Serial device for uart mode, e.g. /dev/ttyUSB0\n";
        std::cout << "  --baud RATE             Baud rate for uart mode, e.g. 115200\n";
        std::cout << "  --verbose               Enable verbose output\n";
        std::cout << "  --help, -h              Show this help message\n\n";

        std::cout << "Examples:\n";
        std::cout << "  " << program_name << " --mode fake --samples 5 --verbose\n";
        std::cout << "  " << program_name << " --mode system --samples 5 --verbose\n";
        std::cout << "  " << program_name
                  << " --mode uart --port /dev/ttyUSB0 --baud 115200 --samples 5 --verbose\n";
    }
}