#ifndef MLTOOLKIT_UTILITY_INTERFACE_PLOT_H
#define MLTOOLKIT_UTILITY_INTERFACE_PLOT_H

#include "CommonTypes.h"
#include "gnuplot.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace MLToolkit
{
namespace Utility
{
namespace Plot
{
  void PlotRegression(const Matrix& aX, const Matrix& aY, const Matrix& aPrediction, 
                      std::string aLabelX, std::string aLabelY, std::string aTitle);

  void PlotCostFunction(const Vector& aTheta1, const Vector& aTheta2, const Matrix& aJ);

  //! \a aGridData must be 3 column matrix, where a row denotes a point on an isoline in 3-D space.
  //! Disjoined isolines must be separated with a row of NaN values.
  void PlotIsolines(const Utility::TIsolines& aIsolines);
}}}

#endif