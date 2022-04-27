#include <hdrs/generator.hpp>

TGenerator::TGenerator(int64_t seed) : gen_(seed) {}

EState mapping(uint32_t source) {
    switch (source) {
    case 0b00:
        return EState::Empty;
    case 0b01:
        return EState::Positive;
    case 2:
    default:
        return EState::Negative;
    }
}

std::vector<TPolynomial> TGenerator::execute(TParams parameters) const {
    const auto &[dimension, length, quantity] = parameters;
    std::vector<TPolynomial> result;
    result.reserve(quantity);

    std::normal_distribution<double> norm_len(length, 1);                           // length
    std::uniform_int_distribution<uint32_t> uni_conj(0, binpow(3, dimension));      // conj_seed

    for (size_t i = 0; i < quantity; ++i) {
        TPolynomial polynomial(dimension);
        size_t len = static_cast<size_t>(std::round(norm_len(gen_)));
        while (polynomial.size() != len) {
            std::vector<TVariable> conj_init;
            uint32_t conj_seed = uni_conj(gen_);
            for (uint32_t k = 0; k < dimension; ++k) {
                if (uint32_t remainder = conj_seed % 3; remainder) {
                    conj_init.push_back({ k + 1, mapping(remainder) });
                }
                conj_seed /= 3;
            }
            if (conj_init.empty()) {
                conj_init.push_back({ 0, EState::Positive });
            }
            polynomial = polynomial + TPolynomial(
                dimension,
                { TElementaryConjuction(dimension, std::move(conj_init)) }
            );
        }
        result.push_back(std::move(polynomial));
    }
    return result;
}
