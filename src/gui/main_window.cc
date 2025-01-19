#include "gui/main_window.h"
#include "log/log.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent), ui(new Ui::MainWindow){
  ui->setupUi(this);
  connect(ui->menubar, &QMenuBar::close, this, []() {
    Log::printInfo("test\n"); });
}

MainWindow::~MainWindow() {
  
}