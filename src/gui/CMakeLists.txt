# set library name
set(LIB_NAME gui)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/inc/gui/ui/")
set(CMAKE_AUTOUIC_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/include/gui/uic/")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOMOC_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/src/gui/moc/")
set(CMAKE_AUTORCC ON)

# generate library
add_library(${LIB_NAME} STATIC gui.cc main_window.cc)
# add ui file
#qt5_wrap_ui(UI_HEADERS ui/main_window.ui)
# add header file
target_sources(${LIB_NAME} PRIVATE 
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/main_window.h
  ${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/gui.h
  #${UI_HEADERS}
  #${CMAKE_SOURCE_DIR}/inc/${LIB_NAME}/uic/ui_main_window.h
)
# set include directory
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/inc)
# set output directory
set_target_properties(${LIB_NAME} PROPERTIES
  ARCHIVE_OUTPUT_NAME_RELEASE ${LIB_NAME}
  ARCHIVE_OUTPUT_NAME_DEBUG ${LIB_NAME}_d
  ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/Release
  ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/Debug)