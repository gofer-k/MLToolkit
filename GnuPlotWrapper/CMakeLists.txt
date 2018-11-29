project(MLToolkit.GnuPlotWrapper)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
  # Headers in Interface folder are public library interface
  INTERFACE Interface
)

target_link_libraries(${PROJECT_NAME}
  # add any thrid party library which is referenced in test defined in this project
)

# Installation folder. Run 'make install' to install this project in specified folder
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION ${BINARY_DESTINATION_DIRECTORY})