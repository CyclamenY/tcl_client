#include "event/event.h"
#include "gui/main_window.h"
#include "tcl.h"

#include <iostream>
#include <QApplication>

int main(int argc, char *argv[]) {
  int i = 0;
  // Tcl_Init(NULL);
  QApplication a(argc, argv);
  Event event(1);
  std::cout << "test " << event.getNum() << std::endl;
  MainWindow main_window;
  main_window.show();
  return a.exec();
}