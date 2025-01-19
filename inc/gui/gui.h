#ifndef GUI_H_H_
#define GUI_H_H_

#include "tcl.h"

class MainWindow;
class QApplication;

class Gui {
public:
  static void initGui(int argc, char* argv[]);
  static int TclInitGui(Tcl_Interp* interp);
  static QApplication* app() { return app_; }
  static MainWindow* main_window() { return main_window_; }
protected:
  Gui() = delete;
  ~Gui() = delete;
private:
  static QApplication* app_;
  static MainWindow* main_window_;
};

#endif