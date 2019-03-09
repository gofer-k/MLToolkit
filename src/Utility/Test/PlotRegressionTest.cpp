#include "CommonTypes.h"
#include "GradientDescent.h"
#include "Plot.h"

#include "gmock/gmock.h"

#include <vector>

namespace MLToolkit {
namespace Utility {
namespace Test {

class PlottingTest : public ::testing::Test
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
    return iMat.col(0);
  }

  Matrix getResults() const
  {
    return iMat.col(1);
  }

private:
  Matrix iMat;
};

TEST_F(PlottingTest, SingleParameterRegression)
{
  PrepareData("/media/gofer/Gofer/dev/My projects/MachineLearning/MLToolkit/src/Regression/SampleData/ex1data1.txt", arma::csv_ascii);

  Matrix X = getSamples();
  const Matrix y = getResults();

  // Add a column of ones to x
  X.insert_cols(0, arma::ones<Matrix>(X.n_rows, 1));
  
  Vector theta = arma::zeros(X.n_cols);

  // learnign rate factor
  constexpr double alpha = 0.01;
  constexpr uint16_t iterations = 1500;

  GradientDescent(X, y, theta, alpha, iterations);
  
  Plot::PlotRegression(X, y, X * theta, "Population in city in 10k", "Profit in 10k USD", 
                       "Single parameterized Regression");
}

} } }