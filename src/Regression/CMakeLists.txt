project(MLToolkit.Regression)

add_library(${PROJECT_NAME}
  Implementation/Regression.cpp
)

target_include_directories(${PROJECT_NAME}
  # Headers in Interface folder are public library interface
  PUBLIC Interface

  # Make Implementation folder not exposed outside library
  PRIVATE Implementation ${OCTAVE_INCLUDE_DIR}  
)

target_link_libraries(${PROJECT_NAME}
  # add any thrid party library which is referenced in test defined in this project
  ${ARMADILLO_LIBRARIES}
)

# Installation folder. Run 'make install' to install this project in specified folder
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION ${BINARY_DESTINATION_DIRECTORY}) 