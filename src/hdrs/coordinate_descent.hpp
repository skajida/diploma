#pragma once

#include <hdrs/basis.hpp>

class TCoordinateDescentMinimizer {
private:
    TPolynomial current_;
    const TBasis &basis_;

public:
    TCoordinateDescentMinimizer(TPolynomial, const TBasis&);

    TPolynomial execute();
};
