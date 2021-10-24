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

namespace detail {
    template<typename T, std::size_t n>
    struct reduce_op {
        static_assert(n != 0, "required none null vector");
        template<typename ReduceT>
        static constexpr T call(const vector<T, n> &a, ReduceT reduce) {
            return call(a, std::move(reduce), std::make_index_sequence<n>{});
        }

        template<typename ReduceT, std::size_t... idx>
        static constexpr T call(const vector<T, n> &a, ReduceT reduce, std::index_sequence<idx...>) {
            T result = a.data[0];
            for (unsigned i = 1; i < n; ++i)
                result = reduce(result, a.data[i]);
            return result;
        }
    };

    template<typename T, std::size_t n>
    struct transform_op {
        static_assert(n != 0, "required none null vector");
        template<typename TransformT>
        static constexpr vector<T, n> call(const vector<T, n> &a, TransformT transform) {
            return call(a, std::move(transform), std::make_index_sequence<n>{});
        }

        template<typename TransformT, std::size_t... idx>
        static constexpr vector<T, n> call(const vector<T, n> &a, TransformT transform, std::index_sequence<idx...>) {
            vector<T, n> result{.data = {transform(a.data[idx])...}};
            return result;
        }

        template<typename TransformT>
        static constexpr vector<T, n> call(const vector<T, n> &a, const vector<T, n> &b, TransformT transform) {
            return call(a, b, std::move(transform), std::make_index_sequence<n>{});
        }

        template<typename TransformT, std::size_t... idx>
        static constexpr vector<T, n> call(const vector<T, n> &a, const vector<T, n> &b, TransformT transform, std::index_sequence<idx...>) {
            vector<T, n> result{.data = {transform(a.data[idx], b.data[idx])...}};
            return result;
        }

        template<typename TransformT>
        static constexpr point<T, n> call(const vector<T, n> &a, const point<T, n> &b, TransformT transform) {
            return call(a, b, std::move(transform), std::make_index_sequence<n>{});
        }

        template<typename TransformT, std::size_t... idx>
        static constexpr point<T, n> call(const vector<T, n> &a, const point<T, n> &b, TransformT transform, std::index_sequence<idx...>) {
            point<T, n> result{.data = {transform(a.data[idx], b.data[idx])...}};
            return result;
        }
    };

    template<typename T, std::size_t n>
    struct transform_reduce_op {
        template<typename ReduceT, typename TransformT>
        static constexpr T call(const vector<T, n> &a, const vector<T, n> &b, ReduceT reduce, TransformT transform) {
            return call(a, b, std::move(reduce), std::move(transform), std::make_index_sequence<n>{});
        }

        template<typename ReduceT, typename TransformT, std::size_t... idx>
        static constexpr T call(const vector<T, n> &a, const vector<T, n> &b, ReduceT reduce, TransformT transform, std::index_sequence<idx...>) {
            vector<T, n> compute{.data = {transform(a.data[idx], b.data[idx])...}};
            return reduce_op<T, n>::call(compute, std::move(reduce));
        }

        template<typename ReduceT, typename TransformT>
        static constexpr T call(const vector<T, n> &a, ReduceT reduce, TransformT transform) {
            return call(a, std::move(reduce), std::move(transform), std::make_index_sequence<n>{});
        }

        template<typename ReduceT, typename TransformT, std::size_t... idx>
        static constexpr T call(const vector<T, n> &a, ReduceT reduce, TransformT transform, std::index_sequence<idx...>) {
            vector<T, n> compute{.data = {transform(a.data[idx])...}};
            return reduce_op<T, n>::call(compute, std::move(reduce));
        }
    };
}// namespace detail

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
    return detail::transform_reduce_op<T, n>::call(a, b, std::plus<>{}, std::multiplies<>{});
}

template<typename T, std::size_t n>
constexpr T normSquare(const vector<T, n> &a) {
    return dot(a, a);
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
constexpr T absSin(const vector<T, n> &a, const vector<T, n> &b) {
    return norm(cross(a, b)) / std::sqrt(normSquare(a) * normSquare(b));
}

template<typename T>
constexpr T absSin(const vector<T, 2> &a, const vector<T, 2> &b) {
    return absSin(vector<T, 3>{.data = {a.data[0], a.data[1], 0}}, vector<T, 3>{.data = {b.data[0], b.data[1], 0}});
}

#endif//MATH_VECTOR_HPP
