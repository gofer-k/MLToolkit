cmake_minimum_required(VERSION 3.0.2)

project(MLToolkit.Utility.Test)

set(GMOCK_DIR "$ENV{GMOCK_DIR}")
set(GTEST_DIR "$ENV{GTEST_DIR}")

MESSAGE( STATUS "Target artifacts will be installed in: " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
MESSAGE( STATUS "Using GTest from: " ${GTEST_DIR} )
MESSAGE( STATUS "Using GMock from: " ${GMOCK_DIR} )

include_directories(${PROJECT_NAME}
  PRIVATE
  "${GMOCK_DIR}/include"
  "${GTEST_DIR}/include"
  "../Interface"
  "../Implementation")

link_directories(
  "${GMOCK_DIR}/build"
  "${GMOCK_DIR}/build/gtest"
  ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

add_executable(${PROJECT_NAME} )

target_sources(${PROJECT_NAME} 
  PRIVATE 
    ContourTest.cpp
    CostFunctionTest.cpp
    GradientDescentTest.cpp
    GTestMain.cpp
    LinearInterpolation2DTest.cpp
    PlotContourTest.cpp
    PlotCostFunctionTest.cpp
    PlotRegressionTest.cpp
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    gmock
    gtest
    pthread
    MLToolkit.Utility
)
  
# Installation folder. Run 'make install' to install this project in specified folder
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${BINARY_DESTINATION_DIRECTORY})