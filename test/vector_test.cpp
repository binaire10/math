#include <iostream>
#include <tpoint.hpp>
#include <tvector.hpp>


int main() {
    constexpr auto a = vector::make_vector<float>(0, 1);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            auto b = vector::make_vector<float>(i * 2 - 1, j * 2 - 1);
            std::cout << cos(a, b) << ' ' << absSin(a, b) << std::endl;
        }
    }
    constexpr auto c = apply(make_point<float>(1, 1), a);
    std::cout << c.x << ' ' << c.y << '\n';

    auto k = dot(vector::make_vector<float>(0, 1), vector::make_vector<float>(3, 2));
    return 0;
}