#ifndef MATH_POINT_HPP
#define MATH_POINT_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>
#include <type_traits>

template<typename T, std::size_t n>
struct point {
    std::array<T, n> data;
};

template<typename T>
struct point<T, 2> {
    static_assert(std::is_scalar_v<T>);
    union {
        struct {
            T x, y;
        };
        T data[2];
    };
};

template<typename T>
struct point<T, 3> {
    static_assert(std::is_scalar_v<T>);
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
