import os
import configparser

def write_module_cmake_content(target_path, module_name):
  cmake_path = os.path.join(target_path, 'CMakeLists.txt')
  conf_path = os.path.join(target_path, module + '.conf')

  config = configparser.ConfigParser()
  config.read(conf_path)
  srcs_str = config.get('DEFAULT', 'sources', fallback=None)
  
  cmake_file = open(cmake_path, 'w')
  cmake_file.write('# 设置lib名\n')
  cmake_file.write('set(LIB_NAME '+ module_name + ')\n')
  cmake_file.write('# 构建静态库\n')
  cmake_file.write('add_library(${LIB_NAME} STATIC ' + srcs_str + ')\n')
  cmake_file.write('# 指定库头文件路径\n')
  cmake_file.write('target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/inc)\n')
  cmake_file.write('# 设置库文件输出目录\n')
  cmake_file.write('set_target_properties(${LIB_NAME} PROPERTIES\n'
   '  ARCHIVE_OUTPUT_NAME_RELEASE ${LIB_NAME}\n'
   '  ARCHIVE_OUTPUT_NAME_DEBUG ${LIB_NAME}_d\n'
   '  ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/lib/Release\n'
   '  ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/lib/Debug)\n')
  cmake_file.close()

def write_main_cmake_content(target_path, module_list):
  conf_path = os.path.join(target_path, 'main.conf')

  config = configparser.ConfigParser()
  config.read(conf_path)
  lib_str = config.get('DEFAULT', 'links', fallback=None)
  lib_list = lib_str.split()

  cmake_path = os.path.join(target_path, 'CMakeLists.txt')
  cmake_file = open(cmake_path, 'w')
  cmake_file.write('# 添加可执行文件\n')
  cmake_file.write('add_executable(${PROJECT_NAME} main.cc)\n')
  cmake_file.write('# 链接库\n')
  cmake_file.write('if(CMAKE_BUILD_TYPE STREQUAL "Debug")\n')
  cmake_file.write('  link_directories(${CMAKE_SOURCE_DIR}/lib/Debug)\n')
  cmake_file.write('elseif(CMAKE_BUILD_TYPE STREQUAL "Release")\n')
  cmake_file.write('  link_directories(${CMAKE_SOURCE_DIR}/lib/Release)\n')
  cmake_file.write('endif()\n')
  for module in module_list:
    cmake_file.write('target_link_libraries(${PROJECT_NAME} ' + module + ')\n')
  for lib in lib_list:
    cmake_file.write('target_link_libraries(${PROJECT_NAME} ${REF_LIB_TOP_PATH}/' + lib + ')\n')
  cmake_file.write('# 指定头文件路径\n')
  cmake_file.write('target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/inc)\n')
  cmake_file.write('# 设置最终的可执行文件输出位置\n')
  cmake_file.write('set_target_properties(${PROJECT_NAME} PROPERTIES\n'
   '  RUNTIME_OUTPUT_NAME_RELEASE ${PROJECT_NAME}\n'
   '  RUNTIME_OUTPUT_NAME_DEBUG ${PROJECT_NAME}-debug\n'
   '  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)\n')
  cmake_file.close()


# 获取conf路径
current_dir = os.path.dirname(os.path.abspath(__file__))
parent_dir = os.path.dirname(current_dir)
proj_conf_path = os.path.join(parent_dir, 'proj.conf')

# 创建 ConfigParser 对象
config = configparser.ConfigParser()
config.read(proj_conf_path)
modules_str = config.get('DEFAULT', 'modules', fallback=None)
modules_list = modules_str.split()

src_path = os.path.join(parent_dir, 'src')
# 创建CmakeLists.txt文件
main_module_path = os.path.join(src_path, 'main')
print("Generate [main] CMakeLists...")
write_main_cmake_content(main_module_path, modules_list)
for module in modules_list:
  target_path = os.path.join(src_path, module)
  print("Generate [" + module + "] CMakeLists...")
  write_module_cmake_content(target_path, module)
  