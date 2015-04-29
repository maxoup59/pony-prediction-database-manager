#pragma once

#include <QMainWindow>
#include "database-manager.hpp"
#include "util.hpp"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
  private:
    Ui::MainWindow *ui;
    DatabaseManager databaseManager;
};
