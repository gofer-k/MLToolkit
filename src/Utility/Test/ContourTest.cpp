#include "CommonTypes.h"
#include "Contour.h"

#include "gmock/gmock.h"

#include <vector>

namespace MLToolkit {
namespace Utility {
namespace Contour {

std::ostream& operator<<(std::ostream& aOut, const Point3d& aPathPoint)
{
  aPathPoint.Print();
  aOut << std::endl;
  return aOut;
}

std::ostream& operator<<(std::ostream& aOut, const ContourGenerator::ContourPoints& aPath)
{
  for (const auto& pathPoint : aPath)
  {
    aOut << "\n  Path point: ";
    aOut << pathPoint;
  }
  return aOut;
}

std::ostream& operator<<(std::ostream& aOut, const ContourGenerator::ContourPaths& aPaths)
{
  for (const auto& path : aPaths)
  {
    aOut << "\nPath:";
    aOut << path;    
  }
  return aOut;
}

namespace Test {

namespace 
{
  using ::testing::Each;
  using ::testing::SizeIs;
}

class ContourTest : public Contour::ContourGenerator, public ::testing::Test
{};

// ________
// |  /\  |
// | /  \ |
// | \  / |
// |  \/  |
// -------- 
TEST_F(ContourTest, CreateContourGrid_DiamondCase)
{
  Matrix test = {{1, 1, 1, 1, 1},
                 {1, 2, 3, 2, 1},
                 {1, 3, 3, 3, 1},
                 {1, 2, 3, 2, 1},
                 {1, 1, 1, 1, 1}};

  const ContourGenerator::IsolineLevels isoLevels = {1, 2, 3};
  const auto isolines = ContourGenerator::GetContours(test, isoLevels);

  EXPECT_EQ(isoLevels.size(), isolines.size());
  
  ASSERT_NO_THROW(isolines.at(1));
  {
    const auto& isoLinePaths = isolines.at(1);
    EXPECT_TRUE(isoLinePaths.empty()) << "For isoline at level 1: " << isoLinePaths.size() << ", points: " << isoLinePaths.front().size();
  }

  ASSERT_NO_THROW(isolines.at(3));
  {
    const auto& isoLinePaths = isolines.at(3);
    ASSERT_FALSE(isoLinePaths.empty()) << "For isoline at level 3: ";
    EXPECT_EQ(5, isoLinePaths.back().size()) << "For isoline at level 3: ";
  }
}

// ___________
// |/ /\__/\ \|
// |  \    /  |
// |  / __ \  |
// |\ \/  \/ /|
// ------------
TEST_F(ContourTest, CreateContourGrid_ButterflyCase)
{
  Matrix test = {{3, 0, 0, 0, 3},
                 {0, 3, 2, 3, 0},
                 {0, 2, 3, 2, 0},
                 {0, 3, 2, 3, 0},
                 {3, 0, 0, 0, 3}};

  const ContourGenerator::IsolineLevels isoLevels = {1, 2, 3};
  const auto isolines = ContourGenerator::GetContours(test, isoLevels);

  EXPECT_EQ(isoLevels.size(), isolines.size());
  
  ASSERT_NO_THROW(isolines.at(1));
  {
    const auto& isoLinePaths = isolines.at(1);
    EXPECT_FALSE(isoLinePaths.empty());
    EXPECT_EQ(1, isoLinePaths.size()) << "for isoline at level 1: ";
    EXPECT_EQ(20, isoLinePaths.front().size()) << "for isoline at level 1 path points: ";
  }
  
  ASSERT_NO_THROW(isolines.at(2));
  {
    const auto& isoLinePaths = isolines.at(2);
    EXPECT_FALSE(isoLinePaths.empty());    

    std::vector<size_t> expectedContourPoints = {2, 2, 13, 2, 2};

    ASSERT_EQ(expectedContourPoints.size(), isoLinePaths.size()) << "for isoline at level 2: ";

    for (size_t i = 0; i < isoLinePaths.size(); ++i)
    {
      EXPECT_EQ(expectedContourPoints.at(i), isoLinePaths.at(i).size()) << "for isoline path " << i;
    }
  }

  ASSERT_NO_THROW(isolines.at(3));
  {
    const auto& isoLinePaths = isolines.at(3);

    EXPECT_EQ(8, isoLinePaths.size()) << "for isoline at level 3: ";
    EXPECT_THAT(isoLinePaths, ::testing::Each(SizeIs(1)));
  }
}


} } } }