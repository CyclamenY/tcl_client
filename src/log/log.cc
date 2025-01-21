#include "log/log.h"

#include <algorithm>
#include <cstdarg>
#include <cstdio>
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

Log* Log::log_ = nullptr;

bool Log::procInit() {
  std::string running_path = locLogDir();
  if (running_path.empty()) {
    std::cout << "Can not loc exe loc!" << std::endl;
  }
#ifdef WIN32
  return createWindowsLogFile(running_path);
#else
  return createLinuxLogFile(running_path);
#endif
}

bool Log::guiInit() {
  std::string running_path = locLogDir();
  if (running_path.empty()) {
    std::cout << "Can not loc exe loc!" << std::endl;
  }
#ifdef WIN32
  return createWindowsLogFile(running_path);
#else
  return createLinuxLogFile(running_path);
#endif
}

Log::Log() {
  
}

Log::~Log() {
  if (logFile_.is_open())
    logFile_.close();
}

std::string Log::locLogDir() {
  std::string running_path = "";
  char buffer[MAX_PATH];
#ifdef WIN32
  GetModuleFileName(NULL, buffer, MAX_PATH);
  auto pos = std::string(buffer).find_last_of("\\/");
  running_path = std::string(buffer).substr(0, pos);
  std::replace(running_path.begin(), running_path.end(), '\\/', '/\\');
  running_path += "\\logfiles";
  if (_access(running_path.c_str(), 0) == -1) {
    if (_mkdir(running_path.c_str()) != 0) {
      std::cout << "Can not create logfiles dir!" << std::endl;
    }
  }
#else
  ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);
  auto pos = std::string(buffer).find_last_of("/\\");
  running_path = std::string(buffer).substr(0, pos) + "\\logfiles";
  if (access(running_path, F_OK) == -1) {
    if (mkdir(running_path, 0777) != 0) {
      std::cout << "Can not create logfiles dir!" << std::endl;
    }
  }
#endif
  return running_path;
}

bool Log::createWindowsLogFile(std::string& path) {
  int max_num = 0;

  WIN32_FIND_DATA findFileData;
  HANDLE hFind = INVALID_HANDLE_VALUE;

  std::string search_path = path + "\\*";
  hFind = FindFirstFile(search_path.c_str(), &findFileData);

  if (hFind == INVALID_HANDLE_VALUE) {
    std::cerr << "FindFirstFile failed (" << GetLastError() << ")" << std::endl;
    return false;
  } else {
    do {
      std::string fileName = findFileData.cFileName;
      if (fileName == "." || fileName == "..")
        continue;
      std::string prefix = fileName.substr(0, 10);
      auto pos = fileName.find_last_of('.');
      std::string num_str = fileName.substr(10, pos - 10);
      if (prefix == "tclClient_") {
        try {
          max_num = max(max_num, std::stoi(num_str));
        } catch (...) {
          continue;
        }
      }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

    for (; max_num > 0; --max_num) {
      std::string old_path = path + "\\tclClient_" + std::to_string(max_num) + ".log";
      if (_access(old_path.c_str(), 0) == -1)
        continue;
      std::string new_path = path + "\\tclClient_" + std::to_string(max_num + 1) + ".log";
      if (!MoveFile(old_path.c_str(), new_path.c_str())) {
        std::cerr << "Failed to rename file: " << old_path << std::endl;
      }
    }
    std::string old_path = path + "\\tclClient.log";
    if (_access(old_path.c_str(), 0) == 0) {
      std::string new_path = path + "\\tclClient_1.log";
      if (!MoveFile(old_path.c_str(), new_path.c_str())) {
        std::cerr << "Failed to rename file: " << old_path << std::endl;
      }
    }
    logFile_.open(old_path.c_str());
    if (!logFile_.is_open()) {
      std::cerr << "Can not open file: " << old_path << std::endl;
      return false;
    }
  }
  return true;
}

#ifndef WIN32
bool Log::createLinuxLogFile(std::string& path) {
  int max_num = 0;

  DIR* dir;
  struct dirent* ent;

  if ((dir = opendir(path.c_str())) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      std::string fileName = ent->d_name;
      if (fileName == "." || fileName == "..")
        continue;
      std::string prefix = fileName.substr(0, 10);
      auto pos = fileName.find_last_of('.');
      std::string num_str = fileName.substr(10, pos - 10);
      if (prefix == "tclClient_") {
        try {
          max_num = max(max_num, std::stoi(num_str));
        } catch (...) {
          continue;
        }
      }
    }
    closedir(dir);

    for (; max_num > 0; --max_num) {
      std::string old_path = path + "\\tclClient_" + std::to_string(max_num) + ".log";
      if (access(old_path.c_str(), F_OK) == -1)
        continue;
      std::string new_path = path + "\\tclClient_" + std::to_string(max_num + 1) + ".log";
      if (!MoveFile(old_path.c_str(), new_path.c_str())) {
        std::cerr << "Failed to rename file: " << old_path << std::endl;
      }
    }
    std::string old_path = path + "\\tclClient.log";
    if (access(old_path.c_str(), F_OK) == -1) {
      std::string new_path = path + "\\tclClient_1.log";
      if (!MoveFile(old_path.c_str(), new_path.c_str())) {
        std::cerr << "Failed to rename file: " << old_path << std::endl;
      }
    }
    if (!logFile_.is_open()) {
      std::cerr << "Can not open file: " << old_path << std::endl;
      return false;
    }
  } else {
    perror("opendir");
    return false;
  }
  return true;
}
#endif

bool Log::printDebug(const char* format, ...) {
  char buffer[2048];
  va_list args;
  va_start(args, format);
  int result = vsnprintf(buffer, 2048, format, args);
  va_end(args);

  return log_->print(0, buffer);
}

bool Log::printInfo(const char* format, ...) {
  char buffer[2048];
  va_list args;
  va_start(args, format);
  int result = vsnprintf(buffer, 2048, format, args);
  va_end(args);

  return log_->print(1, buffer);
}

bool Log::printWarn(const char* format, ...) {
  char buffer[2048];
  va_list args;
  va_start(args, format);
  int result = vsnprintf(buffer, 2048, format, args);
  va_end(args);

  return log_->print(2, buffer);
}

bool Log::printError(const char* format, ...) {
  char buffer[2048];
  va_list args;
  va_start(args, format);
  int result = vsnprintf(buffer, 2048, format, args);
  va_end(args);

  return log_->print(3, buffer);
}

bool Log::print(int log_level, const char* msg) {
  std::string header;
  switch (log_level) {
    case LOG_DEBUG:
      header = LOG_DEBUG_STR; break;
    //case LOG_INFO:
    //  header = LOG_INFO_STR; break;
    case LOG_WARN:
      header = LOG_WARN_STR; break;
    case LOG_ERROR:
      header = LOG_ERROR_STR; break;
  }
  std::cout << header << msg;
#ifdef WIN32
  fflush(stdout);
#endif

  if (!logFile_.is_open()) {
    std::cerr << "logfile is bad!" << std::endl;
    return false;
  }
  logFile_ << header << msg;
#ifdef WIN32
  logFile_.flush();
#endif
  return true;
}