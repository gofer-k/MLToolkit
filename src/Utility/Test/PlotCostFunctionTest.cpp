#include "CommonTypes.h"
#include "CostFunction.h"
#include "Plot.h"

#include "gmock/gmock.h"

#include <vector>

namespace MLToolkit {
namespace Utility {
namespace Test {

class PlotCostFunctionTest : public ::testing::Test
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

#include <iostream>

TEST_F(PlotCostFunctionTest, CostFunction)
{
  PrepareData("/media/gofer/Gofer/dev/My projects/MachineLearning/MLToolkit/src/Regression/SampleData/ex1data1.txt", arma::csv_ascii);

  Matrix X = getSamples();
  const Matrix y = getResults();

  // Add a column of ones to x
  X.insert_cols(0, arma::ones<Matrix>(X.n_rows, 1));
  
  // Grid over which we will calculate J
  Vector theta0_vals = arma::linspace(-10, 10, 100);
  Vector theta1_vals = arma::linspace(-1, 4, 100);

  // initialize J_vals to a matrix of 0's
  Matrix J_vals = arma::zeros(theta0_vals.size(), theta1_vals.size());

  // Fill out J_vals
  for (decltype(theta0_vals.size()) i = 0, rowCount = theta0_vals.size(); i < rowCount; ++i)
  {
    for (decltype(theta1_vals.size()) k = 0, colCount = theta1_vals.size(); k < colCount; ++k)
    {
      Vector theta = {theta0_vals.at(i), theta1_vals.at(k)};
      J_vals(i, k) = CostFunction(X, y, theta);
    }	  
  }
  
  // Because of the way meshgrids work in the surf command, we need to
  // transpose J_vals before calling surf, or else the axes will be flipped
  Plot::PlotCostFunction(theta0_vals, theta1_vals, J_vals.t());
}

} } }