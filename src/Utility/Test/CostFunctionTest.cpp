#include "CommonTypes.h"
#include "CostFunction.h"

#include "gmock/gmock.h"

#include <vector>

namespace MLToolkit {
namespace Utility {
namespace Test {

namespace
{
struct TestData
{
  TestData(double aCost, Matrix aTheta)
  : expectedCost(aCost)
  , theta(aTheta)
  {
    // transpose to make theta column vector
    arma:: inplace_trans(theta);
  }
  double expectedCost;
  Matrix theta;  
};
}

class CostFunctionTest : public ::testing::TestWithParam<TestData>
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

  Matrix getSamples() const
  {
    Matrix X = iMat.col(0);
    // Add a column of ones to x
    // Matlab version: [ones(m, 1), data(:,1)];
    X.insert_cols(0, arma::ones<Matrix>(X.n_rows, 1));

    return X;
  }

  Matrix getResults() const
  {
    return iMat.col(1);
  }

private:
  Matrix iMat;
};

TEST_P(CostFunctionTest, SingleFeature)
{
  PrepareData("/media/gofer/Gofer/dev/My projects/MachineLearning/MLToolkit/src/Regression/SampleData/ex1data1.txt", arma::csv_ascii);

  Matrix X = getSamples();
  Matrix y = getResults();
  Matrix theta = arma::zeros(2, 1);

  const auto testData = GetParam();
  EXPECT_NEAR(testData.expectedCost, CostFunction(X, y, testData.theta), 1e-2);
}

std::vector<TestData> data = 
  {
    {32.07, {0, 0}}, 
    {54.24, {-1, 2}}
  };

INSTANTIATE_TEST_CASE_P(ComputeCost, CostFunctionTest, ::testing::ValuesIn(data));

} } }