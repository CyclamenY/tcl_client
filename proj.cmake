# define var
if(NOT DEFINED QT_ROOT_DIR)
  set(QT_ROOT_DIR "C:/code_env/Qt/msvc2022_x64")  # for native build
endif()
set(CMAKE_PREFIX_PATH "${QT_ROOT_DIR}")
set(MY_PATH "${QT_ROOT_DIR}/bin;C:/code_env/tcl8.6.15/bin")
set(QT_QPA_PLATFORM_PLUGIN_PATH "${QT_ROOT_DIR}/plugins/platforms")

set(QT_MODULES "Widgets" "Core")
set(QT_DLLS "Widgets" "Gui" "Core")
set(REF_LIB_TOP_PATH ${CMAKE_SOURCE_DIR}/ref/${ARCH}/lib)

# define marco
macro(get_git_hash _git_hash)
  find_package(Git QUIET)
  if(GIT_FOUND)
    execute_process(COMMAND 
      git describe --tags --always
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_TAG
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_QUIET)
  endif()
endmacro()

macro(get_git_branch _git_branch)
  find_package(Git QUIET)
  if(GIT_FOUND)
    execute_process(
      COMMAND git rev-parse --abbrev-ref HEAD
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_BRANCH
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  endif()
endmacro()

set(modules event gui tcl log utility serial gtest gmock)