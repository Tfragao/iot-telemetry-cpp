#include "../include/iot/uart_sensor_reader.hpp"
#include "../include/iot/uart_protocol.hpp"
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

namespace iot::sensor {
    namespace {
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
}