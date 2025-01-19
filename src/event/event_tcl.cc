#include "tcl/tcl_command.h"
#include "log/log.h"

int AddFunc(ClientData /*data*/, Tcl_Interp* /*interp*/, int objc, Tcl_Obj* const objv[]) {
  if (Commands::preRun(objc, objv) != TCL_OK)
    return TCL_ERROR;

  std::string test1;
  if (Commands::isOptionUsed(objc, objv, "-test")) {
    Log::printInfo("test\n");
  }
  if (Commands::isOptionUsed(objc, objv, "-test1")) {
    test1.clear();
    Commands::getOptionString(objc, objv, "-test1", test1);
    Log::printInfo("%s\n", test1.c_str());
  }

  if (Commands::postRun())
    return TCL_ERROR;
  return TCL_OK;
}