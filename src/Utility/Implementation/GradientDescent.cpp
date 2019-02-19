#include "CostFunction.h"
#include "GradientDescent.h"

namespace MLToolkit
{
namespace Utility
{
//! Performs gradient descent to learn theta
//! J = GradientDescent(X, y, theta, alpha, numIters) updates theta by taking num_iters gradient 
//! steps with learning rate alpha.
Vector GradientDescent(const Matrix& X, const Vector& y, Vector& theta, 
                       double alpha, uint16_t numIters)
{
  

//! number of training examples
const auto m = y.size();
const auto lengthOfTheta = theta.size();

Vector J_history = arma::zeros(numIters);
Vector gradientDescent = arma::zeros(lengthOfTheta);

for (uint16_t iter = 0; iter < numIters; ++iter)
{
  const auto hypothesis = (X * theta) - y;

  for (std::size_t i = 0; i < gradientDescent.size(); ++i)
  {
    gradientDescent.at(i) = theta.at(i) - (alpha * arma::sum(hypothesis % X.col(i))) / m;
  }
    
  theta = gradientDescent;

  // Save the cost J in every iteration    
  J_history.at(iter) = CostFunction(X, y, theta);
}

 return J_history;
}                       

} }