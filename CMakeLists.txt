cmake_minimum_required(VERSION 3.10)

project(tclClient)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/include/uic")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOMOC_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/src/moc")
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(proj.cmake)

include_directories(inc)                                # private include
include_directories(ref/x86_64/lib/tcl8.6.15/include)   # 3rd lib include
include_directories(${CMAKE_PREFIX_PATH}/include)       # Qt5 include
foreach(_module ${QT_MODULES})
  find_package(QT NAMES Qt5 REQUIRED COMPONENTS ${_module})
  find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${_module})
  include_directories(${CMAKE_PREFIX_PATH}/include/Qt${_module})
endforeach()

if(NOT CMAKE_BUILD_TYPE)
  message("Not select build type, default: Release")
  set(CMAKE_BUILD_TYPE "Release")
endif()

if(MSVC)
  set(ARCH "x86_64")
  set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /Od /MDd /Zi")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
  elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O3 /MD")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
  endif()
elseif(LINUX or MINGW)
  set(ARCH "linux64")
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
  elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  endif()
endif()

message("-- C Tool Chain: ${CMAKE_C_COMPILER}")
message("-- C++ Tool Chain: ${CMAKE_CXX_COMPILER}")
message("-- Compile Mode: ${CMAKE_BUILD_TYPE}")

set(REF_LIB_TOP_PATH ${CMAKE_SOURCE_DIR}/ref/${ARCH}/lib)

add_subdirectory(src)