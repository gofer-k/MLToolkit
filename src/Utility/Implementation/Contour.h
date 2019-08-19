#ifndef MLTOOLKIT_UTILITY_CONTOUR_CONTOUR_H
#define MLTOOLKIT_UTILITY_CONTOUR_CONTOUR_H

#include "CommonTypes.h"

#include <boost/serialization/strong_typedef.hpp>

#include <array>
#include <vector>

#include <boost/serialization/strong_typedef.hpp>

#include <array>
#include <vector>

namespace MLToolkit
{
namespace Utility
{
namespace Contour
{  
  // BOOST_STRONG_TYPEDEF(std::size_t, RowIndex);
  // BOOST_STRONG_TYPEDEF(std::size_t, ColIndex);
  using RowIndex = std::size_t;
  using ColIndex = std::size_t;
  using TCoordType = double;
  struct Point2d
  {
    TCoordType x{};
    TCoordType y{};

    friend bool operator==(const Point2d aLhs, const Point2d aRhs);
    friend bool operator!=(const Point2d aLhs, const Point2d aRhs) { return !(aLhs == aRhs); }
    void Print() const;
  };

  Point2d lerp2d(const Point2d aP0, const Point2d& aP1, int y);

  struct Point3d
  {
    Point2d p;    
    TCoordType z{};

    friend bool operator==(const Point3d aLhs, const Point3d aRhs);
    friend bool operator!=(const Point3d aLhs, const Point3d aRhs) { return !(aLhs == aRhs); }
    void Print() const;
    void Reset();
  };

  enum class CellSide : uint8_t 
  {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    NONE
  };

  class SurfaceCell
  {
  public:
    using Data = std::array<Point3d, 4>;
    using RowSpan = std::array<std::size_t, 2>;
    using ColSpan = std::array<std::size_t, 2>;

    SurfaceCell(const arma::subview<TCoordType> aHeights, RowSpan aRows, ColSpan aCols, TCoordType aIsoline);

    bool operator==(const SurfaceCell& aOther) const;
    bool operator!=(const SurfaceCell& aOther) const;

    ~SurfaceCell() = default;

    void ResetCellType();
    
    Point3d TopLeft() const
    {
      return iData[0];
    }

    Point3d TopRight() const
    {
      return iData[1];
    }

    Point3d BottomRight() const
    {
      return iData[2];
    }

    Point3d BottomLeft() const
    {
      return iData[3];
    }
        
    uint8_t Type() const
    {
      return iCellType;
    }

    bool HasSaddlePoint() const
    {
      return iCellType == 5 || iCellType == 10;
    }

    bool CrossesIsoline() const
    {
      return (iCellType != 0) && (iCellType != 15);
    }

    bool IsFlipped() const
    {
      return iIsFlipped;
    }

    //! Returns a next cell side to be processed in CCW order.
    //! \note Cell types distinguishes 8 contour crossing shapes and the other 8 types are mirrors
    //! of the first 8 types. Thus when returning the first side it is arbitrary chosen
    //! which cell type from a pair is returned in CCW order and the other one from the pair returns
    //! side in CW  order. Example: cell types 7 and 8 both starts from LEFT
    //! but 7 is arbitrary chosen to return LEFT and 8 returns TOP.
    //! The same approach is used for all other cell type pairs.
    //! \param[in] aPreviousSize Previous processed cell side.
    CellSide GetNextTraversalSide(CellSide aPreviousSide) const;

    void Print() const;
  private:
    //! Denotes one of 16 cell types depending on relation between this cell corners
    //! and given isoline.
    uint8_t iCellType{15};

    //! Flag denoting if this cell is flipped. Only cell enclosing saddle point can be flipped.
    bool iIsFlipped{false};

    //! Surface cell data organized as following:
    //! top left     [xValue, yValue, zValue]
    //! top right    [xValue, yValue, zValue]
    //! bottom right [xValue, yValue, zValue]
    //! bottom left  [xValue, yValue, zValue]
    Data iData{};
  };

  class TMarshingSquaresGrid
  {
  public:
    TMarshingSquaresGrid(std::size_t aRows, std::size_t aCols)
    : iRows(aRows)
    , iCols(aCols)
    {
      iGrid.reserve(iRows * iCols);
    }

    const SurfaceCell& GetCell(const RowIndex aRow, const ColIndex aCol) const;
    SurfaceCell& GetCell(const RowIndex aRow, const ColIndex aCol);

    std::size_t Rows() const { return iRows; }
    std::size_t Cols() const { return iCols; }

    std::vector<SurfaceCell>& Data() { return iGrid; }
    
    template<typename... Args>
    void Emplace_back(Args&&... aArgs)
    {
      iGrid.emplace_back(std::forward<Args>(aArgs)...);
    }

  private:
    std::vector<SurfaceCell> iGrid;
    std::size_t iRows;
    std::size_t iCols;
  };

  /**
   * Class implementing marshing square algoruthm as described at:
   * 
   * to compute contour lines on specified regular mesh data.
   */
  class ContourGenerator
  {    
  public:
    using IsolineLevels = std::vector<TCoordType>;
    using ContourPoints = std::vector<Point3d>;
    using ContourPaths = std::vector<ContourPoints>;
    using Contours = std::map<TCoordType /* level */, ContourPaths>;

    //! Returns a list of contours for specified in c-tor isolines.
    static Contours GetContours(const Matrix& aMeshData, IsolineLevels aIsolines);

  protected:    

    static TMarshingSquaresGrid BuildMarshingSquaresGrid(const Matrix& aMeshData, TCoordType aIsoline);

    static ContourPoints GetIsolinePoints(TMarshingSquaresGrid& aGrid, const RowIndex& aRow,
                                          const ColIndex& aCol, TCoordType aIsoline);

    //! Return contour point in \a aCell at \a aIsoline level. The computed point is either
    //! the first point in the cell in CCW order or the second one.
    static Point3d 
    GetContourPoint(const SurfaceCell& aCell, TCoordType aIsoline,
                    CellSide aPreviousSide, bool aFirstPoint);
  };
}
}}
#endif
