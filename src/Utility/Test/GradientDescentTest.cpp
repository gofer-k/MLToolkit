#include "CommonTypes.h"
#include "GradientDescent.h"

#include "gmock/gmock.h"

#include <ostream>
#include <vector>

namespace MLToolkit {
namespace Utility {
namespace Test {

namespace
{
struct TestData
{
  TestData(double aAplha, std::size_t aNumIters, Vector aTheta)
  : alpha(aAplha)
  , iterations(aNumIters)
  , expectedTheta(aTheta)
  {
    // transpose to make theta column vector
    // arma::inplace_trans(expectedTheta);
  }

  double alpha = {0.01};
  std::size_t iterations = {1500};  
  Vector expectedTheta;  
};
}

class GradientDescentTest : public ::testing::TestWithParam<TestData>
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

TEST_P(GradientDescentTest, SingleFeature)
{
  PrepareData("/media/gofer/Gofer/dev/My projects/MachineLearning/MLToolkit/src/Regression/SampleData/ex1data1.txt", arma::csv_ascii);

  Matrix X = getSamples();
  Matrix y = getResults();
  Vector theta = arma::zeros(2);

  const auto testData = GetParam();
  const auto J = GradientDescent(X, y, theta, testData.alpha, testData.iterations);  
  EXPECT_EQ(testData.iterations, J.n_rows);
  
  std::size_t index = 0;
  
  testData.expectedTheta.for_each([&](const decltype(testData.expectedTheta)::elem_type& elem)
  {
    EXPECT_NEAR(elem, theta.at(index), 1e-2) << " at index: " << index;
    ++index;
  });  
}

std::vector<TestData> data = 
  {
    {0.01, 1500, {-3.6303, 1.1664}}
  };

INSTANTIATE_TEST_CASE_P(ComputeGradientDescent, GradientDescentTest, ::testing::ValuesIn(data));

} } }