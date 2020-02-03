#include "CommonTypes.h"

#include <cmath>
#include <iostream>

namespace MLToolkit
{
namespace Utility
{
bool operator==(const Point2d aLhs, const Point2d aRhs)
{
  static const double eps = 1e-6;

  if (std::fabs(aLhs.x - aRhs.x) >= eps)
  {
    return false;
  }
  if (std::fabs(aLhs.y - aRhs.y) >= eps)
  {
    return false;
  }
  return true;
}

bool operator==(const Point3d aLhs, const Point3d aRhs)
{
  static const double eps = 1e-6;

  if (aLhs.p != aRhs.p)
  {
    return false;
  }
  if (std::fabs(aLhs.z - aRhs.z) >= eps)
  {
    return false;
  }
  return true;
}

}}