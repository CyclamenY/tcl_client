#ifndef LOG_H_H_
#define LOG_H_H_

#include <iostream>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <fstream>

#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_WARN 2
#define LOG_ERROR 3

#define LOG_DEBUG_STR "[DEBUG] "
#define LOG_INFO_STR "[INFO] "
#define LOG_WARN_STR "[WARN] "
#define LOG_ERROR_STR "[ERROR] "

typedef bool(*GuiLogFunc)();

class Log {
public:
  static Log* instanse() {
    if (log_ == NULL) {
      log_ = new Log();
    }
    return log_;
  }
  bool procInit();
  bool guiInit();
  static bool printDebug(const char* format, ...);
  static bool printInfo(const char* format, ...);
  static bool printWarn(const char* format, ...);
  static bool printError(const char* format, ...);

protected:
  Log();
  ~Log();
  std::string locLogDir();
  bool createWindowsLogFile(std::string& path);
#ifndef WIN32
  bool createLinuxLogFile(std::string& path);
#endif
  bool print(int log_level, const char* msg);

private:
  static Log* log_;
  std::ofstream logFile_;
  GuiLogFunc guiLogFunc_;
};

#endif