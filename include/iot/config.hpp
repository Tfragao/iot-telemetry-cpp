#pragma once

#include <string>

namespace iot::config {
    enum class SensorMode {
        Fake,
        System,
        Uart
    };
    struct AppConfig {
        std::string device_id{"rpi4-gateway-001"};
        std::string log_file_path{"data/telemetry.csv"};

        SensorMode mode{SensorMode::Fake};

        std::string port{"/dev/ttyUSB0"};
        int baud_raute{115200};

        int samples{1};

        bool verbose{false};
        bool show_help{false};
        bool valid{true};
    };

    AppConfig parse_arguments(int argc, char* argv[]);
    void print_usage(const char* program_name);
    std::string sensor_mode_to_text(SensorMode mode);
}