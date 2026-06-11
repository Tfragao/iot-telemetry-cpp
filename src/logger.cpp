#include "../include/iot/logger.hpp"
#include <iomanip>

namespace iot::logger {
    CsvLogger::CsvLogger(const std::string& file_path): file_{file_path, std::ios::app}{

    }

    void CsvLogger::write_header() {
        if (!file_) {
            return;
        }
        file_ <<"device_id, temperature_c, humidity_percent, voltage_v, di0, di1, di2, di3, status\n";
    }

    void CsvLogger::write_packet(const telemetry::TelemetryPacket& packet) {
        if (!file_) {
            return;
        }
        
        file_ << packet.device_id << ','
              << std::fixed << std::setprecision(2)
              << packet.reading.temperature_celsius << ','
              << packet.reading.humidity_percent << ','
              << packet.reading.voltage_volts << ','
              << packet.reading.digital_inputs.at(0) << ','
              << packet.reading.digital_inputs.at(1) << ','
              << packet.reading.digital_inputs.at(2) << ','
              << packet.reading.digital_inputs.at(3) << ','
              << telemetry::status_to_text(packet.status)
              << '\n';
    }

    bool CsvLogger::is_open() const {
        return file_.is_open();
    }
}