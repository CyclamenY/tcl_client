#include "tcl/tcl_command.h"
#include "log/log.h"

#include <algorithm>
#include <sstream>
#include <iostream>

Commands* Commands::instance_ = nullptr;

#define DEFINE_TCL_ENTRY( Holder ) extern int Holder(ClientData data,\
  Tcl_Interp* interp, int objc, Tcl_Obj* const objv[]);

// define func area
DEFINE_TCL_ENTRY(AddFunc);

Commands* Commands::instance() {
  if (!instance_)
    instance_ = new Commands();
  return instance_;
}

bool Commands::isOptionUsed(const int objc, Tcl_Obj* const objv[], const char* optionName) {
  if (getOptionIndex(objc, objv, optionName) >= 0)
    return true;
  return false;
}

int Commands::getOptionIndex(const int objc, Tcl_Obj* const objv[], const char* optionName) {
  for (int i = 1; i < objc; ++i) {
    if (strcmp(optionName, Tcl_GetString(objv[i])) == 0) {
      return i;
    }
  }
  return -1;
}

int Commands::getOptionString(const int objc, Tcl_Obj* const objv[], const char* optionName, std::string& option) {
  for (int i = 1; i < objc; ++i) {
    if (strcmp(optionName, Tcl_GetString(objv[i])) == 0) {
      option = Tcl_GetString(objv[i + 1]);
      return TCL_OK;
    }
  }
  return TCL_ERROR;
}

int Commands::registerCmd(Tcl_Interp* interp, const std::string cmdName, const std::string cmdOptions, CommandFunc cmdFunc) {
  commands_.emplace_back(cmdName);
  options_.emplace_back(cmdOptions);
  commandFuncs_.emplace_back(cmdFunc);
  args_.push_back({});
  int cmdId = commands_.size() - 1;

  // parse param
  std::istringstream iss(cmdOptions);
  std::string last_option = "";
  for (std::string token; iss >> token;) {
    if (token[0] == '-') {
      last_option = token;
      args_[cmdId][token] = OptionType::kNone;
      continue;
    }
    if (last_option.empty())
      return TCL_ERROR;
    if(token == "<string>")
      args_[cmdId][last_option] = OptionType::kString;
    else if(token == "<bool>")
      args_[cmdId][last_option] = OptionType::kBool;
    else if(token == "<int>")
      args_[cmdId][last_option] = OptionType::kInt;
    else {
      args_[cmdId][last_option] = OptionType::kNull;
      return TCL_ERROR;
    }
  }
  Tcl_CreateObjCommand(interp, cmdName.c_str(), cmdFunc, (ClientData)nullptr, (Tcl_CmdDeleteProc*)nullptr);
  return TCL_OK;
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
#ifdef _DEBUG
  commands->registerCmd(interp, "add_func", "-test -test1 <string>", AddFunc);
#endif

  return TCL_OK;
}

int Commands::preRun(const int objc, Tcl_Obj* const objv[]) {
  if (!instance_ || objc <= 0)
    return TCL_ERROR;
  std::string cmd = Tcl_GetString(objv[0]);
  auto iter = std::find(instance_->commands_.begin(), instance_->commands_.end(), cmd);
  if (iter == instance_->commands_.end())
    return TCL_ERROR;
  int pos = iter - instance_->commands_.begin();
  std::map<std::string, OptionType>& args = instance_->args_[pos];

  // parse param
  for (int i = 1; i < objc; ++i) {
    std::string option = Tcl_GetString(objv[i]);
    if (option.length() && option[0] == '-') {
      auto iter = args.find(option);
      if (iter == args.end()) {
        Log::printError("Error option %s\n", option.c_str());
        return TCL_ERROR;
      }
      switch (iter->second) {
        case Commands::OptionType::kString: {
          if (i + 1 > objc) return TCL_ERROR;
          std::string next_option = Tcl_GetString(objv[i + 1]);
          if (next_option.empty()) return TCL_ERROR;
          if (next_option[0] == '-') return TCL_ERROR;
          break;
        }
        case Commands::OptionType::kBool: {
          if (i + 1 > objc) return TCL_ERROR;
          std::string next_option = Tcl_GetString(objv[i + 1]);
          if (next_option.empty()) return TCL_ERROR;
          if (next_option[0] == '-') return TCL_ERROR;
          if (next_option != "true" && next_option != "false") return TCL_ERROR;
          break;
        }
        case Commands::OptionType::kInt: {
          if (i + 1 > objc) return TCL_ERROR;
          std::string next_option = Tcl_GetString(objv[i + 1]);
          if (next_option.empty()) return TCL_ERROR;
          if (next_option[0] == '-') return TCL_ERROR;
          try {
            std::stoi(next_option);
          } catch (...) {
            return TCL_ERROR;
          }
          break;
        }
        case Commands::OptionType::kNone:
        default: break;
      }
    }
  }
  instance_->start_ = std::chrono::high_resolution_clock::now();
  
  return TCL_OK;
}

int Commands::postRun() {
  if (!instance_)
    return TCL_ERROR;

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - instance_->start_);
  Log::printInfo("Elapsed time: %.2fs\n", duration.count() / 1000.0);
  return TCL_OK;
}
