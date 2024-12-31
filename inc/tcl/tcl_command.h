#ifndef TCL_COMMAND_H_
#define TCL_COMMAND_H_

#include <tcl.h>
#include <string>

#include <vector>

typedef int(*CommandFunc)(ClientData data, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[]);

int TclInitProc(Tcl_Interp* interp);
int RegisterAllCmds(Tcl_Interp* interp);

class Commands {
public:
  static Commands* instance();
  static bool isOptionUsed(const int objc, Tcl_Obj* const objv[], const char* optionName);
  static int getOptionIndex(const int objc, Tcl_Obj* const objv[], const char* optionName);
  void setInterp(Tcl_Interp* interp) { interp_ = interp; }
  void registerCmd(Tcl_Interp*, const std::string, const std::string, CommandFunc);
protected:
  Commands() {}
  ~Commands() {}
private:
  static Commands* instance_;
  Tcl_Interp* interp_;
  std::vector<std::string> commands_;     // command header
  std::vector<std::string> options_;      // command args
  std::vector<CommandFunc> commandFuncs_; // command handler functions
};

#endif