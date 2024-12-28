#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <QMainWindow>

class MainWindow : public QMainWindow {
    //Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif