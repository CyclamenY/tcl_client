#include "tcl/tcl_command.h"

Commands* Commands::instance_ = nullptr;

#define DEFINE_TCL_ENTRY( Holder ) extern int Holder(ClientData data,\
  Tcl_Interp* interp, int objc, Tcl_Obj* const objv[]);

DEFINE_TCL_ENTRY(AddFunc);

Commands* Commands::instance() {
  if (!instance_)
    instance_ = new Commands();
  return instance_;
}

bool Commands::isOptionUsed(const int objc, Tcl_Obj* const objv[], const char* optionName) {
  return getOptionIndex(objc, objv, optionName);
}

int Commands::getOptionIndex(const int objc, Tcl_Obj* const objv[], const char* optionName) {
  for (int i = 1; i < objc; ++i) {
    if (strcmp(optionName, Tcl_GetString(objv[i])) == 0) {
      return i;
    }
  }
  return -1;
}

void Commands::registerCmd(Tcl_Interp* interp, const std::string cmdName, const std::string cmdOptions, CommandFunc cmdFunc) {
  commands_.emplace_back(cmdName);
  options_.emplace_back(cmdOptions);
  commandFuncs_.emplace_back(cmdFunc);

  Tcl_CreateObjCommand(interp, cmdName.c_str(), cmdFunc, (ClientData)nullptr, (Tcl_CmdDeleteProc*)nullptr);
}

int TclInitProc(Tcl_Interp* interp) {
  if (!interp)
    return TCL_ERROR;
  if (Tcl_Init(interp) != TCL_OK)
    return TCL_ERROR;
  Commands* commands = Commands::instance();
  commands->setInterp(interp);

  RegisterAllCmds(interp);

  return TCL_OK;
}

int RegisterAllCmds(Tcl_Interp* interp) {
  Commands* commands = Commands::instance();
  commands->setInterp(interp);

  commands->registerCmd(interp, "add_func", "", AddFunc);

  return TCL_OK;
}