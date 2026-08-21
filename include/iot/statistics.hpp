#pragma once

#include <concepts>
#include <cstddef>
#include <functional>
#include <ranges>

namespace iot::statistics {
    template <typename Range, typename Projection>
    concept NumericProjection = std::ranges::input_range<const Range> && requires(
        Projection projection,
        std::ranges::range_reference_t<const Range> value
    ) {
        {std::invoke(projection, value)} -> std::convertible_to<double>;
    };

    template <typename Range, typename Projection>
        requires NumericProjection<Range, Projection>
    double average_by(const Range& values, Projection projection) {
        double sum{};
        std::size_t count{};

        for (const auto& value : values) {
            sum += static_cast<double>(std::invoke(projection, value));
            ++count;
        }
        return (count == 0U) ?  0.0 : (sum / static_cast<double>(count));
    }

    template <typename Range, typename Predicate>
        requires std::ranges::input_range<const Range> && std::predicate<Predicate, std::ranges::range_reference_t<const Range>>
    std::size_t count_if(const Range& values, Predicate predicate) {
        std::size_t count{};
        for (const auto& value : values) {
            if (std::invoke(predicate, value)) {
                ++count;
            }
        }
        return count;
    }
}