#include "event/event.h"
#include "gui/main_window.h"
#include "tcl/tcl_command.h"
#include <tcl.h>

#include <iostream>
#include <QApplication>
#include <QCoreApplication>
#include <vector>
#ifdef WIN32
#include "windows.h"
#endif

//int main(int argc, char *argv[]) {
//  int i = 0;
//  Tcl_Interp* interp = Tcl_CreateInterp();
//  Tcl_Channel stdOut = Tcl_GetStdChannel(TCL_STDOUT);
//  if(stdOut != NULL)
//    Tcl_UnregisterChannel(interp, stdOut);
//  Tcl_Channel chan = Tcl_OpenFileChannel(interp, "file1.txt", "w+", 777);
//  Tcl_RegisterChannel(interp, chan);
//  Tcl_SetStdChannel(chan, TCL_STDOUT);
//  if (Tcl_Init(interp) != TCL_OK) {
//    std::cout << "error" << std::endl;
//    return TCL_ERROR;
//  }
//  Tcl_FindExecutable(nullptr);
//  std::string cmd = "set t 3; puts $t";
//  if (Tcl_Eval(interp, cmd.c_str()) != TCL_OK) {
//    std::cout << "error" << std::endl;
//    return TCL_ERROR;
//  }
//  Tcl_DeleteInterp(interp);
//  // Tcl_Init(NULL);
//  QApplication a(argc, argv);
//  Event event(1);
//  std::cout << "test " << event.getNum() << std::endl;
//  MainWindow main_window;
//  main_window.show();
//  return a.exec();
//}

void attachConsole() {
  AllocConsole();
  FILE* fDummy;
  freopen_s(&fDummy, "CONOUT$", "w", stdout);
  freopen_s(&fDummy, "CONOUT$", "w", stderr);
  freopen_s(&fDummy, "CONIN$", "r", stdin);
  SetConsoleTitle("App Console");
}

int main(int argc, char* argv[]) {
  bool isGui = false;
  for (int i = 1; i < argc; ++i) {
    if (strcmp("-gui", argv[i]) == 0) {
      isGui = true;
    }
  }

  int ret_value = TCL_OK;

  if (isGui) {
    QApplication app(argc, argv);
    MainWindow main_window;
    main_window.show();
    ret_value = app.exec();
  } else {
    attachConsole();
    Tcl_Main(argc, argv, TclInitProc);
  }
  return TCL_OK;
}