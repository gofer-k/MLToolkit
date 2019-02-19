#ifndef MLTOOLKIT_UTILITY_INTERFACE_GRTADIENTDESCENT_H
#define MLTOOLKIT_UTILITY_INTERFACE_GRTADIENTDESCENT_H

#include "CommonTypes.h"

namespace MLToolkit
{
namespace Utility
{
  Vector GradientDescent(const Matrix& X, const Vector& y, Vector& theta, 
                         double alpha, uint16_t numIters);
}
}
#endif // MLTOOLKIT_UTILITY_INTERFACE_GRTADIENTDESCENT_H