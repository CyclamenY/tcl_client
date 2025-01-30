#include "serial/serial_port.h"
#include "log/log.h"

#ifdef WIN32
#include <windows.h>
#endif

bool SerialPort::iterateSerialPort(std::vector<std::string>& portVec) {
#ifdef WIN32
  HKEY hKey;
  if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
      "HARDWARE\\DEVICEMAP\\SERIALCOMM", 0, KEY_READ, &hKey)) {
    char portName[256];
    char portValue[256];
    DWORD portNameSize;
    DWORD portValueSize;
    DWORD type;
    DWORD index = 0;
    while (true) {
      portNameSize = sizeof(portName);
      portValueSize = sizeof(portValue);
      if (ERROR_SUCCESS != RegEnumValue(hKey, index, portValue, &portValueSize,
          nullptr, &type, reinterpret_cast<LPBYTE>(portName), &portNameSize))
        break;
      if (type == REG_SZ)
        portVec.emplace_back(portName);
      index++;
    }
    RegCloseKey(hKey);
  } else {
    Log::printError("Can not open windows RegMap!\n");
    return false;
  }
#endif
  return true;
}