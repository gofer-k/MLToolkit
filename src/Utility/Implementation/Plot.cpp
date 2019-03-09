#include "Plot.h"

namespace MLToolkit
{
namespace Utility
{
namespace Plot
{
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
#include <ostream>
  void PlotCostFunction(const Matrix& aTheta1, const Matrix& aTheta2, const Matrix& aJ)
  {
    const std::string filename {"/tmp/plotCost.dat"};

    const Vector J = arma::vectorise(aJ); 
    Matrix plotMat(aJ.size(), 3);
    std::size_t indexInJ = 0;
    std::size_t rowIndex = 0;

    for (std::size_t row = 0; row < aTheta1.n_rows; ++row)
    {
      for (std::size_t col = 0; col < aTheta2.n_rows; ++col)
      {        
        plotMat.at(rowIndex, 0) = aTheta1.at(row);
        plotMat.at(rowIndex, 1) = aTheta2.at(col);
        plotMat.at(rowIndex, 2) = J.at(indexInJ++);
        ++rowIndex;
      }
    }
    
    if (!plotMat.save(filename, arma::raw_ascii))
    {
      std::cerr << "\nSaving matrix for file" << filename << " failed";
      return;
    }
    std::stringstream plotCommand;
    plotCommand << "splot \"" << filename << "\" using 1:2:3";

    GnuplotPipe gp;    
    gp.sendLine(plotCommand.str());
    gp.setTitle("Cost function(theta1, theta2)");
        
    gp.setContourMode();
  }

}}}