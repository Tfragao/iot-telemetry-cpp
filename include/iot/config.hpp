#pragma once

#include <string>

namespace iot::config {
    struct AppConfig {
        std::string device_id{"rpi4-gateway-001"};
        std::string log_file_path{"data/telemetry.csv"};
        int samples{1};
        bool show_help{false};
        bool valid{true};
    };

    AppConfig parse_arguments(int argc, char* argv[]);
    void print_usage(const char* program_name);
}