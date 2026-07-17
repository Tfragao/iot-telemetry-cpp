#include <gtest/gtest.h>
#include <stdexcept>
#include "../include/iot/uart_protocol.hpp"

TEST(UartProtocolTest, ParsesValidUartLine) {
    const iot::sensor::SensorReading reading{
        iot::sensor::parse_uart_line(
            "TEMP=25.4,HUM=51.3,VOLT=3.3,DI0=1,DI1=0,DI2=1,DI3=0"
        )
    };

    EXPECT_DOUBLE_EQ(reading.temperature_celsius, 25.4);
    EXPECT_DOUBLE_EQ(reading.humidity_percent, 51.3);
    EXPECT_DOUBLE_EQ(reading.voltage_volts, 3.3);

    EXPECT_TRUE(reading.digital_inputs.at(0));
    EXPECT_FALSE(reading.digital_inputs.at(1));
    EXPECT_TRUE(reading.digital_inputs.at(2));
    EXPECT_FALSE(reading.digital_inputs.at(3));
}

TEST(UartProtocolTest, ParseHighLowDigitalInputs) {
    const iot::sensor::SensorReading reading{
        iot::sensor::parse_uart_line(
            "TEMP=21.0,HUM=45.0,VOLT=3.8,DI0=HIGH,DI1=LOW,DI2=1,DI3=0"
        )
    };

    EXPECT_TRUE(reading.digital_inputs.at(0));
    EXPECT_FALSE(reading.digital_inputs.at(1));
    EXPECT_TRUE(reading.digital_inputs.at(2));
    EXPECT_FALSE(reading.digital_inputs.at(3));
}

TEST(UartProtocolTest, ThrowsWhenHumidityIsMissing) {
    EXPECT_THROW(
        iot::sensor::parse_uart_line(
            "TEMP=25.4,HUM=51.2,DI0=1,DI1=0,DI2=1,DI3=0"
        ),
        std::runtime_error
    );
}

TEST(UartProtocolTest, ParsesLineWithSpacesAfterCommas) {
    const iot::sensor::SensorReading reading{
        iot::sensor::parse_uart_line(
            "TEMP=25.4, HUM=51.2, VOLT=3.3, DI0=1, DI1=0, DI2=1, DI3=0"
        )
    };

    EXPECT_DOUBLE_EQ(reading.temperature_celsius, 25.4);
    EXPECT_DOUBLE_EQ(reading.humidity_percent, 51.2);
    EXPECT_DOUBLE_EQ(reading.voltage_volts, 3.3);

    EXPECT_TRUE(reading.digital_inputs.at(0));
    EXPECT_FALSE(reading.digital_inputs.at(1));
    EXPECT_TRUE(reading.digital_inputs.at(2));
    EXPECT_FALSE(reading.digital_inputs.at(3));
}