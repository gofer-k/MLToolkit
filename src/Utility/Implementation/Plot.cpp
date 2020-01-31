#include "Plot.h"
#include <sstream>
#include <iostream>

namespace MLToolkit
{
namespace Utility
{
namespace Plot
{
  std::string PrepareLevels(const Vector& aLevels)
  {
    std::stringstream ss;

    for (std::remove_const_t<decltype(aLevels.n_elem)> i = 0, last = aLevels.size(); i < last; ++i)
    {
      ss << aLevels.at(i);
      if (i < (last - 1))
      {
        ss << ",";
      }
    }
    return ss.str();
  }
  void PlotContours(GnuplotPipe& aPlotter, const std::string& aFileName, const Vector& aLevels)
  {
    aPlotter.setContourMode();
    aPlotter.sendLine("set cntrparam levels discrete " + PrepareLevels(aLevels));
    aPlotter.setTitle("Cost function(theta1, theta2)");

    std::stringstream plotCommand;
    
    plotCommand << "splot \"" << aFileName << "\" using 1:2:3";
    aPlotter.sendLine(plotCommand.str());    
  }

  void PlotRegression(const Matrix& aX, const Matrix& aY, const Matrix& aPrediction, 
                      std::string aLabelX, std::string aLabelY, std::string aTitle)
  {
    const std::string filename {"/tmp/plotRegression.dat"};

    Matrix plotMat = arma::join_rows(aX, aY);
    plotMat.insert_cols(plotMat.n_cols, aPrediction);

    if (!plotMat.save(filename, arma::raw_ascii))
    {
      std::cerr << "\nSaving matrix for file" << filename << " failed";
      return;
    }
    GnuplotPipe gp;
  
    std::stringstream plotCommand;
    plotCommand << "plot \"" << filename << "\" using 2:3 title \'Training set\'," << "\"" << filename << "\" using 2:4 title \'Prediction\' with lines";

    gp.setLabelX(aLabelX);
    gp.setLabelY(aLabelY);
    gp.setTitle(aTitle);
    gp.sendLine(plotCommand.str());
  }
  void PlotCostFunction(const Vector& aTheta1, const Vector& aTheta2, const Matrix& aJ)
  {
    std::stringstream gridData;
    GnuplotPipe gp;

    for (std::size_t row = 0; row < aTheta1.n_rows; ++row)
    {      
      for (std::size_t col = 0; col < aTheta2.n_rows; ++col)
      {
        gridData << aTheta1.at(row) << "\t" << aTheta2.at(col) << "\t" << aJ.at(row, col);        
        gp.sendLine(gridData.str(), true);
        gridData.str("");
      }
      gp.sendLine("", true);
    }

    const std::string filename {"/tmp/plotCost.dat"};
    gp.writeBufferToFile(filename);
    // 15 levels logarithmically spaced from 0.01 to 1000
    const auto isoLevels = arma::logspace(-2, 3, 15);

    PlotContours(gp, filename, isoLevels);
  }
  
}}}