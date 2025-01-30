#include "tcl/tcl_command.h"
#include "serial/serial_port.h"

#include <iostream>

int SerialTest(ClientData /*data*/, Tcl_Interp* /*interp*/, int objc, Tcl_Obj* const objv[]) {
  if (Commands::preRun(objc, objv) != TCL_OK)
    return TCL_ERROR;

  SerialPort serialPort;
  std::vector<std::string> portVec;
  serialPort.iterateSerialPort(portVec);
  for (auto port : portVec) {
    std::cout << port << std::endl;
  }

  if (Commands::postRun())
    return TCL_ERROR;
  return TCL_OK;
}