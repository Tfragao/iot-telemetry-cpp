#include "../include/iot/statistics.hpp"
#include <gtest/gtest.h>
#include <string>
#include <vector>

namespace {
    struct Sample {
        double temperature{};
        int voltage_mv{};
        std::string status;
    };
}

TEST(StatisticsTest, ComputesAverageFromNumericValues) {
    const std::vector<int> values{10, 20, 30};
    const double average{
        iot::statistics::average_by(
            values, 
            [](int value) {
                return value;
            }
        )
    };
   
    EXPECT_DOUBLE_EQ(average, 20.0);
}

TEST(StatisticsTest, ReturnsZeroForEmptyRange) {
    const std::vector<int> values{};
    const double average{
        iot::statistics::average_by(
            values,
            [](int value) {
                return value;
            }
        )
    };
   
    EXPECT_DOUBLE_EQ(average, 0.0);
}

TEST(StatisticsTest, ComputeAverageUsingProjectionLambda) {
    const std::vector<Sample> samples{
        Sample{.temperature = 20.0, .voltage_mv = 3300, .status = "OK"},
        Sample{.temperature = 30.0, .voltage_mv = 3200, .status = "WARNING"},
        Sample{.temperature = 40.0, .voltage_mv = 3100, .status = "WARNING"}
    };

    const double average_temperature{
        iot::statistics::average_by(
            samples,
            [](const Sample& sample) {
                return sample.temperature;
            }
        )
    };

    EXPECT_DOUBLE_EQ(average_temperature, 30.0);
}

TEST(StatisticsTest, CountsMatchingValuesUsingPredicateLambda) {
    const std::vector<Sample> samples {
        Sample{.temperature = 20.0, .voltage_mv = 3300, .status = "OK"},
        Sample{.temperature = 30.0, .voltage_mv = 3200, .status = "WARNING"},
        Sample{.temperature = 40.0, .voltage_mv = 3100, .status = "WARNING"}
    };
    const std::size_t warning_count{
        iot::statistics::count_if(
            samples,
            [](const Sample& sample) {
                return sample.status == "WARNING";
            }
        )
    };

    EXPECT_EQ(warning_count, 2U);
}