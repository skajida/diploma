#pragma once

#include <hdrs/elementary_conjuction.hpp>

class TPolynomial {
    using const_iterator = std::vector<TElementaryConjuction>::const_iterator;
    using iterator = std::vector<TElementaryConjuction>::iterator;
private:
    const uint32_t dim_;
    std::vector<TElementaryConjuction> polynomial_;

    iterator begin() noexcept;
    iterator end() noexcept;

    void clear();
    void normalize();
public:
    explicit TPolynomial(uint32_t dim);
    TPolynomial(const TPolynomial&);
    TPolynomial(TPolynomial&&);
    TPolynomial(uint32_t dim, std::vector<TElementaryConjuction>);

    bool evaluate(uint32_t vector) const;
    std::string vector() const;

    TPolynomial& operator=(const TPolynomial&);
    TPolynomial operator+(const TPolynomial&) const;
    bool operator<(const TPolynomial&) const;

    TPolynomial operator*(const TElementaryConjuction&) const;
    TPolynomial operator*(const TPolynomial&) const;

    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;

    uint32_t dimension() const noexcept;
    uint32_t size() const noexcept;

    friend std::istream& operator>>(std::istream&, TPolynomial&);
    friend std::ostream& operator<<(std::ostream&, const TPolynomial&);
};
