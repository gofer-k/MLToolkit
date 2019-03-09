#ifndef CONTOUR_H
#define CONTOUR_H

#include "CommonTypes.h"

namespace MLToolkit
{
namespace Utility
{
namespace Contour
{
  ByteMat GetBinaryMap(const Matrix& aInputGrid, double aContourLevel);
}
}}
#endif
