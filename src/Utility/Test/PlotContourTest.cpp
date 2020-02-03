#include "CommonTypes.h"
#include "Contour.h"
#include "Plot.h"

#include "gmock/gmock.h"

#include <string>
#include <vector>

namespace MLToolkit {
namespace Utility {
namespace Test {

// TODO:
// 1. For raw data set stored in a file in grid format use splot with enabled contour and disabled surface 
// 2. For contour recognition algorithm (Marching Square) output in grid format,
//    isoline paths separated (by double empty line) draw with plot.
class PlotContoursTest : public ::testing::Test
{
  public:
  void SetUp() override {}
  void TearDown() override {}

  void PrepareData(std::string filename, arma::file_type fileType)
  {
    if (!iMat.load(filename, fileType))
    {
      ADD_FAILURE() << "Failed to load data from file: " << filename;
    }
  }

  int SaveGridData(const Matrix& aData, const std::string& aFileName) const;

  Matrix getSamples() const
  {    
    return iMat.col(0);
  }

  Matrix getResults() const
  {
    return iMat.col(1);
  }

private:
  Matrix iMat;
};

TEST_F(PlotContoursTest, Isolines)
{
  Matrix gridData = {{3, 2, 1, 2, 3},
                     {2, 3, 2, 3, 2},
                     {1, 2, 3, 2, 1},
                     {2, 3, 2, 3, 2},
                     {3, 2, 1, 2, 3}};

  const Contour::ContourGenerator::IsolineLevels isoLevels = {1, 2, 3};
  const auto contours = Contour::ContourGenerator::GetContours(gridData, isoLevels);

  TIsolines isolines;

  std::size_t numberOfIsolines = 0;
  for (const auto& isoLevel : contours)
  {
    numberOfIsolines += isoLevel.second.size();
  }

  isolines.reserve(numberOfIsolines);

  for (const auto& isoLevel : contours)
  {
    for (const auto& isoline : isoLevel.second)
    {
      TIsoline outputIsoline;
      outputIsoline.reserve(isoline.size());
      for (const auto& isolinePoint : isoline)
      {
        outputIsoline.push_back({isolinePoint.p.x, isolinePoint.p.y});
      }
      isolines.push_back(std::move(outputIsoline));
    }
  }

  Plot::PlotIsolines(isolines);
}

} } }