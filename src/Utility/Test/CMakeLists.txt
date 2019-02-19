cmake_minimum_required(VERSION 3.0.2)

project(MLToolkit.Utility.Test)

set(GMOCK_DIR "$ENV{GMOCK_DIR}")
set(GTEST_DIR "$ENV{GTEST_DIR}")

MESSAGE( STATUS "Target artifacts will be installed in: " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
MESSAGE( STATUS "Using GTest from: " ${GTEST_DIR} )
MESSAGE( STATUS "Using GMock from: " ${GMOCK_DIR} )

set(IMPLEMENTATION_SOURCES
  CostFunctionTest.cpp
  GradientDescentTest.cpp
  GTestMain.cpp
)

include_directories(
  "${GMOCK_DIR}/include"
  "${GTEST_DIR}/include"
  "../Interface")

link_directories(
  "${GMOCK_DIR}/build"
  "${GMOCK_DIR}/build/gtest"
  ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

add_executable(${PROJECT_NAME} ${IMPLEMENTATION_SOURCES})

add_dependencies(${PROJECT_NAME} MLToolkit.Utility)

target_link_libraries(${PROJECT_NAME}  
  gtest
  gmock
  pthread
  MLToolkit.Utility
  # add any thrid party library which is referenced in test defined in this project
)

# Installation folder. Run 'make install' to install this project in specified folder
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${BINARY_DESTINATION_DIRECTORY})