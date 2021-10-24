#ifndef MATH_VECTOR_HPP
#define MATH_VECTOR_HPP

#include "point.hpp"
#include <algorithm>
#include <cmath>
#include <numeric>

template<typename T, std::size_t n>
struct vector {
    std::array<T, n> data;
};

template<typename T>
struct vector<T, 2> {
    static_assert(std::is_scalar_v<T>);
    union {
        struct {
            T x, y;
        };
        T data[2];
    };
};

template<typename T>
struct vector<T, 3> {
    static_assert(std::is_scalar_v<T>);
    union {
        struct {
            T x, y, z;
        };
        T data[3];
    };
};

template<typename T, typename U, std::size_t n, typename K = decltype(T() + U())>
constexpr point<K, n> apply(const point<T, n> &a, const vector<U, n> &b) {
    point<K, n> p;
    std::transform(std::begin(a.data), std::end(a.data), std::begin(b.data), std::begin(p.data), std::plus<>{});
    return p;
}

template<typename T, typename... ArgT>
constexpr vector<T, sizeof...(ArgT)> make_vector(ArgT... v) {
    return vector<T, sizeof...(ArgT)>{.data = {static_cast<T>(v)...}};
}

template<typename T, std::size_t n>
constexpr T dot(const vector<T, n> &a, const vector<T, n> &b) {
    return std::transform_reduce(std::begin(a.data), std::end(a.data), std::begin(b.data), T{}, std::plus<>{}, std::multiplies<>{});
}

template<typename T, std::size_t n>
constexpr T normSquare(const vector<T, n> &a) {
    return std::transform_reduce(std::begin(a.data), std::end(a.data), std::begin(a.data), T{}, std::plus<>{}, std::multiplies<>{});
}

template<typename T, std::size_t n>
constexpr T norm(const vector<T, n> &a) {
    return std::sqrt(normSquare(a));
}

template<typename T, std::size_t n>
constexpr vector<T, n> cross(const vector<T, n> &a, const vector<T, n> &b) {
    vector<T, n> result{.data = {}};
    static_assert(n >= 3);
    for (unsigned i{}; i < 3; ++i)
        result.data[i] = a.data[(i + 2) % 3] * b.data[(i + 3) % 3] - b.data[(i + 2) % 3] * a.data[(i + 3) % 3];

    return result;
}

template<typename T, std::size_t n>
constexpr T cos(const vector<T, n> &a, const vector<T, n> &b) {
    return dot(a, b) / std::sqrt(normSquare(a) * normSquare(b));
}

template<typename T, std::size_t n>
constexpr T sin(const vector<T, n> &a, const vector<T, n> &b) {
    return norm(cross(a, b)) / std::sqrt(normSquare(a) * normSquare(b));
}

template<typename T>
constexpr T sin(const vector<T, 2> &a, const vector<T, 2> &b) {
    return sin(vector<T, 3>{.data = {a.data[0], a.data[1], 0}}, vector<T, 3>{.data = {b.data[0], b.data[1], 0}});
}

#endif//MATH_VECTOR_HPP
