# set library name
set(LIB_NAME tcl)
# genearer library
add_library(${LIB_NAME} STATIC tcl_command.cc)
# add header file
target_sources(${LIB_NAME} PRIVATE 
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/tcl_command.h
)
# set include directory
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/inc)
# set output directory
set_target_properties(${LIB_NAME} PROPERTIES
  ARCHIVE_OUTPUT_NAME_RELEASE ${LIB_NAME}
  ARCHIVE_OUTPUT_NAME_DEBUG ${LIB_NAME}_d
  ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/lib/Release
  ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/lib/Debug)