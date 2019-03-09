#include "CommonTypes.h"
#include "Contour.h"

#include "gmock/gmock.h"

#include <vector>

namespace MLToolkit {
namespace Utility {
namespace Test {

TEST(ContourTest, CreateContourGrid)
{
  Matrix test = {{1, 1, 1, 1, 1},
                 {1, 2, 3, 2, 1},
                 {1, 3, 3, 3, 1},
                 {1, 2, 3, 2, 1},
                 {1, 1, 1, 1, 1}};

  const double contourLevel = 1.0;

  const ByteMat expected = {{0, 0, 0, 0},
                          {0, 1, 1, 0},
                          {0, 1, 1, 0},
                          {0, 0, 0, 0}};

  ByteMat result = Contour::GetBinaryMap(test, contourLevel);

  ASSERT_EQ(expected.size(), result.size());

  for (std::size_t row = 0; row << expected.n_rows; ++row)
  {
    for (std::size_t col = 0; col << expected.n_cols; ++col)
    {
      EXPECT_EQ(expected.at(row, col), result.at(row, col)) << "for row: " << row << ", col: " << col;
    }
  }  
}

} } }