#include "Contour.h"

#include <cmath>
#include <cassert>
#include <iostream>
#include <tuple>
#include <utility>

namespace MLToolkit
{
namespace Utility
{
namespace Contour
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

  TCoordType lerp(const TCoordType aV0, const TCoordType& aV1, TCoordType aT)
  {
    const static TCoordType unitValue = static_cast<TCoordType>(1);
    return (unitValue - aT) * aV0 + aT * aV1;
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

namespace
{
  constexpr bool kFirstPoint = true;
  constexpr bool kSecondPoint = false;

  /**
   * Look up table for controu lines, No flip - case 5
   *  o----+
   *  |/   |
   *  |   /|
   *  +----O
   */
  Point3d
  ComputeCountourInSaddleCellType5(const SurfaceCell& aCell, TCoordType aIsoline, CellSide aPreviousSide, 
                                   bool aFirstPoint)
  {
    const auto& topLeft = aCell.TopLeft();
    const auto& topRight = aCell.TopRight();  
    const auto& bottomRight = aCell.BottomRight();
    const auto& bottomLeft = aCell.BottomLeft();

    Point2d c{};

    if (aPreviousSide == CellSide::LEFT)
    {
      if (aFirstPoint)
      {
        const TCoordType t = (aIsoline - topRight.z) / (bottomRight.z - topRight.z);
        c.x = topRight.p.x; c.y = lerp(topRight.p.y, bottomRight.p.y, t);
      }
      else
      {
        const TCoordType t = (aIsoline - bottomLeft.z) / (bottomRight.z - bottomLeft.z);
        c.x = lerp(bottomLeft.p.x, bottomRight.p.x, t); c.y = bottomLeft.p.y;
      }
    }
    else if(aPreviousSide == CellSide::RIGHT)
    {
      if (aFirstPoint)
      {
        const TCoordType t = (aIsoline - topLeft.z) / (bottomLeft.z - topLeft.z);
        c.x = topLeft.p.x;  c.y = lerp(topLeft.p.y, bottomLeft.p.y, t);
      }
      else
      {
        const TCoordType t = (aIsoline - topLeft.z) / (topRight.z - topLeft.z);
        c.x = lerp(topLeft.p.x, topRight.p.x, t); c.y = topLeft.p.y;
      }      
    }
    else if (aPreviousSide == CellSide::TOP)
    { 
      if (aFirstPoint)
      {
        const TCoordType t = (aIsoline - bottomLeft.z) / (bottomRight.z - bottomLeft.z);
        c.x = lerp(bottomLeft.p.x, bottomRight.p.x, t); c.y = bottomLeft.p.y;
      }
      else
      {
        const TCoordType t = (aIsoline - topRight.z) / (bottomRight.z - topRight.z);
        c.x = topRight.p.x; c.y = lerp(topRight.p.y, bottomRight.p.y, t);
      }
    }
    else if(aPreviousSide == CellSide::BOTTOM)
    {
      if (aFirstPoint)
      {
        const TCoordType t = (aIsoline - topLeft.z) / (topRight.z - topLeft.z);
        c.x = lerp(topLeft.p.x, topRight.p.x, t); c.y = topLeft.p.y;
      }
      else
      {
        const TCoordType t = (aIsoline - topLeft.z) / (bottomLeft.z - topLeft.z);
        c.x = topLeft.p.x;  c.y = lerp(topLeft.p.y, bottomLeft.p.y, t);
      }
    }
    else
    {
      throw std::logic_error("Unexpected previosu side passed to function \
                              computing contour in saddle cell of type 5");
    }
    
    return {c.x, c.y, aIsoline};
  }

  /**
   * Look up table for controu lines, No flip - case 10
   *  +----o
   *  |   \|
   *  |\   |
   *  o----+
   */
  Point3d
  ComputeCountourInSaddleCellType10(const SurfaceCell& aCell, TCoordType aIsoline, CellSide aPreviousSide,
                                    bool aFirstPoint)
  {
    const auto& topLeft = aCell.TopLeft();
    const auto& topRight = aCell.TopRight();  
    const auto& bottomRight = aCell.BottomRight();
    const auto& bottomLeft = aCell.BottomLeft();
      
    Point2d c{};
      
    if (aPreviousSide == CellSide::LEFT)
    {
      if (aFirstPoint)
      {
        const TCoordType t = (aIsoline - topRight.z) / (bottomRight.z - topRight.z);
        c.x = topRight.p.x; c.y = lerp(topRight.p.y, bottomRight.p.y, t);
      }
      else
      {
        const TCoordType t = (aIsoline - topLeft.z) / (topRight.z - topLeft.z);
        c.x = lerp(topLeft.p.x, topRight.p.x, t); c.y = topLeft.p.y;
      }
    }
    else if (aPreviousSide == CellSide::RIGHT)
    {
      if (aFirstPoint)
      {
        const TCoordType t = (aIsoline - topLeft.z) / (bottomLeft.z - topLeft.z);
        c.x = topLeft.p.x;  c.y = lerp(topLeft.p.y, bottomLeft.p.y, t);
      }
      else
      {
        const TCoordType t = (aIsoline - bottomLeft.z) / (bottomRight.z - bottomLeft.z);
        c.x = lerp(bottomLeft.p.x, bottomRight.p.x, t); c.y = bottomLeft.p.y;
      }
    }
    else if (aPreviousSide == CellSide::TOP)
    {
      if (aFirstPoint)
      {
        const TCoordType t = (aIsoline - bottomLeft.z) / (bottomRight.z - bottomLeft.z);
        c.x = lerp(bottomLeft.p.x, bottomRight.p.x, t); c.y = bottomLeft.p.y;
      }
      else
      {
        const TCoordType t = (aIsoline - topLeft.z) / (bottomLeft.z - topLeft.z);
        c.x = topLeft.p.x;  c.y = lerp(topLeft.p.y, bottomLeft.p.y, t);
      }
    }
    else if (aPreviousSide == CellSide::BOTTOM)
    {
      if (aFirstPoint)
      {
        const TCoordType t = (aIsoline - topLeft.z) / (topRight.z - topLeft.z);
        c.x = lerp(topLeft.p.x, topRight.p.x, t); c.y = topLeft.p.y;
      }
      else
      {
        const TCoordType t = (aIsoline - topRight.z) / (bottomRight.z - topRight.z);
        c.x = topRight.p.x; c.y = lerp(topRight.p.y, bottomRight.p.y, t);
      }
    }
    else
    {
      throw std::logic_error("Unexpected previosu side passed to function \
                              computing contour in saddle cell of type 10");
    }

    return {c.x, c.y, aIsoline};
  }

  /**
   * Look up table for controu lines:
   *  o----o  +----+  
   *  |    |  |    |
   *  |    |  |    | 
   *  o--.-o  +----+
   *  case 0  case 15
   */
  template<uint16_t taIndex>
  Point3d
  ComputeContourPointInCell(const SurfaceCell& aCell, TCoordType aIsoline, CellSide /* ignored */, bool aFirstPoint)
  {
    assert(taIndex == 0 || taIndex == 15);
    return {};
  }

  /**
   * Look up table for controu lines". case 1
   *  o----o
   *  |    |
   *  |\   |
   *  +----o
   */
  template<>
  Point3d
  ComputeContourPointInCell<1>(const SurfaceCell& aCell, TCoordType aIsoline, CellSide /* ignored */, bool aFirstPoint)
  {
    const auto& topLeft = aCell.TopLeft();
    const auto& bottomRight = aCell.BottomRight();
    const auto& bottomLeft = aCell.BottomLeft();

    Point2d c{};
    if (aFirstPoint)
    {
      const TCoordType t = (aIsoline - topLeft.z) / (bottomLeft.z - topLeft.z);
      c.x = topLeft.p.x;  c.y = lerp(topLeft.p.y, bottomLeft.p.y, t);
    }
    else
    {
      const TCoordType t = (aIsoline - bottomLeft.z) / (bottomRight.z - bottomLeft.z);
      c.x = lerp(bottomLeft.p.x, bottomRight.p.x, t); c.y = bottomLeft.p.y;            
    }
    return {c.x, c.y, aIsoline};
  }

  /**
   * Look up table for controu lines..case 2
   *  o----o
   *  |    |
   *  |   /|
   *  o----+
   */
  template<>
  Point3d
  ComputeContourPointInCell<2>(const SurfaceCell& aCell, TCoordType aIsoline, CellSide /* ignored */, bool aFirstPoint)
  {
    const auto& topRight = aCell.TopRight();    
    const auto& bottomRight = aCell.BottomRight();
    const auto& bottomLeft = aCell.BottomLeft();
    
    Point2d c{};
    if (aFirstPoint)
    {
      const TCoordType t = (aIsoline - bottomLeft.z) / (bottomRight.z - bottomLeft.z);
      c.x = lerp(bottomLeft.p.x, bottomRight.p.x, t); c.y = bottomLeft.p.y;      
    }
    else
    {
      const TCoordType t = (aIsoline - topRight.z) / (bottomRight.z - topRight.z);
      c.x = topRight.p.x; c.y = lerp(topRight.p.y, bottomRight.p.y, t);      
    }
    
    return {c.x, c.y, aIsoline};
  }
  
  /**
   * Look up table for controu lines.case 3
   *  o----0
   *  |____|
   *  |    |
   *  +----+
   */
  template<>
  Point3d
  ComputeContourPointInCell<3>(const SurfaceCell& aCell, TCoordType aIsoline, CellSide /* ignored */, bool aFirstPoint)
  {
    const auto& topLeft = aCell.TopLeft();
    const auto& topRight = aCell.TopRight();  
    const auto& bottomRight = aCell.BottomRight();
    const auto& bottomLeft = aCell.BottomLeft();
    
    Point2d c{};
    if (aFirstPoint)
    {
      const TCoordType t = (aIsoline - topLeft.z) / (bottomLeft.z - topLeft.z);
      c.x = topLeft.p.x;  c.y = lerp(topLeft.p.y, bottomLeft.p.y, t);
    }
    else
    {
      const TCoordType t = (aIsoline - topRight.z) / (bottomRight.z - topRight.z);
      c.x = topRight.p.x; c.y = lerp(topRight.p.y, bottomRight.p.y, t);
    }

    return {c.x, c.y, aIsoline};
  }

  /**
   * Look up table for controu lines. case 4
   *  o----+
   *  |   \|
   *  |    |
   *  O----O
   */
  template<>
  Point3d
  ComputeContourPointInCell<4>(const SurfaceCell& aCell, TCoordType aIsoline, CellSide /* ignored */, bool aFirstPoint)
  {
    const auto& topLeft = aCell.TopLeft();
    const auto& topRight = aCell.TopRight();  
    const auto& bottomRight = aCell.BottomRight();
    
    Point2d c{};
    if (aFirstPoint)
    {
      const TCoordType t = (aIsoline - topRight.z) / (bottomRight.z - topRight.z);
      c.x = topRight.p.x; c.y = lerp(topRight.p.y, bottomRight.p.y, t);      
    }
    else
    {
      const TCoordType t = (aIsoline - topLeft.z) / (topRight.z - topLeft.z);
      c.x = lerp(topLeft.p.x, topRight.p.x, t); c.y = topLeft.p.y;
    }

    return {c.x, c.y, aIsoline};
  }

  template<>
  Point3d
  ComputeContourPointInCell<5>(const SurfaceCell& aCell, TCoordType aIsoline, CellSide aPreviousSide, bool aFirstPoint)
  {
   return aCell.IsFlipped()
          ? ComputeCountourInSaddleCellType10(aCell, aIsoline, aPreviousSide, aFirstPoint)
          : ComputeCountourInSaddleCellType5(aCell, aIsoline, aPreviousSide, aFirstPoint);
  }

  /**
   * Look up table for controu lines: case 6
   *  o----+
   *  |  | |
   *  |  | |
   *  O----+
   */
  template<>
  Point3d
  ComputeContourPointInCell<6>(const SurfaceCell& aCell, TCoordType aIsoline, CellSide /* ignored */, bool aFirstPoint)
  {
    const auto& topLeft = aCell.TopLeft();
    const auto& topRight = aCell.TopRight();  
    const auto& bottomRight = aCell.BottomRight();
    const auto& bottomLeft = aCell.BottomLeft();

    Point2d c{};
    if (aFirstPoint)
    {
      const TCoordType t = (aIsoline - bottomLeft.z) / (bottomRight.z - bottomLeft.z);
      c.x = lerp(bottomLeft.p.x, bottomRight.p.x, t); c.y = bottomLeft.p.y;      
    }
    else
    {
      const TCoordType t = (aIsoline - topLeft.z) / (topRight.z - topLeft.z);
      c.x = lerp(topLeft.p.x, topRight.p.x, t); c.y = topLeft.p.y;
    } 

    return {c.x, c.y, aIsoline};
  }

  /**
   * Look up table for controu lines, case 7
   *  o----+  
   *  |/   |
   *  |    | 
   *  +----+
   */
  template<>
  Point3d
  ComputeContourPointInCell<7>(const SurfaceCell& aCell, TCoordType aIsoline, CellSide /* ignored */, bool aFirstPoint)
  {
    const auto& topLeft = aCell.TopLeft();
    const auto& topRight = aCell.TopRight();  
    const auto& bottomLeft = aCell.BottomLeft();    

    Point2d c{};
    if (aFirstPoint)
    {
      const TCoordType t = (aIsoline - topLeft.z) / (bottomLeft.z - topLeft.z);
      c.x = topLeft.p.x;  c.y = lerp(topLeft.p.y, bottomLeft.p.y, t);
    }
    else
    {
      const TCoordType t = (aIsoline - topLeft.z) / (topRight.z - topLeft.z);
      c.x = lerp(topLeft.p.x, topRight.p.x, t); c.y = topLeft.p.y;
    }

    return {c.x, c.y, aIsoline};
  }

  /**
   * Look up table for controu lines, 
   *  +----o
   *  |/   |
   *  |    |
   *  o--.-o
   */
  template<>
  Point3d
  ComputeContourPointInCell<8>(const SurfaceCell& aCell, TCoordType aIsoline, CellSide aPreviousSide, bool aFirstPoint)
  {
    // Case 8 is a mirror of case 7 but in opposite direction.
    return ComputeContourPointInCell<7>(aCell, aIsoline, aPreviousSide, !aFirstPoint);
  }

  /**
   * Look up table for controu lines, case 9
   *  +----o
   *  |  | |
   *  |  | |
   *  +----o
   */
  template<>
  Point3d
  ComputeContourPointInCell<9>(const SurfaceCell& aCell, TCoordType aIsoline, CellSide aPreviousSide, bool aFirstPoint)
  {
    // Case 9 is a mirror of case 6 but in opposite direction.
    return ComputeContourPointInCell<6>(aCell, aIsoline, aPreviousSide, !aFirstPoint);
  }
  
  //TODO: Check correctness of computeatin here
  template<>
  Point3d
  ComputeContourPointInCell<10>(const SurfaceCell& aCell, TCoordType aIsoline, CellSide aPreviousSide, bool aFirstPoint)
  {
    return aCell.IsFlipped()
           ? ComputeCountourInSaddleCellType5(aCell, aIsoline, aPreviousSide, aFirstPoint)
           : ComputeCountourInSaddleCellType10(aCell, aIsoline, aPreviousSide, aFirstPoint);
  }

  /**
   * Look up table for controu lines, case 11
   *  +----0
   *  |   \|
   *  |    |
   *  +----+
   */
  template<>
  Point3d
  ComputeContourPointInCell<11>(const SurfaceCell& aCell, TCoordType aIsoline, CellSide aPreviousSide, bool aFirstPoint)
  {
    // Case 11 is a mirror of case 4 but in opposite direction.
    return ComputeContourPointInCell<4>(aCell, aIsoline, aPreviousSide, !aFirstPoint);
  }

  /**
   * Look up table for controu lines, case 12
   *  +----+
   *  |____|
   *  |    |
   *  O----O
   */
  template<>
  Point3d
  ComputeContourPointInCell<12>(const SurfaceCell& aCell, TCoordType aIsoline, CellSide aPreviousSide, bool aFirstPoint)
  {
    // Case 12 is a mirror of case 3 but in opposite direction.
    return ComputeContourPointInCell<3>(aCell, aIsoline, aPreviousSide, !aFirstPoint);
  }

  /**
   * Look up table for controu lines, case 13
   *  +----+
   *  |    |
   *  |   /|
   *  +----O
   */
  template<>
  Point3d
  ComputeContourPointInCell<13>(const SurfaceCell& aCell, TCoordType aIsoline, CellSide aPreviousSide, bool aFirstPoint)
  {
    // Case 13 is a mirror of case 2 but in opposite direction.
    return ComputeContourPointInCell<2>(aCell, aIsoline, aPreviousSide, !aFirstPoint);
  }

  /**
   * Look up table for controu lines, case 14
   *  +----+
   *  |    |
   *  |\   |
   *  O----+
   */
  template<>
  Point3d
  ComputeContourPointInCell<14>(const SurfaceCell& aCell, TCoordType aIsoline, CellSide aPreviousSide, bool aFirstPoint)
  {
    // Case 14 is a mirror of case 1 but in opposite direction.
    return ComputeContourPointInCell<1>(aCell, aIsoline, aPreviousSide, !aFirstPoint);
  }  

  void GetNextGridElementIndices(RowIndex& aRow, ColIndex& aCol, CellSide aPreviousSide)
  {
    switch (aPreviousSide)
    {
      case CellSide::BOTTOM: aRow++; break;
      case CellSide::LEFT:   aCol--; break;
      case CellSide::RIGHT:  aCol++; break;
      case CellSide::TOP:    aRow--; break;
      default:                       break;
    }
  }

  void Print(const TMarshingSquaresGrid& aGrid)
  {    
    std::cout << "Marshing squares grid:\n";
    for (std::size_t row = 0; row < aGrid.Rows(); ++row)
    {
      for (std::size_t col = 0; col < aGrid.Cols(); ++col)
      {
        std::cout << "Cell(row, col) = (" << row << ", " << col << "):\n";
        aGrid.GetCell(row, col).Print();
        std::cout << "\n";
      }
    }
  }
}

void Point2d::Print() const
{
  std::cout << "(x, y) = (" << x << ", " << y << ")";
}

void Point3d::Print() const
{
  std::cout << "(x, y, z) = (" << p.x << ", " << p.y << ", " << z << ")";
}

void Point3d::Reset()
{
  p.x = p.y = z = 0.0;
}

void SurfaceCell::Print() const
{
  std::cout << "(type, flipped, (top-left), [(top-right), (bottom-right), (bottom-left)]): "
       << "\n\t(" << static_cast<int>(iCellType) << ", " << (iIsFlipped ? "true" : "false")
       << ", [";

  for (auto i = 0; i < iData.size(); ++i)
  {
    iData.at(i).Print();
    if (i < (iData.size() - 1))
    {
      std::cout << ", ";
    }
  }
  std::cout << "]" << std::endl;
}

SurfaceCell::SurfaceCell(const arma::subview<TCoordType> aHeights, RowSpan aRows, ColSpan aCols, TCoordType aIsoline)
{
  assert(aHeights.n_rows == 2 && aHeights.n_cols == 2);
              //          x                           y                         z
  iData[0] = {static_cast<TCoordType>(aCols[0]), static_cast<TCoordType>(aRows[0]), aHeights(0, 0)};
  iData[1] = {static_cast<TCoordType>(aCols[1]), static_cast<TCoordType>(aRows[0]), aHeights(0, 1)};
  iData[2] = {static_cast<TCoordType>(aCols[1]), static_cast<TCoordType>(aRows[1]), aHeights(1, 1)};
  iData[3] = {static_cast<TCoordType>(aCols[0]), static_cast<TCoordType>(aRows[1]), aHeights(1, 0)};

  // counter Clockwise traverse over a cell starting from bottom left corner. 
  iCellType =             (aHeights(1, 0) < aIsoline ? 0 : 1);
  iCellType = iCellType | ((aHeights(1, 1) < aIsoline ? 0 : 1) << 1);
  iCellType = iCellType | ((aHeights(0, 1) < aIsoline ? 0 : 1) << 2);
  iCellType = iCellType | ((aHeights(0, 0) < aIsoline  ? 0 : 1) << 3);

  const auto& topLeft = TopLeft();
  const auto& topRight = TopRight();  
  const auto& bottomRight = BottomRight();
  const auto& bottomLeft = BottomLeft();

  // Value in the cell center is average of the corners
  const auto center = (topLeft.z + topRight.z + bottomLeft.z + bottomRight.z) / 4;

  iIsFlipped = ((iCellType == 5 || iCellType == 10) && (center < aIsoline));
  if (iIsFlipped)
  {
    std::cout << "\n----------";
    Print();
    std::cout << "----------" << std::endl;
  }
}

bool SurfaceCell::operator==(const SurfaceCell& aOther) const
{
  if (iCellType != aOther.iCellType || iIsFlipped != aOther.iIsFlipped)
  {
    return false;
  }
  return true;
}

bool SurfaceCell::operator!=(const SurfaceCell&  aOther) const
{
  return !(*this == aOther);
}

void SurfaceCell::ResetCellType()
{
  switch (iCellType)
  {
    // cell doesn't cross isoline
    case 0:
    case 15:
    // contains saddle point
    case 5:
    case 10:    
      break;
    default:
      // make this cell above isoline to prevent reusing it in subsequent contour computations.
      iCellType = 15;
  }
}

CellSide SurfaceCell::GetNextTraversalSide(CellSide aPreviousSide) const
{
  if (iCellType == 5)
  {
    switch (aPreviousSide)
    {
      case CellSide::LEFT:
        return (iIsFlipped ? CellSide::TOP : CellSide::BOTTOM);
      case CellSide::RIGHT:
        return (iIsFlipped ? CellSide::BOTTOM : CellSide::TOP);
      default:
        throw std::logic_error("Unexpected previous side for cell type 5 (saddle)");
    }
  }
  else if (iCellType == 10)
  {
    switch (aPreviousSide)
    {
      case CellSide::BOTTOM:
        return (iIsFlipped ? CellSide::LEFT : CellSide::RIGHT);
      case CellSide::TOP:
        return (iIsFlipped ? CellSide::RIGHT: CellSide::LEFT);
      default:
        throw std::logic_error("Unexpected previous side for cell type 10 (saddle)");
    }
  }
  else
  {
    switch(iCellType)
    {
      case 1:
      case 9:
      case 13:
        return CellSide::BOTTOM;
      case 2:
      case 3:
      case 11:
        return CellSide::RIGHT;
      case 4:
      case 6:
      case 7:
        return CellSide::TOP;
      case 8:
      case 12:
      case 14:
        return CellSide::LEFT;   
      default:
        throw std::logic_error("Invalid cell type");
    } 
  }
}

const SurfaceCell& TMarshingSquaresGrid::GetCell(const RowIndex aRow, const ColIndex aCol) const
{
  return iGrid.at(aRow * iCols + aCol);
}

SurfaceCell& TMarshingSquaresGrid::GetCell(const RowIndex aRow, const ColIndex aCol)
{
  return iGrid.at(aRow * iCols + aCol);
}

Point3d 
ContourGenerator::GetContourPoint(const SurfaceCell& aCell, TCoordType aIsoline, 
                                  CellSide aPreviousSide, bool aFirstPoint)
{
  constexpr auto kInvalidIndex = std::numeric_limits<uint16_t>::max();

  switch(aCell.Type())
  {
    case 1:
      return ComputeContourPointInCell<1>(aCell, aIsoline, aPreviousSide, aFirstPoint);
    case 2:
      return ComputeContourPointInCell<2>(aCell, aIsoline, aPreviousSide, aFirstPoint);
    case 3:
      return ComputeContourPointInCell<3>(aCell, aIsoline, aPreviousSide, aFirstPoint);
    case 4:
      return ComputeContourPointInCell<4>(aCell, aIsoline, aPreviousSide, aFirstPoint);
    case 5:
      return ComputeContourPointInCell<5>(aCell, aIsoline, aPreviousSide, aFirstPoint);
    case 6:
      return ComputeContourPointInCell<6>(aCell, aIsoline, aPreviousSide, aFirstPoint);
    case 7:
      return ComputeContourPointInCell<7>(aCell, aIsoline, aPreviousSide, aFirstPoint);
    case 8:
      return ComputeContourPointInCell<8>(aCell, aIsoline, aPreviousSide, aFirstPoint);
    case 9:
      return ComputeContourPointInCell<9>(aCell, aIsoline, aPreviousSide, aFirstPoint);
    case 10:
      return ComputeContourPointInCell<10>(aCell, aIsoline, aPreviousSide, aFirstPoint);
    case 11:
      return ComputeContourPointInCell<11>(aCell, aIsoline, aPreviousSide, aFirstPoint);
    case 12:
      return ComputeContourPointInCell<12>(aCell, aIsoline, aPreviousSide, aFirstPoint);
    case 13:
      return ComputeContourPointInCell<13>(aCell, aIsoline, aPreviousSide, aFirstPoint);
    case 14:
      return ComputeContourPointInCell<14>(aCell, aIsoline, aPreviousSide, aFirstPoint);
    default:
        return ComputeContourPointInCell<kInvalidIndex>(aCell, aIsoline, aPreviousSide, aFirstPoint);
  }
  return {};
}

TMarshingSquaresGrid
ContourGenerator::BuildMarshingSquaresGrid(const Matrix& aMeshData, TCoordType aIsoline)
{
  TCoordType minValue = aMeshData.min() - 1;
  Matrix paddedData(aMeshData.n_rows + 2, aMeshData.n_cols + 2);
  
  paddedData.fill(minValue);

  for (std::remove_const_t<decltype(aMeshData.n_rows)> row = 0, lastRow = aMeshData.n_rows;
       row < lastRow; ++row)
  {
    for (std::remove_const_t<decltype(aMeshData.n_cols)> col = 0, lastCol = aMeshData.n_cols;
         col < lastCol; ++col)
    {
      paddedData.at(row + 1, col + 1) = aMeshData.at(row, col);
    }
  }

  const decltype(paddedData.n_rows) gridRows = paddedData.n_rows - 1;
  const decltype(paddedData.n_cols) gridCols = paddedData.n_cols - 1;

  TMarshingSquaresGrid grid(gridRows, gridCols);
  
  for (std::remove_const_t<decltype(paddedData.n_rows)> row = 0, lastRow = paddedData.n_rows - 1;
       row < lastRow; ++row)
  {
    for (std::remove_const_t<decltype(paddedData.n_cols)> col = 0, lastCol = paddedData.n_cols - 1;
         col < lastCol; ++col)
    {
      SurfaceCell::RowSpan rs = {row, row + 1};
      SurfaceCell::ColSpan cs = {col, col + 1};
      grid.Emplace_back(paddedData.submat(row, col, row + 1, col + 1), 
                        std::move(rs), std::move(cs), aIsoline);
    }
  }
  
  // Print(grid);

  return grid;
}

// TODO: Debug traversing mashing squares. Algorithm gets stuck in endless loop.
// Caused by incorrect computation of the next traversing cell.
ContourGenerator::ContourPoints
ContourGenerator::GetIsolinePoints(TMarshingSquaresGrid& aGrid, const RowIndex& aRow,
                                   const ColIndex& aCol, TCoordType aIsoline)
{
  auto AreIndicesInvalid = [](const RowIndex& aRow, const ColIndex& aCol, const auto& aGrid)
                           {
                            if (aRow < 0 || aRow >= aGrid.Rows() || aCol < 0 || aCol >= aGrid.Cols())
                            {
                              return true;
                            }
                            return false;
                           };

  auto IsPaddingCell = [&aGrid](const RowIndex& aRow, const ColIndex& aCol)
                       {
                         return (aRow == 0 || aRow >= aGrid.Rows() - 1
                                 || aCol == 0 || aCol >= aGrid.Cols() - 1);
                       };

  CellSide cellSide = CellSide::NONE;
  ContourPoints path;
  RowIndex row = aRow;
  ColIndex col = aCol;
  auto& startCell = aGrid.GetCell(row, col);
   
  Contour::Point3d firstPoint, secondPoint;

  std::cout << "--algorithm start at row: " << row
            << ", col: " << col
            << ", cell type: " << static_cast<int>(startCell.Type())
            << ", flipped: " << startCell.IsFlipped() << std::endl;

  // 1. Get contour points from starting cell and add them to the contour path.
  // Note: padding cells are skiped for computing contour path.
  if (!IsPaddingCell(row, col))
  {
    firstPoint = GetContourPoint(startCell, aIsoline, cellSide, kFirstPoint);
    path.push_back(firstPoint);

    secondPoint = GetContourPoint(startCell, aIsoline, cellSide, kSecondPoint);

    if (path.empty() || secondPoint != path.back())
    {
      path.push_back(secondPoint);
    } 
  }

  // 2. Choose the next cell to process based on current cell type
  cellSide = startCell.GetNextTraversalSide(cellSide);
  GetNextGridElementIndices(row, col, cellSide);
  
  // 3. clear type of starting cell to prevent reusing it.
  startCell.ResetCellType();

  if (AreIndicesInvalid(row, col, aGrid))
  {
    return path;
  }
  
  // 5. Get the next cell
  auto* currentCell = &aGrid.GetCell(row, col);
  
  // 6. Repeat steps 1-5 until the next cell is a starting cell
  std::cout << "algorithm before iteration moves to row: " << row << ", col: " << col
            << ", cell type: " << static_cast<int>(currentCell->Type())
            << ", flipped: " << currentCell->IsFlipped() << std::endl;
  while(currentCell->CrossesIsoline())
  {
    if (!IsPaddingCell(row, col))
    {
      firstPoint = GetContourPoint(*currentCell, aIsoline, cellSide, kFirstPoint);
      if (path.empty() || firstPoint != path.back())
      {
        path.push_back(firstPoint);
      }

      secondPoint = GetContourPoint(*currentCell, aIsoline, cellSide, kSecondPoint);
      if (path.empty() || secondPoint != path.back())
      {
        path.push_back(secondPoint);
      }
    }

    cellSide = currentCell->GetNextTraversalSide(cellSide);
    GetNextGridElementIndices(row, col, cellSide);

    if (AreIndicesInvalid(row, col, aGrid))
    {
      return path;
    }

    currentCell->ResetCellType();

    currentCell = &aGrid.GetCell(row, col);
    std::cout << "algorithm moves to row: " << row << ", col: " << col
              << ", cell type: " << static_cast<int>(currentCell->Type())
              << ", flipped: " << currentCell->IsFlipped() << std::endl;
  }

  // std::cout << "Algorithm finished with found path of "
  //           << static_cast<int>(path.size()) << " segments" << std::endl;
  
  // for (row = 0; row < aGrid.Rows(); ++row)
  // {
  //   for (col = 0; col < aGrid.Cols(); ++col)
  //   {
  //     std::cout << "Elem[" << row << ", " << col
  //               << "] type: " << static_cast<int>(aGrid.GetCell(row, col).Type()) << std::endl;
  //   }
  // }
  return path;
}

ContourGenerator::Contours
ContourGenerator::GetContours(const Matrix& aMeshData, IsolineLevels aIsolines)
{
  Contours contours;

  auto RemovePadding = [](ContourPoints& aIsolibePoints)
                       {
                         const TCoordType shift = 1;

                         for (auto& point : aIsolibePoints)
                         {
                           point.p.x -= shift;
                           point.p.y -= shift;
                         }
                       };

  for (const auto& isoline : aIsolines)
  {
    std::cout << "Isoline: " << isoline << std::endl;

    // 1. Build marching squeares mesh    
    auto marshingSquaresGrid = BuildMarshingSquaresGrid(aMeshData, isoline);

    ContourPaths paths; 

    // 2. for each cell in the grid
    for (auto row = 0; row < marshingSquaresGrid.Rows(); ++row)
    {
      const auto rowIndex = RowIndex(row);
      for (auto col = 0; col < marshingSquaresGrid.Cols(); ++col)
      {
        const auto& cell = marshingSquaresGrid.GetCell(row, col);
        if (cell.CrossesIsoline() && !cell.HasSaddlePoint())
        {
          ContourPoints isolinePoints = GetIsolinePoints(marshingSquaresGrid, rowIndex, ColIndex(col), isoline);          
          if (!isolinePoints.empty())
          {
            RemovePadding(isolinePoints);
            paths.emplace_back(std::move(isolinePoints));
          }
        }
      }
    }
    // std::cout << "For isoline: " << isoline << " found contour paths: " << paths.size() << std::endl;
    // int i = 1;
    // for (const auto& path : paths)
    // {
    //   std::cout << "path: " << i << std::endl;
    //   int p = 1;
    //   for (const auto& point : path)
    //   {        
    //     std::cout << "Point " << p << ": ";
    //     point.Print();
    //     std::cout << std::endl;
    //     ++p;
    //   }
    //   ++i;
    // }
    contours.try_emplace(isoline, paths);
  }

  return contours;
}

} } }