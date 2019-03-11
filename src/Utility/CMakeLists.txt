cmake_minimum_required(VERSION 3.0.2)
project(MLToolkit.Utility)

find_package(Boost 1.67 COMPONENTS serialization REQUIRED)

add_library(${PROJECT_NAME} 
    Implementation/Contour.cpp
    Implementation/Contour.h
    Implementation/CostFunction.cpp
    Implementation/GradientDescent.cpp
    Implementation/Plot.cpp  
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    Interface
    ${ARMADILLO_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
   # add any thrid party library which is referenced in test defined in this project
   MLToolkit.GnuPlotWrapper
   ${ARMADILLO_LIBRARIES}
)

# Installation folder. Run 'make install' to install this project in specified folder
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION ${BINARY_DESTINATION_DIRECTORY})

add_subdirectory(Test)