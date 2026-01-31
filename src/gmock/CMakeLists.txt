# set library name
set(LIB_NAME gmock)
# genearer library
add_library(${LIB_NAME} STATIC
  gmock.cc
  gmock-cardinalities.cc
  gmock-internal-utils.cc
  gmock-matchers.cc
  gmock-spec-builders.cc
)
# add header file
target_sources(${LIB_NAME} PRIVATE 
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/gmock.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/gmock-actions.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/gmock-cardinalities.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/gmock-function-mocker.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/gmock-matchers.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/gmock-more-actions.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/gmock-more-matchers.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/gmock-nice-strict.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/gmock-spec-builders.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/internal/gmock-internal-utils.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/internal/gmock-port.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/internal/gmock-pp.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/internal/custom/gmock-generated-actions.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/internal/custom/gmock-matchers.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/internal/custom/gmock-port.h
)
# set include directory
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/inc)
# set output directory
set_target_properties(${LIB_NAME} PROPERTIES
  ARCHIVE_OUTPUT_NAME_RELEASE ${LIB_NAME}
  ARCHIVE_OUTPUT_NAME_DEBUG ${LIB_NAME}_d
  ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/Release
  ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/Debug)