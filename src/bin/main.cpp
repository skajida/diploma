#include <hdrs/generator.hpp>
#include <hdrs/parallel.hpp>

int main() {
    TGenerator gen(418);
    std::vector<TPolynomial> test_set = gen.execute({.dimension=4, .length=13, .quantity=100});
    TBasis zero_basis(test_set.front().dimension(), EMode::Negative, 5);
    TParallelDecreaser decreaser;
    auto decreased = decreaser.execute(test_set, zero_basis);

    size_t source_length = 0, result_length = 0;
    for (const auto &[src, res] : decreased) {
        std::cout << src << '\t' << res << '\n';
        source_length += src.size();
        result_length += res.size();
    }
    std::cout << 100 * (1 - static_cast<double>(result_length) / source_length) << '%' << std::endl;

    return 0;

    // uint32_t dimension;
    // std::cin >> dimension;
    // TPolynomial polynomial(dimension);
    // std::cin >> polynomial;
    //
    // TBasis zero_basis(polynomial.dimension(), EMode::Negative, 5);
    // TCoordinateDescentMinimizer minimizer(polynomial, zero_basis);
    // TPolynomial shrunk = minimizer.execute();
    //
    // std::cout << shrunk << std::endl;
    // return 0;
}
