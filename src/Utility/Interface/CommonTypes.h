#ifndef MLTOOLKIT_UTILITY_INTERFACE_COMMONTYPES_H
#define MLTOOLKIT_UTILITY_INTERFACE_COMMONTYPES_H

#include <armadillo>

namespace MLToolkit
{
namespace Utility
{
  using Matrix = arma::Mat<double>;  
  using ByteMat = arma::Mat<uint16_t>;
  using Vector = arma::vec;  
}
}

#endif // MLTOOLKIT_UTILITY_INTERFACE_COMMONTYPES_H