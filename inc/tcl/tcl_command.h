#ifndef TCL_COMMAND_H_
#define TCL_COMMAND_H_

#include <tcl.h>

#include <vector>

typedef int(*CommandFunc)(ClientData, Tcl_Interp*, int, const Tcl_Obj*);

int TclInitProc(Tcl_Interp* interp);

class Commands {
public:
  static Commands* instance();
  void setInterp(Tcl_Interp* interp) { interp_ = interp; }
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