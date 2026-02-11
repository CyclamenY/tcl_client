#include "tcl/tcl_command.h"
#include "utility/utility.h"
#include "log/log.h"

#include <algorithm>
#include <sstream>
#include <iostream>

Commands* Commands::instance_ = nullptr;

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
  std::vector<std::string> split_str;
  Utility::String::split(cmdOptions, split_str);
  int index = 0;
  while(index < split_str.size()) {
    const std::string& token = split_str[index];
    if (token.at(0) == '-') {
      if (index < split_str.size() && split_str[index + 1].at(0) == '<') {
        // see next string is not this one param
        if (token == "<string>")
          args_[cmdId][token] = OptionType::kString;
        else if (token == "<bool>")
          args_[cmdId][token] = OptionType::kBool;
        else if (token == "<int>")
          args_[cmdId][token] = OptionType::kInt;
        index += 2;
      } else {
        // next is not this one param
        args_[cmdId][token] = OptionType::kNone;
        index += 1;
      }
    } else if (index == 0 && token.at(0) == '<') {
      args_[cmdId]["-dummy_first"] = OptionType::kNone;
      index += 1;
    } else {
      Log::printError("TCL %s Cmd format error.\n", split_str.at(0).c_str());
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
    if (option.empty())
      continue;
    Commands::OptionType optionType;
    bool dummy_flag = false;
    if (option[0] == '-') {   // option
      auto iter = args.find(option);
      if (iter == args.end()) {
        Log::printError("Error option %s\n", option.c_str());
        return TCL_ERROR;
      }
      optionType = iter->second;
    } else if (i == 1) {
      auto iter = args.find("-dummy_first");
      if (iter == args.end()) {
        Log::printError("Error option dummy\n");
        return TCL_ERROR;
      }
      optionType = iter->second;
      dummy_flag = true;
    } else {
      Log::printError("Unexpected param.\n");
      return TCL_ERROR;
    }
    int option_index = i + !dummy_flag;
    switch (optionType) {
      case Commands::OptionType::kString: {
        if (option_index > objc) return TCL_ERROR;
        std::string next_option;
        next_option = Tcl_GetString(objv[option_index]);
        if (next_option.empty()) return TCL_ERROR;
        if (next_option[0] == '-') return TCL_ERROR;
        break;
      }
      case Commands::OptionType::kBool: {
        if (option_index > objc) return TCL_ERROR;
        std::string next_option = Tcl_GetString(objv[option_index]);
        if (next_option.empty()) return TCL_ERROR;
        if (next_option[0] == '-') return TCL_ERROR;
        if (next_option != "true" && next_option != "false") return TCL_ERROR;
        break;
      }
      case Commands::OptionType::kInt: {
        if (option_index > objc) return TCL_ERROR;
        std::string next_option = Tcl_GetString(objv[option_index]);
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
