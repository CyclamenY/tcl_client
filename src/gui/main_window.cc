#include "gui/main_window.h"
#include "log/log.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent), ui(new Ui::MainWindow){
  ui->setupUi(this);
}

MainWindow::~MainWindow() {
  
}