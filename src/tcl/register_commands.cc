#include "tcl/tcl_command.h"

#define DEFINE_TCL_ENTRY( Holder ) extern int Holder(ClientData data,\
  Tcl_Interp* interp, int objc, Tcl_Obj* const objv[]);

// define func area
DEFINE_TCL_ENTRY(AddFunc);

int RegisterAllCmds(Tcl_Interp* interp) {
  Commands* commands = Commands::instance();
  commands->setInterp(interp);
#ifdef _DEBUG
  commands->registerCmd(interp, "add_func", "-test -test1 <string>", AddFunc);
#endif

  return TCL_OK;
}