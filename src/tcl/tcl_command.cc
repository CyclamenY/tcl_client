#include "tcl/tcl_command.h"

Commands* Commands::instance_ = nullptr;

Commands* Commands::instance() {
  if (!instance_)
    instance_ = new Commands();
  return instance_;
}

int TclInitProc(Tcl_Interp* interp) {
  if (!interp)
    return TCL_ERROR;
  if (Tcl_Init(interp) != TCL_OK)
    return TCL_ERROR;
  Commands* command = Commands::instance();
  command->setInterp(interp);
  return TCL_OK;
}