# 设置变量
set(modules event)
set(REF_LIB ${REF_LIB_TOP_PATH}/tcl8.6.15/lib/tcl86tg.lib 
            ${REF_LIB_TOP_PATH}/tcl8.6.15/lib/tclstub86.lib)

# 添加可执行文件
add_executable(${PROJECT_NAME} main.cc)
# 链接私有库
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  link_directories(${CMAKE_SOURCE_DIR}/lib/Debug)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  link_directories(${CMAKE_SOURCE_DIR}/lib/Release)
endif()
target_link_libraries(${PROJECT_NAME} ${modules})
target_link_libraries(${PROJECT_NAME} ${REF_LIB})
# target_link_libraries(${PROJECT_NAME} event)
# target_link_libraries(${PROJECT_NAME} ${REF_LIB_TOP_PATH}/tcl8.6.15/lib/tcl86tg.lib)
# target_link_libraries(${PROJECT_NAME} ${REF_LIB_TOP_PATH}/tcl8.6.15/lib/tclstub86.lib)
# 指定头文件路径
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/inc)
# 设置最终的可执行文件输出位置
set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_NAME_RELEASE ${PROJECT_NAME}
  RUNTIME_OUTPUT_NAME_DEBUG ${PROJECT_NAME}-debug
  RUNTIME_OUTPUT_DIRECTORY $<IF:$<CONFIG:Debug>,${CMAKE_BINARY_DIR}/bin,${CMAKE_BINARY_DIR}/bin>)

# 拷贝dll文件到生成目录
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${CMAKE_SOURCE_DIR}/ref/${ARCH}/dll" 
  "${CMAKE_BINARY_DIR}/bin"
  COMMENT "Copying dll files to ${CMAKE_BINARY_DIR}/bin"
)