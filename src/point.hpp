#ifndef MATH_POINT_HPP
#define MATH_POINT_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>
#include <type_traits>

template<typename T, std::size_t n>
struct point {
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
struct point<T, 2> {
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
        T data[2];
    };
};

template<typename T>
struct point<T, 3> {
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
            T x, y, z;
        };
        T data[3];
    };
};

template<typename T, typename... ArgT>
constexpr point<T, sizeof...(ArgT)> make_point(ArgT... v) {
    return point<T, sizeof...(ArgT)>{.data = {static_cast<T>(v)...}};
}

template<typename T, std::size_t n>
constexpr T distanceSquare(const point<T, n> &a, const point<T, n> &b) {
    return std::transform_reduce(std::begin(a.data), std::end(a.data), std::begin(b.data), T{}, std::plus<>{}, [](auto a, auto b) {
        auto diff = b - a;
        return diff * diff;
    });
}

template<typename T, std::size_t n>
constexpr T distance(const point<T, n> &a, const point<T, n> &b) {
    return std::sqrt(distanceSquare(a, b));
}

#endif//MATH_POINT_HPP
