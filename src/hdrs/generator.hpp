#pragma once

#include <hdrs/basis.hpp>

#include <random>
#include <ctime>

struct TParams {
    uint32_t dimension;
    size_t length;
    size_t quantity;
};

class TGenerator {
private:
    mutable std::mt19937 gen_;

public:
    explicit TGenerator(int64_t seed=time(nullptr));

    std::vector<TPolynomial> execute(TParams) const;
};
