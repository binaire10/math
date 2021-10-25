#ifndef MATH_VECTOR_HPP
#define MATH_VECTOR_HPP

#include "point.hpp"
#include <algorithm>
#include <cmath>
#include <numeric>

template<typename T, std::size_t n>
struct vector {
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
struct vector<T, 2> {
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
struct vector<T, 3> {
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

namespace detail {
    template<typename T, std::size_t n>
    struct reduce_op {
        static_assert(n != 0, "required none null vector");
        template<typename ReduceT>
        static constexpr T call(const vector<T, n> &a, ReduceT reduce) {
            T result = a[0];
            for (unsigned i = 1; i < n; ++i)
                result = reduce(result, a[i]);
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
            vector<T, n> result{.data = {transform(a[idx])...}};
            return result;
        }

        template<typename TransformT>
        static constexpr vector<T, n> call(const vector<T, n> &a, const vector<T, n> &b, TransformT transform) {
            return call(a, b, std::move(transform), std::make_index_sequence<n>{});
        }

        template<typename TransformT, std::size_t... idx>
        static constexpr vector<T, n> call(const vector<T, n> &a, const vector<T, n> &b, TransformT transform, std::index_sequence<idx...>) {
            vector<T, n> result{.data = {transform(a[idx], b[idx])...}};
            return result;
        }

        template<typename TransformT>
        static constexpr point<T, n> call(const point<T, n> &a, const vector<T, n> &b, TransformT transform) {
            return call(a, b, std::move(transform), std::make_index_sequence<n>{});
        }

        template<typename TransformT, std::size_t... idx>
        static constexpr point<T, n> call(const point<T, n> &a, const vector<T, n> &b, TransformT transform, std::index_sequence<idx...>) {
            point<T, n> result{.data = {transform(a[idx], b[idx])...}};
            return result;
        }
    };

    template<typename T, std::size_t n>
    struct transform_reduce_op {
        template<typename ReduceT, typename TransformT>
        static constexpr T call(const vector<T, n> &a, const vector<T, n> &b, ReduceT reduce, TransformT transform) {
            return reduce_op<T, n>::call(transform_op<T, n>::call(a, b, std::move(transform)), std::move(reduce));
        }

        template<typename ReduceT, typename TransformT>
        static constexpr T call(const vector<T, n> &a, ReduceT reduce, TransformT transform) {
            return reduce_op<T, n>::call(transform_op<T, n>::call(a, std::move(transform)), std::move(reduce));
        }
    };
}// namespace detail

template<typename T, typename U, std::size_t n, typename K = decltype(T() + U())>
constexpr point<K, n> apply(const point<T, n> &a, const vector<U, n> &b) {
    return detail::transform_op<T, n>::call(a, b, std::plus<>{});
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
        result[i] = a[(i + 2) % 3] * b[(i + 3) % 3] - b[(i + 2) % 3] * a[(i + 3) % 3];

    return result;
}

template<typename T, std::size_t n>
constexpr T cos(const vector<T, n> &a, const vector<T, n> &b) {
    return dot(a, b) / std::sqrt(normSquare(a) * normSquare(b));
}

template<typename T, std::size_t n>
constexpr T absSin(const vector<T, n> &a, const vector<T, n> &b) {
    return std::sqrt(normSquare(cross(a, b)) / (normSquare(a) * normSquare(b)));
}

template<typename T>
constexpr T absSin(const vector<T, 2> &a, const vector<T, 2> &b) {
    return absSin(vector<T, 3>{.data = {a[0], a[1], 0}}, vector<T, 3>{.data = {b[0], b[1], 0}});
}

#endif//MATH_VECTOR_HPP
