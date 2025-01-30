#ifndef SERIAL_H_
#define SERIAL_H_

#include <serial/serial.h>

class SerialPort {
public:
  SerialPort() : serial_(nullptr) {}
  ~SerialPort() {}
  bool iterateSerialPort(std::vector<std::string>& portVec);
private:
  serial::Serial* serial_;
};

#endif