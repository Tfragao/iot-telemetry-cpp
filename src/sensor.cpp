#include <random>
#include <fstream>
#include <string>
#include <iostream>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <termios.h>
#include <unistd.h>
#include "../include/iot/sensor.hpp" 
namespace iot::sensor {
    namespace {
        double random_double(double min_value, double max_value) {
            static std::random_device random_device;
            static std::mt19937 generator {
                random_device()
            };

            std::uniform_real_distribution<double> distribution {
                min_value, max_value
            };
            return distribution(generator);
        }

        bool random_bool() {
            static std::random_device random_device;
            static std::mt19937 generator {
                random_device()
            };
            std::bernoulli_distribution distribution{0.5};
            return distribution(generator);
        }

        double read_linux_cpu_temperature() {
            std::ifstream temperature_file{"/sys/class/thermal/thermal_zone0/temp"};

            if (!temperature_file) {
                return random_double(30.0, 55.0);
            }

            int temperature_milli_celsius{};
            temperature_file >> temperature_milli_celsius;

            if (!temperature_file) {
                return random_double(30.0, 55.0);
            }

            return static_cast<double> (temperature_milli_celsius) / 1000.0;
        }

        speed_t to_termios_baud_rate(int baud_rate) {
            switch (baud_rate) {
                case 9600:
                    return B9600;
                case 19200:
                    return B19200;
                case 38400:
                    return B38400;
                case 57600:
                    return B576000;
                case 115200:
                    return B1152000;
                default:
                    throw std::runtime_error{"Unsupported baud rate"};
            }
        }

        bool parse_bool_value(const std::string& value) {
            return value == "1" || value == "true" || value == "HIGH";
        }

        SensorReading parse_uart_line(const std::string& line) {
            SensorReading reading{};

            std::istringstream stream{line};
            std::string token;

            bool has_temperature{false};
            bool has_humidity{false};
            bool has_voltage{false};

            while (std::getline(stream, token, ',')) {
                const std::size_t separator_pos{token.find('=')};
                if (separator_pos == std::string::npos) {
                    continue;
                }

                const std::string key{token.substr(0, separator_pos)};
                const std::string value{token.substr(separator_pos + 1)};

                if (key == "TEMP") {
                    reading.temperature_celsius = std::stod(value);
                    has_temperature = true;
                } else if (key =="HUM") {
                    reading.humidity_percent = std::stod(value);
                    has_humidity = true;
                } else if (key == "VOLT") {
                    reading.voltage_volts = std::stod(value);
                    has_voltage = true;
                } else if (key == "DIO") {
                    reading.digital_inputs.at(0) = parse_bool_value(value);
                } else if (key == "DI1") {
                    reading.digital_inputs.at(1) = parse_bool_value(value);
                } else if (key == "DI2") {
                    reading.digital_inputs.at(2) = parse_bool_value(value);
                } else if(key == "DI3") {
                    reading.digital_inputs.at(3) = parse_bool_value(value);
                }
            }

            if (!has_temperature || !has_humidity || !has_voltage) {
                throw std::runtime_error{
                    "Invalid UART line. Expected: TEMP=25.4,HUM=51.2,VOLT=3.3,DI0=1,DI1=0,DI2=1,DI3=0"
                };
            }
            return reading;
        }
    }

    UartSensorReader::UartSensorReader(const std::string& port, int baud_rate) {
            fd_ = ::open(port.c_str(), O_RDWR | O_NOCTTY);
            if (fd_ < 0) {
                throw std::runtime_error {
                    "Failed to open serial port " + port + ": " + std::strerror(errno)
                };
            }
            configure(baud_rate);
        }

    UartSensorReader::~UartSensorReader() {
        if (fd_ >= 0) {
        ::close(fd_);
        }
    }
    
    void UartSensorReader::configure(int baud_rate) {
        termios tty{};
        if (::tcgetattr(fd_, &tty) != 0) {
            throw std::runtime_error {
                std::string{"tcgetattr failed: "} + std::strerror(errno)
            };
        }
        const speed_t speed{to_termios_baud_rate(baud_rate)};
        if (::cfsetispeed(&tty, speed) != 0) {
            throw std::runtime_error{
                std::string{"cfsetisspeed failed: "} + std::strerror(errno)
            };
        }
        if (::cfsetospeed(&tty, speed) != 0) {
            throw std::runtime_error{
                std::string{"cfsetospeed failed: "} + std::strerror(errno)
            };
        }

        tty.c_cflag &= ~PARENB; // No parity
        tty.c_cflag &= ~CSTOPB; // 1  stop bit
        tty.c_cflag &= ~CSIZE;
        tty.c_cflag |= CS8;     // 8 data bits
        tty.c_cflag |= CREAD;
        tty.c_cflag |= CLOCAL; 
        
        #ifdef CRTSCTS
                tty.c_cflag &= ~CRTSCTS; // No hardware flow control
        #endif 
        tty.c_lflag &= ~ICANON;
        tty.c_lflag &= ~ECHO;
        tty.c_cflag &= ~ECHOE;
        tty.c_lflag &= ~ECHONL;
        tty.c_lflag &= ~ISIG;

        tty.c_iflag &= ~(IXON | IXOFF | IXANY);
        tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

        tty.c_oflag &= ~OPOST;
        tty.c_oflag &= ~ONLCR;

        tty.c_cc[VTIME] = 10; // 1 second timeout
        tty.c_cc[VMIN] = 0;

        if (::tcsetattr(fd_, TCSANOW, &tty) != 0) {
            throw std::runtime_error{
                std::string{"tcsetattr failed: "} + std::strerror(errno)
            };
        }
    }

    std::string UartSensorReader::read_line() {
        std::string line;
        char byte{};

        while (true) {
            const ssize_t bytes_read{::read(fd_, &byte, 1)};
            if (bytes_read < 0) {
                throw std::runtime_error {
                    std::string{"Failed to read from serail port: "} + std::strerror(errno)
                };
            }

            if (bytes_read == 0) {
                continue;
            }

            if (byte == '\n') {
                break;
            }
            
            if (byte != '\r') {
                line.push_back(byte);
            }
        }
        return line;
    }
    
    SensorReading UartSensorReader::read() {
        const std::string line{read_line()};
        std::cout << "UART RX: " << line << "\n";
        return parse_uart_line(line);
    }

    SensorReading generate_fake_reading() {
        SensorReading reading {
            .temperature_celsius = random_double(18.0, 45.0),
            .humidity_percent = random_double(20.0, 90.0),
            .voltage_volts = random_double(3.0, 5.2),
            .digital_inputs = {random_bool(), random_bool(), random_bool(), random_bool()}
        };
        return reading;
    }

    SensorReading read_system_reading() {
        const double cpu_temperature{read_linux_cpu_temperature()};

        SensorReading reading{
            .temperature_celsius = cpu_temperature,
            .humidity_percent = random_double(35.0, 70.0),
            .voltage_volts = random_double(4.7, 5.1),
            .digital_inputs = {false, true, false, true}
        };
        return reading;
    }

    bool is_temperature_valid(double temperature_celsius) {
        return temperature_celsius >= -20.0 && temperature_celsius <= 85.0;
    }

    bool is_humidity_valid(double humidity_percent) {
        return humidity_percent >= 0.0 && humidity_percent <= 100.0;
    }

    bool is_voltage_valid(double voltage_volts) {
        return voltage_volts >= 3.0 && voltage_volts <= 5.5;
    }

    std::string digital_input_to_text(bool state) {
       return state ? "HIGH" : "LOW";
    }
}