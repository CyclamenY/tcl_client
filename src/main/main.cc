#include "event/event.h"
#include "tcl.h"

#include <iostream>

int main() {
  int i = 0;
  Tcl_Init(NULL);
  Event event(1);
  std::cout << "test " << event.getNum() << std::endl;
  return 0;
}