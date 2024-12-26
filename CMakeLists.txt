cmake_minimum_required(VERSION 3.10)

project(tclClient)

include_directories(inc)
include_directories(ref/x86_64/lib/tcl8.6.15/include)

if(NOT CMAKE_BUILD_TYPE)
  message("Not select build type, default: Release")
  set(CMAKE_BUILD_TYPE "Release")
endif()

if(MSVC)
  set(ARCH "x86_64")
  set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /Od /MDd /Zi")
  elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O3")
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