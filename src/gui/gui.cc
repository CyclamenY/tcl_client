#include "gui/gui.h"
#include "gui/main_window.h"
#include "tcl/tcl_command.h"

#include <QApplication>

QApplication* Gui::app_ = nullptr;
MainWindow* Gui::main_window_ = nullptr;

void Gui::initGui(int argc, char* argv[]) {
  Tcl_FindExecutable(argv[0]);
  app_ = new QApplication(argc, argv);
  app_->setApplicationName("tclClient");

  Tcl_SetServiceMode(TCL_SERVICE_ALL);
  Tcl_Main(argc, argv, TclInitGui);
}

int Gui::TclInitGui(Tcl_Interp* interp) {
  if (!interp)
    return TCL_ERROR;
  if (Tcl_Init(interp) == TCL_ERROR)
    return TCL_ERROR;

#ifdef WIN32
  Tcl_Channel tclStdout, tclStdin, tclStderr;
  tclStdout = Tcl_OpenFileChannel(interp, "nul", "r", 0);
  tclStdin = Tcl_OpenFileChannel(interp, "nul", "w", 0);
  tclStderr = Tcl_OpenFileChannel(interp, "nul", "w", 0);
  Tcl_RegisterChannel(interp, tclStdout);
  Tcl_RegisterChannel(interp, tclStdin);
  Tcl_RegisterChannel(interp, tclStderr);
  Tcl_SetStdChannel(tclStdout, TCL_STDOUT);
  Tcl_SetStdChannel(tclStdin, TCL_STDIN);
  Tcl_SetStdChannel(tclStderr, TCL_STDERR);
#endif

  Commands* commands = Commands::instance();
  commands->setInterp(interp);
  RegisterAllCmds(interp);

  // init gui
  main_window_ = new MainWindow();
  main_window_->setMinimumSize(800, 600);
  main_window_->show();
  exit(app_->exec());
  return TCL_OK;
}
