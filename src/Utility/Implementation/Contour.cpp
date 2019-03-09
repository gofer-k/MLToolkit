#include "Contour.h"

namespace MLToolkit
{
namespace Utility
{
namespace Contour
{
  ByteMat GetBinaryMap(const Matrix& aInputGrid, double aContourLevel)
  {
    ByteMat cellMat(aInputGrid.n_rows - 1, aInputGrid.n_cols - 1);

    for (auto row = 0, nextRow = row + 1; nextRow < aInputGrid.n_rows; ++row, ++nextRow)
    {
      for (auto col = 0, nextCol = col + 1; nextCol < aInputGrid.n_cols; ++col, ++nextCol)
      {
        try
        {
          uint8_t cellType = 0;

          // counter Clockwise traverse over a cell starting from topleft corner
          cellType = aInputGrid(row, nextCol) < aContourLevel ? 0 : 1;
          cellType = cellType | ((aInputGrid(nextRow, nextCol) < aContourLevel ? 0 : 1) << 1);
          cellType = cellType | ((aInputGrid(row, nextCol) < aContourLevel ? 0 : 1) << 2);
          cellType = cellType | ((aInputGrid(row, col) < aContourLevel ? 0 : 1) << 3);

          cellMat(row, col) = cellType;
        }
        catch(const std::logic_error& e)
        {
          std::cerr << e.what() << '\n';
        }      
      }
    }
    return cellMat;
  }

}}}