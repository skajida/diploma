#include <hdrs/basis.hpp>

#include <algorithm>
#include <numeric>

constexpr size_t MAX_BASIS_SIZE = 4'194'304u;

int32_t binpow(int32_t base, int32_t exp) {
   int32_t res = 1;
   while (exp) {
       if (exp & 1) {
           res *= base;
       }
       base *= base;
       exp >>= 1;
   }
   return res;
}

uint32_t combination(uint32_t k, uint32_t n) {
    if (n < k) {
        return 0;
    }
    double res = 1;
	for (uint32_t i = 0; i < k; ++i) {
		res = res * (n - k + i + 1) / (i + 1);
    }
	return static_cast<uint32_t>(res + 0.01);
}

std::vector<TPolynomial> generateZeroBasis(uint32_t dimension, EMode expand) {
    TPolynomial multiplierOne(
        dimension,
        { TElementaryConjuction(dimension, { { 0, EState::Positive } }) }
    );

    std::vector<TPolynomial> result;
    bool mask = expand == EMode::Negative;
    result.reserve(binpow(3 + mask, dimension) - binpow(2 + mask, dimension));
    for (uint32_t i = 0; i < dimension; ++i) {
        for (uint32_t j = 0; j < dimension - i; ++j) {
            for (uint32_t l = 0; l < (mask ? dimension - i - j : 1); ++l) {
                std::vector<EMode> permutation;
                permutation.reserve(dimension);
                auto back_insert_iterator = std::back_inserter(permutation);
                std::fill_n(back_insert_iterator, i + 1, EMode::Zero);
                std::fill_n(back_insert_iterator, dimension - i - 1 - j - mask * l, EMode::One);
                std::fill_n(back_insert_iterator, j, EMode::Positive);
                std::fill_n(back_insert_iterator, mask * l, EMode::Negative);
                do {
                    TPolynomial polynomial = multiplierOne;

                    for (uint32_t k = 0; k < dimension; ++k) {
                        TPolynomial multiplierZero(
                            dimension,
                            {
                                TElementaryConjuction(dimension, { { 0, EState::Positive } }),
                                TElementaryConjuction(dimension, { { k + 1, EState::Positive } }),
                                TElementaryConjuction(dimension, { { k + 1, EState::Negative } })
                            }
                        );
                        TPolynomial multiplierPositive(
                            dimension,
                            { TElementaryConjuction(dimension, { { k + 1, EState::Positive } }) }
                        );
                        TPolynomial multiplierNegative(
                            dimension,
                            { TElementaryConjuction(dimension, { { k + 1, EState::Negative } }) }
                        );

                        switch (permutation[k]) {
                        case EMode::Zero:
                            polynomial = polynomial * multiplierZero;
                            break;
                        case EMode::One:
                            polynomial = polynomial * multiplierOne;
                            break;
                        case EMode::Positive:
                            polynomial = polynomial * multiplierPositive;
                            break;
                        case EMode::Negative:
                            polynomial = polynomial * multiplierNegative;
                        default:
                            break;
                        }
                    }
                    result.push_back(std::move(polynomial));
                } while (std::next_permutation(permutation.begin(), permutation.end()));
            }
        }
    }
    return result;
}

bool next_combination(std::vector<size_t> &perm, uint32_t n) {
    size_t k = perm.size();
    for (size_t i = k; i--; ) {
        if (perm[i] < n - k + i) {
            ++perm[i];
            for (size_t j = i + 1; j < k; ++j) {
                perm[j] = perm[j - 1] + 1;
            }
            return true;
        }
    }
    return false;
}

void TBasis::add_composition(size_t first_layer_size, size_t depth) {
    uint32_t n = first_layer_size;
    std::vector<size_t> permutation(depth);
    std::iota(permutation.begin(), permutation.end(), 0ul);
    do {
        TPolynomial sum(dimension());
        for (size_t idx : permutation) {
            sum = sum + operator[](idx);
        }
        basis_.push_back(std::move(sum));
    } while (next_combination(permutation, n));
}

TBasis::TBasis(uint32_t dimension, EMode expanded, uint32_t depth)
    : expanded_(expanded == EMode::Negative)
    , depth_(1)
    , basis_(generateZeroBasis(dimension, expanded))
    , conj_to_idx_([this] () {
        std::map<TElementaryConjuction, std::vector<size_t>> result;
        for (size_t i = 0; i < basis_.size(); ++i) {
            for (const TElementaryConjuction &it : operator[](i)) {
                result[it].push_back(i);
            }
        }
        return result;
    }())
{
    for (size_t i = 1; i < depth; ++i) {
        expand();
    }
}

std::set<size_t> TBasis::getIndices(const TElementaryConjuction &src) const {
    const std::vector<size_t> &indices = conj_to_idx_.at(src);
    return std::set<size_t> (indices.begin(), indices.end());
}

std::set<size_t> TBasis::getIndices(const TPolynomial &src) const {
    std::set<size_t> result;
    for (const TElementaryConjuction &it : src) {
        result.merge(getIndices(it));
    }
    return result;
}

bool TBasis::expand(bool force) {
    size_t first_layer_size = expanded_ ? binpow(4, dimension()) - binpow(3, dimension())
                                        : binpow(3, dimension()) - binpow(2, dimension());
    if (force || combination(depth_ + 1, first_layer_size) + size() < MAX_BASIS_SIZE) {
        size_t prev_size = size();
        add_composition(first_layer_size, ++depth_);
        for (size_t i = prev_size; i < size(); ++i) {
            for (const TElementaryConjuction &it : operator[](i)) {
                conj_to_idx_[it].push_back(i);
            }
        }
        return true;
    }
    return false;
}

const TPolynomial& TBasis::operator[](size_t idx) const noexcept {
    return basis_[idx];
}

TBasis::const_iterator TBasis::begin() const noexcept {
    return basis_.begin();
}

TBasis::const_iterator TBasis::end() const noexcept {
    return basis_.end();
}

uint32_t TBasis::dimension() const noexcept {
    return basis_.front().dimension();
}

uint32_t TBasis::size() const noexcept {
    return basis_.size();
}
