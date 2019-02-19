#ifndef MLTOOLKIT_UTILITY_INTERFACE_COSTFUNCTION_H
#define MLTOOLKIT_UTILITY_INTERFACE_COSTFUNCTION_H

#include "CommonTypes.h"

namespace MLToolkit
{
  namespace Utility
  { 
    double CostFunction(const Matrix& X, const Vector& y, const Vector& theta);
  }
}
#endif // MLTOOLKIT_UTILITY_INTERFACE_COSTFUNCTION_H