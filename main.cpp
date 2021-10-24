#include "src/point.hpp"
#include "src/vector.hpp"
#include <iostream>


int main() {
    constexpr auto a = make_vector<float>(1.0f, 1.0f), b = make_vector<float>(0.0f, 1.0f);
    constexpr auto c = make_vector<float>(1.0f, 1.0f, 0.0f), d = make_vector<float>(0.0f, 1.0f, 0.0f);
    constexpr auto e = cross(make_vector<float>(1, 0, 0), make_vector<float>(1, 1, 0));

    std::cout << cos(a, b) << ' ' << cos(c, d) << std::endl;
    std::cout << sin(a, b) << ' ' << sin(c, d) << std::endl;
    std::cout << e.x << ' ' << e.y << ' ' << e.z;

    return 0;
}