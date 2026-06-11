#pragma once 

#include "./telemetry.hpp"
#include <fstream>
#include <string>

namespace iot::logger {
    class CsvLogger {
        public:
            explicit CsvLogger(const std::string& file_path);

            void write_header();
            void write_packet(const telemetry::TelemetryPacket& packet);
            bool is_open() const;
        private:
            std::ofstream file_;
    };
}