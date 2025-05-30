if(ARCH STREQUAL "x86_64")
  set(REF_LIB ${REF_LIB_TOP_PATH}/tcl8.6.15/lib/tcl86tg.lib 
              ${REF_LIB_TOP_PATH}/tcl8.6.15/lib/tclstub86.lib
              ${REF_LIB_TOP_PATH}/serial/serial.lib)
endif()

# add executable
add_executable(${PROJECT_NAME} main.cc)
# link library
link_directories(${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE})
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

# set dll path
set_property(TARGET ${PROJECT_NAME} PROPERTY 
  VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${MY_PATH};QT_QPA_PLATFORM_PLUGIN_PATH=${QT_QPA_PLATFORM_PLUGIN_PATH}")
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
# add definitions
add_definitions(-DGIT_TAG="${GIT_TAG}" -DGIT_BRANCH="${GIT_BRANCH}")

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_SOURCE_DIR}/script/pre_build.bat
  "${CMAKE_SOURCE_DIR}/src/main/param.def"
  COMMENT "Copying dll files to ${CMAKE_BINARY_DIR}/bin"
)

# copy dll
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy_directory
#   "${CMAKE_SOURCE_DIR}/ref/${ARCH}/dll" 
#   "${CMAKE_BINARY_DIR}/bin"
#   COMMENT "Copying dll files to ${CMAKE_BINARY_DIR}/bin"
# )
# foreach(_dll ${QT_DLLS})
#   if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#     add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#       COMMAND ${CMAKE_COMMAND} -E copy
#       "\"${CMAKE_PREFIX_PATH}/bin/Qt${QT_VERSION_MAJOR}${_dll}d.dll\""
#       "\"${CMAKE_BINARY_DIR}/bin\""
#     )
#   elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
#     add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#       COMMAND ${CMAKE_COMMAND} -E copy
#       "\"${CMAKE_PREFIX_PATH}/bin/Qt${QT_VERSION_MAJOR}${_dll}.dll\""
#       "\"${CMAKE_BINARY_DIR}/bin\""
#   )
#   endif()
# endforeach()