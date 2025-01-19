#ifndef TCL_COMMAND_H_
#define TCL_COMMAND_H_

#include <tcl.h>
#include <string>

#include <vector>
#include <map>
#include <chrono>

typedef int(*CommandFunc)(ClientData data, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[]);

int TclInitProc(Tcl_Interp* interp);
int RegisterAllCmds(Tcl_Interp* interp);

class Commands {
public:
  enum OptionType {
    kNone,
    kString,
    kBool,
    kInt,
    kNull
  };
  static Commands* instance();
  static bool isOptionUsed(const int objc, Tcl_Obj* const objv[], const char* optionName);
  static int getOptionIndex(const int objc, Tcl_Obj* const objv[], const char* optionName);
  static int getOptionString(const int objc, Tcl_Obj* const objv[], const char* optionName, std::string& option);
  static int preRun(const int objc, Tcl_Obj* const objv[]);
  static int postRun();
  void setInterp(Tcl_Interp* interp) { interp_ = interp; }
  int registerCmd(Tcl_Interp*, const std::string, const std::string, CommandFunc);
protected:
  Commands() : interp_(nullptr) {}
  ~Commands() {}
private:
  static Commands* instance_;
  Tcl_Interp* interp_;
  std::vector<std::string> commands_;                   // command header
  std::vector<std::string> options_;                    // command options
  std::vector<std::map<std::string, OptionType>> args_; // command args
  std::vector<CommandFunc> commandFuncs_;               // command handler functions
  std::chrono::steady_clock::time_point start_;
};

#endif