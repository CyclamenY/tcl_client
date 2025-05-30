cmake_minimum_required(VERSION 3.10)

project(tclClient)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O3 /MD")
  endif()
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
elseif(LINUX or MINGW)
  set(ARCH "linux64")
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
  elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  endif()
endif()

include(proj.cmake)

include_directories(inc)                                # private include
# 3rd lib include
include_directories(${REF_LIB_TOP_PATH}/tcl8.6.15/include)
include_directories(${REF_LIB_TOP_PATH}/serial/include)

include_directories(${CMAKE_PREFIX_PATH}/include)       # Qt5 include
foreach(_module ${QT_MODULES})
  find_package(QT NAMES Qt5 REQUIRED COMPONENTS ${_module})
  find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${_module})
  include_directories(${CMAKE_PREFIX_PATH}/include/Qt${_module})
endforeach()

message(STATUS "C Tool Chain: ${CMAKE_C_COMPILER}")
message(STATUS "C++ Tool Chain: ${CMAKE_CXX_COMPILER}")
message(STATUS "Compile Mode: ${CMAKE_BUILD_TYPE}")

# get current git info
get_git_branch(GIT_BRANCH)
message(STATUS "Current Git Branch: ${GIT_BRANCH}")
get_git_hash(GIT_TAG)
message(STATUS "Current Git Hash: ${GIT_TAG}")

add_subdirectory(src)