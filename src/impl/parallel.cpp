#include <hdrs/parallel.hpp>
#include <hdrs/coordinate_descent.hpp>

TParallelDecreaser::TParallelDecreaser() : targets_() {}

std::pair<TPolynomial, TPolynomial> TParallelDecreaser::shrink(
    const TPolynomial &source,
    const TBasis &zero_basis
) const {
    TCoordinateDescentMinimizer minimizer(source, zero_basis);
    return std::make_pair(source, minimizer.execute());
}

std::vector<std::pair<TPolynomial, TPolynomial>> TParallelDecreaser::execute(
    const std::vector<TPolynomial> &source,
    const TBasis &zero_basis
) const {
    std::vector<std::pair<TPolynomial, TPolynomial>> result;
    result.reserve(source.size());
    for (const TPolynomial &it : source) {
        targets_.push_back(
            std::async(&TParallelDecreaser::shrink, this, std::ref(it), std::ref(zero_basis))
        );
    }
    for (auto &it : targets_) {
        result.push_back(std::move(it.get()));
    }
    return result;
}
