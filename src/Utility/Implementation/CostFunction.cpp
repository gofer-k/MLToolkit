#include "CostFunction.h"
#include <cassert>

namespace MLToolkit
{
namespace Utility
{
  double CostFunction(const Matrix& X, const Vector& y, const Vector& theta)
  {
    assert(theta.is_colvec());
    assert(X.n_cols == theta.size());
    
    const auto hypothesis = (X * theta) - y;

    const auto squareError = arma::square(hypothesis);

    const auto cost = arma::sum(squareError) / (2. * y.size());

    return cost;
  }
} }