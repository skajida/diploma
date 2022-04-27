#include <hdrs/coordinate_descent.hpp>

#include <algorithm>
#include <numeric>

TCoordinateDescentMinimizer::TCoordinateDescentMinimizer(TPolynomial init, const TBasis &basis)
    : current_(std::move(init))
    , basis_(basis) {}

TPolynomial TCoordinateDescentMinimizer::execute() {
    while ([this] () {
        size_t prev_size = current_.size();
        std::map<size_t, TPolynomial> reductions;
        do {
            reductions.clear();
            for (size_t idx : basis_.getIndices(current_)) {
                TPolynomial sum(current_ + basis_[idx]);
                if (sum < current_) {
                    reductions.emplace(std::make_pair(current_.size() - sum.size(), std::move(sum)));
                }
            }
            if (!reductions.empty()) {
                const auto &[reduct, result] = *reductions.rbegin();
                // std::cerr << reduct << '\t' << result << std::endl;
                current_ = result;
            }
        } while (!reductions.empty());
        return current_.size() < prev_size;
    }());
    return current_;
}
