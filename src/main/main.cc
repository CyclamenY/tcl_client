#include "event/event.h"
#include "gui/gui.h"
#include "tcl/tcl_command.h"
#include "log/log.h"

#include <tcl.h>
#include <iostream>
#include <vector>
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
  SetConsoleTitle("tclClient Console");
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
    Tcl_Main(argc, argv, TclInitProc);
  }
  return TCL_OK;
}