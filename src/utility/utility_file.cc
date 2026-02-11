#include "utility/utility.h"

#include <algorithm>
#include <iostream>

#ifdef WIN32
#include <windows.h>
#include <direct.h>
#include <io.h>
#endif
#ifndef WIN32
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#endif

std::string Utility::File::getCurrentPwdPath() {
  std::string running_path = "";
#ifdef WIN32
  char buffer[MAX_PATH];
  GetModuleFileName(NULL, buffer, MAX_PATH);
  auto pos = std::string(buffer).find_last_of("\\/");
  running_path = std::string(buffer).substr(0, pos);
  std::replace(running_path.begin(), running_path.end(), '\\/', '/\\');
#else
  ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);
  auto pos = std::string(buffer).find_last_of("/\\");
  running_path = std::string(buffer).substr(0, pos);
#endif
  return running_path;
}

void Utility::File::getDirFiles(const std::string& target_path, std::vector<std::string>& file_vec) {
#ifdef WIN32
  WIN32_FIND_DATA findFileData;
  HANDLE hFind = INVALID_HANDLE_VALUE;

  std::string search_path = target_path + "\\*";
  hFind = FindFirstFile(search_path.c_str(), &findFileData);

  if (hFind == INVALID_HANDLE_VALUE) {
    std::cerr << "FindFirstFile failed (" << GetLastError() << ")" << std::endl;
    return;
  } else {
    do {
      std::string fileName = findFileData.cFileName;
      if (fileName == "." || fileName == "..")
        continue;
      file_vec.emplace_back(std::move(fileName));
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
  }
#else
  DIR* dir;
  struct dirent* ent;

  if ((dir = opendir(path.c_str())) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      std::string fileName = ent->d_name;
      if (fileName == "." || fileName == "..")
        continue;
      file_vec.emplace_back(std::move(fileName));
    }
    closedir(dir);
  } else {
    perror("opendir");
    return;
  }
#endif
}

bool Utility::File::createDir(const std::string& path) {
  std::string temp_path = path;
#ifdef WIN32
  std::replace(temp_path.begin(), temp_path.end(), '\\', '/');
#endif
  auto pos = temp_path.find_first_of('/');
  while (pos != std::string::npos) {
    std::string sub_path = temp_path.substr(0, pos);
#ifdef WIN32
    if (_access(sub_path.c_str(), 0) == -1) {
      CreateDirectory(sub_path.c_str(), 0);
    }
#else
    if (access(sub_path.c_str(), F_OK) == -1) {
      mkdir(sub_path, S_IRWXU | S_IRWXG | S_IROTH);
    }
#endif
    pos = temp_path.find_first_of('/', pos + 1);
  }
#ifdef WIN32
  if (_access(temp_path.c_str(), 0) == -1) {
    CreateDirectory(temp_path.c_str(), 0);
  }
#else
  if (access(temp_path.c_str(), F_OK) == -1) {
    mkdir(temp_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH);
  }
#endif
  return true;
}