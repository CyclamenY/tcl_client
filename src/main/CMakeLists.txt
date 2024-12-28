include(../../proj.cmake)

set(REF_LIB ${REF_LIB_TOP_PATH}/tcl8.6.15/lib/tcl86tg.lib 
            ${REF_LIB_TOP_PATH}/tcl8.6.15/lib/tclstub86.lib)

# add executable
if(ARCH STREQUAL "x86_64")
  add_executable(${PROJECT_NAME} WIN32 main.cc)
else()
  add_executable(${PROJECT_NAME} main.cc)
endif()
# link library
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  link_directories(${CMAKE_SOURCE_DIR}/lib/Debug)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  link_directories(${CMAKE_SOURCE_DIR}/lib/Release)
endif()
target_link_libraries(${PROJECT_NAME} ${modules})
target_link_libraries(${PROJECT_NAME} ${REF_LIB})
foreach(_module ${QT_MODULES})
  target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::${_module})
endforeach()

# set include path
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/inc)
# set output name and dir
set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_NAME_RELEASE ${PROJECT_NAME}
  RUNTIME_OUTPUT_NAME_DEBUG ${PROJECT_NAME}-debug
  RUNTIME_OUTPUT_DIRECTORY $<IF:$<CONFIG:Debug>,${CMAKE_BINARY_DIR}/bin,${CMAKE_BINARY_DIR}/bin>)

# copy dll
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${CMAKE_SOURCE_DIR}/ref/${ARCH}/dll" 
  "${CMAKE_BINARY_DIR}/bin"
  COMMENT "Copying dll files to ${CMAKE_BINARY_DIR}/bin"
)
foreach(_dll ${QT_DLLS})
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
      "\"${CMAKE_PREFIX_PATH}/bin/Qt${QT_VERSION_MAJOR}${_dll}d.dll\""
      "\"${CMAKE_BINARY_DIR}/bin\""
    )
  elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
      "\"${CMAKE_PREFIX_PATH}/bin/Qt${QT_VERSION_MAJOR}${_dll}.dll\""
      "\"${CMAKE_BINARY_DIR}/bin\""
  )
  endif()
endforeach()