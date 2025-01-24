#include "gui/main_window.h"
#include "log/log.h"

#include <QMessagebox>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent), ui(new Ui::MainWindow), 
    menu_tools_(nullptr), action_about_(nullptr){
  ui->setupUi(this);
  initUi();
}

MainWindow::~MainWindow() {
  
}

bool MainWindow::initUi() {
  menu_tools_ = new QMenu("Tools");
  action_about_ = new QAction("About");

  ui->menuBar_->addMenu(menu_tools_);
  ui->menuBar_->addAction(action_about_);


  // signal connect
  connect(action_about_, &QAction::triggered, this, [this]() {
    QMessageBox aboutMessagebox(this);
    aboutMessagebox.setWindowTitle("About");
    aboutMessagebox.setText("this is a about");
    aboutMessagebox.setIcon(QMessageBox::Information);
    aboutMessagebox.exec();
  });
  return true;
}