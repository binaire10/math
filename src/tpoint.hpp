#ifndef MATH_TPOINT_HPP
#define MATH_TPOINT_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>
#include <type_traits>

template<typename T, std::size_t n>
struct tpoint {
    using value_type = T;
    using reference = value_type &;
    using const_reference = const value_type &;
    using iterator = value_type *;
    using const_iterator = const value_type *;

    static_assert(std::is_scalar_v<value_type>);

    constexpr reference operator[](std::size_t i) noexcept {
        return data[i];
    }

    constexpr const_reference operator[](std::size_t i) const noexcept {
        return data[i];
    }

    constexpr iterator begin() noexcept {
        return data.begin();
    }

    constexpr iterator end() noexcept {
        return data.end();
    }

    constexpr const_iterator begin() const noexcept {
        return data.begin();
    }

    constexpr const_iterator end() const noexcept {
        return data.end();
    }

    std::array<T, n> data;
};

template<typename T>
struct tpoint<T, 2> {
    using value_type = T;
    using reference = value_type &;
    using const_reference = const value_type &;
    using iterator = value_type *;
    using const_iterator = const value_type *;

    static_assert(std::is_scalar_v<value_type>);

    constexpr reference operator[](std::size_t i) noexcept {
        return data[i];
    }

    constexpr const_reference operator[](std::size_t i) const noexcept {
        return data[i];
    }

    constexpr iterator begin() noexcept {
        return data.begin();
    }

    constexpr iterator end() noexcept {
        return data.end();
    }

    constexpr const_iterator begin() const noexcept {
        return data.begin();
    }

    constexpr const_iterator end() const noexcept {
        return data.end();
    }

    union {
        struct {
            T x, y;
        };
        std::array<T, 2> data;
    };
};

template<typename T>
struct tpoint<T, 3> {
    using value_type = T;
    using reference = value_type &;
    using const_reference = const value_type &;
    using iterator = value_type *;
    using const_iterator = const value_type *;

    static_assert(std::is_scalar_v<value_type>);

    constexpr reference operator[](std::size_t i) noexcept {
        return data[i];
    }

    constexpr const_reference operator[](std::size_t i) const noexcept {
        return data[i];
    }

    constexpr iterator begin() noexcept {
        return std::begin(data);
    }

    constexpr iterator end() noexcept {
        return std::end(data);
    }

    constexpr const_iterator begin() const noexcept {
        return std::begin(data);
    }

    constexpr const_iterator end() const noexcept {
        return std::end(data);
    }

    union {
        struct {
            T x, y, z;
        };
        std::array<T, 3> data;
    };
};

template<typename T, typename... ArgT>
constexpr tpoint<T, sizeof...(ArgT)> make_point(ArgT... v) {
    return tpoint<T, sizeof...(ArgT)>{.data = {static_cast<T>(v)...}};
}

template<typename T, std::size_t n>
constexpr T distanceSquare(const tpoint<T, n> &a, const tpoint<T, n> &b) {
    return std::transform_reduce(std::begin(a.data), std::end(a.data), std::begin(b.data), T{}, std::plus<>{}, [](auto a, auto b) {
        const auto diff = b - a;
        return diff * diff;
    });
}

template<typename T, std::size_t n>
constexpr T distance(const tpoint<T, n> &a, const tpoint<T, n> &b) {
    return std::sqrt(distanceSquare(a, b));
}

#endif//MATH_TPOINT_HPP
