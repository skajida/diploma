#pragma once

#include <hdrs/basis.hpp>

#include <future>
#include <utility>

class TParallelDecreaser {
private:
    mutable std::vector<std::future<std::pair<TPolynomial, TPolynomial>>> targets_;

    std::pair<TPolynomial, TPolynomial> shrink(const TPolynomial&, const TBasis&) const;

public:
    TParallelDecreaser();

    std::vector<std::pair<TPolynomial, TPolynomial>> execute(
        const std::vector<TPolynomial> &source,
        const TBasis&
    ) const;
};
