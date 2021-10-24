#include <point.hpp>
#include <vector.hpp>
#include <iostream>


int main() {
    constexpr auto a = make_vector<float>(0, 1);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            auto b = make_vector<float>(i*2-1, j*2-1);
            std::cout << cos(a, b) << ' ' << absSin(a, b) << std::endl;
        }
    }

    return 0;
}