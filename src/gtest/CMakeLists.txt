# set library name
set(LIB_NAME gtest)
# genearer library
add_library(${LIB_NAME} STATIC
  gtest.cc
  gtest-assertion-result.cc
  gtest-death-test.cc
  gtest-filepath.cc
  gtest-matchers.cc
  gtest-port.cc
  gtest-printers.cc
  gtest-test-part.cc
  gtest-typed-test.cc
)
# add header file
target_sources(${LIB_NAME} PRIVATE 
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/gtest.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/gtest_pred_impl.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/gtest_prod.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/gtest-assertion-result.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/gtest-death-test.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/gtest-matchers.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/gtest-message.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/gtest-param-test.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/gtest-printers.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/gtest-spi.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/gtest-test-part.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/gtest-typed-test.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/internal/gtest-death-test-internal.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/internal/gtest-filepath.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/internal/gtest-internal.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/internal/gtest-param-util.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/internal/gtest-port.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/internal/gtest-port-arch.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/internal/gtest-string.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/internal/gtest-type-util.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/internal/custom/gtest.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/internal/custom/gtest-port.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/internal/custom/gtest-printers.h
)
# set include directory
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/inc)
# set output directory
set_target_properties(${LIB_NAME} PROPERTIES
  ARCHIVE_OUTPUT_NAME_RELEASE ${LIB_NAME}
  ARCHIVE_OUTPUT_NAME_DEBUG ${LIB_NAME}_d
  ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/Release
  ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/Debug)