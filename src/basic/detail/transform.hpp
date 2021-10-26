#ifndef MATH_TRANSFORM_HPP
#define MATH_TRANSFORM_HPP

namespace t_detail {
    template<typename T, std::size_t n, template<typename, std::size_t> typename ContainerT>
    struct reduce_op {
        static_assert(n != 0, "required none null vector");
        template<typename ReduceT>
        static constexpr T call(const ContainerT<T, n> &a, ReduceT reduce) {
            T result = a[0];
            for (unsigned i = 1; i < n; ++i)
                result = reduce(result, a[i]);
            return result;
        }
    };

    template<typename T, std::size_t n, template<typename, std::size_t> typename ContainerT>
    struct transform_op {
        static_assert(n != 0, "required none null vector");
        template<typename TransformT>
        static constexpr ContainerT<T, n> call(const ContainerT<T, n> &a, TransformT transform) {
            return call(a, std::move(transform), std::make_index_sequence<n>{});
        }

        template<typename TransformT, std::size_t... idx>
        static constexpr ContainerT<T, n> call(const ContainerT<T, n> &a, TransformT transform, std::index_sequence<idx...>) {
            ContainerT<T, n> result{.data = {transform(a[idx])...}};
            return result;
        }

        template<typename TransformT>
        static constexpr ContainerT<T, n> call(const ContainerT<T, n> &a, const ContainerT<T, n> &b, TransformT transform) {
            return call(a, b, std::move(transform), std::make_index_sequence<n>{});
        }

        template<typename TransformT, std::size_t... idx>
        static constexpr ContainerT<T, n> call(const ContainerT<T, n> &a, const ContainerT<T, n> &b, TransformT transform, std::index_sequence<idx...>) {
            ContainerT<T, n> result{.data = {transform(a[idx], b[idx])...}};
            return result;
        }
    };

    template<typename T, std::size_t n, template<typename, std::size_t> typename ContainerT>
    struct transform_reduce_op {
        template<typename ReduceT, typename TransformT>
        static constexpr T call(const ContainerT<T, n> &a, const ContainerT<T, n> &b, ReduceT reduce, TransformT transform) {
            return reduce_op<T, n, ContainerT>::call(transform_op<T, n, ContainerT>::call(a, b, std::move(transform)), std::move(reduce));
        }

        template<typename ReduceT, typename TransformT>
        static constexpr T call(const ContainerT<T, n> &a, ReduceT reduce, TransformT transform) {
            return reduce_op<T, n, ContainerT>::call(transform_op<T, n, ContainerT>::call(a, std::move(transform)), std::move(reduce));
        }
    };
}// namespace t_detail
#endif//MATH_TRANSFORM_HPP
