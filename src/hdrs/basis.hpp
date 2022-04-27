#pragma once

#include <hdrs/polynomial.hpp>

#include <map>
#include <set>

enum class EMode {
    Zero,
    One,
    Positive,
    Negative
};

int32_t binpow(int32_t base, int32_t exp);
uint32_t combination(uint32_t k, uint32_t n);

std::vector<TPolynomial> generateZeroBasis(uint32_t dimension, EMode expand);

class TBasis {
    using const_iterator = std::vector<TPolynomial>::const_iterator;
    using iterator = std::vector<TPolynomial>::iterator;
private:
    const bool expanded_;
    uint32_t depth_;
    std::vector<TPolynomial> basis_;
    std::map<TElementaryConjuction, std::vector<size_t>> conj_to_idx_;

    void add_composition(size_t first_layer_size, size_t depth);

public:
    TBasis(uint32_t dimension, EMode expanded=EMode::Positive, uint32_t depth=1);

    std::set<size_t> getIndices(const TElementaryConjuction&) const;
    std::set<size_t> getIndices(const TPolynomial&) const;

    bool expand(bool force=false);

    const TPolynomial& operator[](size_t) const noexcept;

    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;

    uint32_t dimension() const noexcept;
    uint32_t size() const noexcept;
};
