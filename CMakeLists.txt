cmake_minimum_required(VERSION 3.0.2)
project(MLToolkit)

 set(CMAKE_C_COMPILER "/usr/bin/clang")
 set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
 # Installaton folder for built targets. TO install a target run 'make install' command.
set(BINARY_DESTINATION_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

 include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake) 
 conan_basic_setup()

 add_definitions("-std=c++17")

 find_package(Armadillo REQUIRED)
 
 add_executable(${PROJECT_NAME} main.cpp)

 target_include_directories(${PROJECT_NAME} 
   PRIVATE
    ${ARMADILLO_INCLUDE_DIRS}
  )

target_link_libraries(${PROJECT_NAME} 
  PRIVATE
   ${CONAN_LIBS} 
   ${ARMADILLO_LIBRARIES}
 )

 add_subdirectory(GnuPlotWrapper)
 add_subdirectory(src)