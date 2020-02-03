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
  using TCoordType = double;

  struct Point2d
  {
    TCoordType x{};
    TCoordType y{};

    friend bool operator==(const Point2d aLhs, const Point2d aRhs);
    friend bool operator!=(const Point2d aLhs, const Point2d aRhs) { return !(aLhs == aRhs); }
  };

  struct Point3d
  {
    Point2d p;    
    TCoordType z{};

    friend bool operator==(const Point3d aLhs, const Point3d aRhs);
    friend bool operator!=(const Point3d aLhs, const Point3d aRhs) { return !(aLhs == aRhs); }
  };

  using TIsoline = std::vector<Point2d>;
  using TIsolines = std::vector<TIsoline>;
}
}

#endif // MLTOOLKIT_UTILITY_INTERFACE_COMMONTYPES_H