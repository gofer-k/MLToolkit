#ifndef MLTOOLKIT_UTILITY_INTERFACE_PLOT_H
#define MLTOOLKIT_UTILITY_INTERFACE_PLOT_H

#include "CommonTypes.h"
#include "gnuplot.h"

#include <iostream>
#include <sstream>
#include <string>

namespace MLToolkit
{
namespace Utility
{
namespace Plot
{
  void PlotRegression(const Matrix& aX, const Matrix& aY, const Matrix& aPrediction, 
                      std::string aLabelX, std::string aLabelY, std::string aTitle);

  void PlotCostFunction(const Vector& aTheta1, const Vector& aTheta2, const Matrix& aJ);

}}}

#endif