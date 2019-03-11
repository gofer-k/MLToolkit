#include "Contour.h"

#include "gmock/gmock.h"

namespace MLToolkit {
namespace Utility {
namespace Contour {
namespace Test {

struct TestData
{
  Point2d p1;
  Point2d p2;
  double  level;
};

const TestData kTestData[] = {
  // Vertical case
  {{.x = 0, .y = 0}, {.x = 0, .y = 100}, 50},
  // Horizontal case
  {{.x = 0, .y = 0}, {.x = 100, .y = 0}, 0},
  // climbing y case
  {{.x = 0, .y = 0}, {.x = 100, .y = 100}, 50},
  // Lowering y case
  {{.x = 0, .y = 100}, {.x = 100, .y = 0}, 50},
  // climbing x case
  {{.x = 0, .y = 0}, {.x = 100, .y = 0}, 0},
  // Lowering x case
  {{.x = 100, .y = 0}, {.x = 0, .y = 0}, 0}
  };

class LinearInterpolation2dTest : public ::testing::TestWithParam<TestData>
{};

TEST_P(LinearInterpolation2dTest, lerp2d)
{

}

INSTANTIATE_TEST_CASE_P(ComputeInterpolatedPoint, LinearInterpolation2dTest,
                        ::testing::ValuesIn(kTestData));

} } } }