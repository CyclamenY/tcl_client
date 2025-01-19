#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <QMainWindow>

#include "ui/ui_main_window.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
private:
  Ui::MainWindow* ui;
};

#endif