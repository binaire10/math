#include "tvector.hpp"
#include <assert.h>
#include <functional>
#include <iostream>

int main() {
    using namespace vector::operand;

    for (int i = -10; i <= 10; ++i)
        for (int j = -10 ; j <= 10; ++j)
            for (int k = -10; k <= 10; ++k) {
                auto v = vector::make_vector<float>(i, j, k);
                assert(v.x == i and v.y == j and v.z == k);
            }
    for (int i = -10; i <= 10; ++i)
        for (int j = -10 ; j <= 10; ++j)
            for (int k = -10; k <= 10; ++k)
                for (int l = -10; l <= 10; ++l) {
                    auto u = vector::make_vector<float>(k, l);
                    auto v = vector::make_vector<float>(i, j);
                    auto r1 = u + v;
                    assert(r1.x == (i + k) and r1.y == (j + l));
                    auto r2 = u - v;
                    assert(r2.x == (k - i) and r2.y == (l - j));
                }
}