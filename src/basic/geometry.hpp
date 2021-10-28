#ifndef MATH_BASIC_GEOMETRY_HPP
#define MATH_BASIC_GEOMETRY_HPP

#include <cstdint>

namespace dim2 {
    template<typename T, template<typename, std::size_t> typename ContainerT>
    struct trectangle {
        ContainerT<T, 2> base;
        ContainerT<T, 2> size;
        T angle;
    };

    template<typename T, template<typename, std::size_t> typename ContainerT>
    struct tcircle {
        ContainerT<T, 2> base;
        T radius;
    };
}

namespace dim3 {
    template<typename T, template<typename, std::size_t> typename ContainerT>
    struct trectangle {
        ContainerT<T, 3> base;
        ContainerT<T, 2> size;
        ContainerT<T, 4> quaternion;
    };

    template<typename T, template<typename, std::size_t> typename ContainerT>
    struct tcircle {
        ContainerT<T, 2> base;
        ContainerT<T, 3> orientation;
        T radius;
    };
}

#endif//MATH_BASIC_GEOMETRY_HPP
