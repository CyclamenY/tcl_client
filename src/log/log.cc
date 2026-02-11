#include "log/log.h"
#include "utility/utility.h"

#include <algorithm>
#include <cstdarg>
#include <cstdio>
#ifdef WIN32
#include <windows.h>
//#include <direct.h>
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
  running_path = Utility::File::getCurrentPwdPath();
  running_path += "\\logfiles";
  return running_path;
}

bool Log::createWindowsLogFile(std::string& path) {
  Utility::File::createDir(path);
  int max_num = 0;
  std::vector<std::string> fileVec;

  Utility::File::getDirFiles(path, fileVec);
  for (std::string fileName : fileVec) {
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
  return true;
}

#ifndef WIN32
bool Log::createLinuxLogFile(std::string& path) {
  Utility::File::createDir(path);
  int max_num = 0;
  std::vector<std::string> fileVec;

  Utility::File::getDirFiles(path, fileVec);
  for (std::string fileName : fileVec) {
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