#ifndef MATH_TVECTOR_HPP
#define MATH_TVECTOR_HPP

#include "tpoint.hpp"
#include <cmath>
#include <functional>

#include <basic/detail/transform.hpp>
namespace vector {
    template<typename T, std::size_t n>
    struct tvector {
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

        std::array<T, n> data;
    };

    namespace detail {
        template<typename T, std::size_t n>
        using transform_op = t_detail::transform_op<T, n, tvector>;
        template<typename T, std::size_t n>
        using transform_reduce_op = t_detail::transform_reduce_op<T, n, tvector>;
        template<typename T, std::size_t n>
        using reduce_op = t_detail::reduce_op<T, n, tvector>;
    }// namespace detail

    template<typename T>
    struct tvector<T, 2> {
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
                T x, y;
            };
            std::array<T, 2> data;
        };
    };

    template<typename T>
    struct tvector<T, 3> {
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


    template<typename T, std::size_t n>
    constexpr tpoint<T, n> apply(const tpoint<T, n> &a, const tvector<T, n> &b) {
        return t_detail::transform_op<T, n, tpoint>::call(a, tpoint<T, n>{.data = b.data}, std::plus<>{});
    }

    template<typename T, typename... ArgT>
    constexpr tvector<T, sizeof...(ArgT)> make_vector(ArgT... v) {
        return tvector<T, sizeof...(ArgT)>{.data = {static_cast<T>(v)...}};
    }

    template<typename T, std::size_t n>
    constexpr T dot(const tvector<T, n> &a, const tvector<T, n> &b) {
        return detail::transform_reduce_op<T, n>::call(a, b, std::plus<>{}, std::multiplies<>{});
    }

    template<typename T, std::size_t n>
    constexpr T normSquare(const tvector<T, n> &a) {
        return dot(a, a);
    }

    template<typename T, std::size_t n>
    constexpr T norm(const tvector<T, n> &a) {
        return std::sqrt(normSquare(a));
    }

    template<typename T>
    constexpr tvector<T, 3> cross(const tvector<T, 3> &a, const tvector<T, 3> &b) {
        tvector<T, 3> result{.data = {}};
        for (unsigned i{}; i < 3; ++i)
            result[i] = a[(i + 2) % 3] * b[(i + 3) % 3] - b[(i + 2) % 3] * a[(i + 3) % 3];

        return result;
    }

    template<typename T, std::size_t n>
    constexpr T cos(const tvector<T, n> &a, const tvector<T, n> &b) {
        return dot(a, b) / std::sqrt(normSquare(a) * normSquare(b));
    }

    template<typename T, std::size_t n>
    constexpr T absSin(const tvector<T, n> &a, const tvector<T, n> &b) {
        return std::sqrt(normSquare(cross(a, b)) / (normSquare(a) * normSquare(b)));
    }

    template<typename T>
    constexpr T absSin(const tvector<T, 2> &a, const tvector<T, 2> &b) {
        return absSin(tvector<T, 3>{.data = {a[0], a[1], 0}}, tvector<T, 3>{.data = {b[0], b[1], 0}});
    }

    using std::signbit;


    namespace operand {

        // Scalar Operand

        template<typename T, std::size_t n>
        constexpr tvector<T, n> operator-(const T &a, const tvector<T, n> &b) {
            return detail::transform_op<T, n>::call(b, std::bind(std::minus<>{}, a, std::placeholders::_1));
        }

        template<typename T, std::size_t n>
        constexpr tvector<T, n> operator-(const tvector<T, n> &a, const T &b) {
            return detail::transform_op<T, n>::call(a, std::bind(std::minus<>{}, std::placeholders::_1, b));
        }

        template<typename T, std::size_t n>
        constexpr tvector<T, n> operator+(const T &a, const tvector<T, n> &b) {
            return detail::transform_op<T, n>::call(b, std::bind(std::plus<>{}, a, std::placeholders::_1));
        }

        template<typename T, std::size_t n>
        constexpr tvector<T, n> operator+(const tvector<T, n> &a, const T &b) {
            return detail::transform_op<T, n>::call(a, std::bind(std::plus<>{}, std::placeholders::_1, b));
        }

        template<typename T, std::size_t n>
        constexpr tvector<T, n> operator*(const T &a, const tvector<T, n> &b) {
            return detail::transform_op<T, n>::call(b, std::bind(std::multiplies<>{}, a, std::placeholders::_1));
        }

        template<typename T, std::size_t n>
        constexpr tvector<T, n> operator*(const tvector<T, n> &a, const T &b) {
            return detail::transform_op<T, n>::call(a, std::bind(std::multiplies<>{}, std::placeholders::_1, b));
        }

        // Vector operator

        template<typename T, std::size_t n>
        constexpr tvector<T, n> operator+(const tvector<T, n> &a, const tvector<T, n> &b) {
            return detail::transform_op<T, n>::call(a, b, std::plus<>{});
        }

        template<typename T, std::size_t n>
        constexpr tvector<T, n> operator-(const tvector<T, n> &a, const tvector<T, n> &b) {
            return detail::transform_op<T, n>::call(a, b, std::minus<>{});
        }
    }// namespace operand
}// namespace vector
#endif//MATH_TVECTOR_HPP
