#include "event/event.h"
#include "gui/gui.h"
#include "tcl/tcl_command.h"
#include "log/log.h"
#include "param.def"

#include <tcl.h>
#include <iostream>
#include <vector>
#include <time.h>
#ifdef WIN32
#include <windows.h>
#endif

long __stdcall CrashCallback(_EXCEPTION_POINTERS* excp) {
  char buf[256] = { 0 };
  sprintf(buf, "Error address: %p", excp->ExceptionRecord->ExceptionAddress);
  return EXCEPTION_EXECUTE_HANDLER;
}

void ExitHandler(void*) {
  printf("Thank you for using tclClient.\n");
}

void attachConsole() {
  AllocConsole();
  FILE* fDummy;
  freopen_s(&fDummy, "CONOUT$", "w", stdout);
  freopen_s(&fDummy, "CONOUT$", "w", stderr);
  freopen_s(&fDummy, "CONIN$", "r", stdin);
#ifdef _DEBUG
  SetConsoleTitle("tclClient Console Debug");
#else
  SetConsoleTitle("tclClient Console Release");
#endif
}

void printWelcomeInfo() {
#ifdef _DEBUG
  Log::printInfo("tclClient Debug Ver.\n");
#else
  Log::printInfo("tclClient Release Ver.\n");
#endif
  Log::printInfo("Build Git Hash: %s\n", GIT_TAG);
  Log::printInfo("Build Start Time: %s\n", COMPILE_TIME);
  time_t t = time(NULL);
  struct tm* stime = localtime(&t);
  char time_str[32]{ 0 };
  snprintf(time_str, sizeof(time_str),
    "%04d-%02d-%02d %02d:%02d:%02d", 
    1900 + stime->tm_year, 1 + stime->tm_mon, stime->tm_mday, stime->tm_hour, stime->tm_min, stime->tm_sec);
  Log::printInfo("Launch Start Time: %s\n", time_str);
}

int main(int argc, char* argv[]) {
  bool isGui = false;
  for (int i = 1; i < argc; ++i) {
    if (strcmp("-gui", argv[i]) == 0) {
      isGui = true;
    }
  }

#ifdef WIN32
  SetUnhandledExceptionFilter(CrashCallback);
#endif
  Tcl_CreateExitHandler(ExitHandler, NULL);

  if (isGui) {
    Log::instanse()->guiInit();
    Gui::initGui(argc, argv);
  } else {
    attachConsole();
    Log::instanse()->procInit();
    // print compile info
    printWelcomeInfo();
    Tcl_Main(argc, argv, TclInitProc);
  }
  return TCL_OK;
}