#ifndef LOG_H_H_
#define LOG_H_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LOG_INFO 0
#define LOG_WARN 1
#define LOG_ERROR 2

#define LOG_INFO_STR "INFO"
#define LOG_WARN_STR "WARN"
#define LOG_ERROR_STR "ERROR"

typedef bool(*GuiLogFunc)();

class Log {
public:
  Log* instanse() {
    if (log_ == NULL) {
      log_ = new Log();
    }
    return log_;
  }
  static bool printInfo();
  static bool printWarn();
  static bool printError();
  static bool printDebug();

protected:
  Log() {}
  ~Log() {}

private:
  static Log* log_;
  FILE* logFile_;
  GuiLogFunc guiLogFunc_;
};

#endif