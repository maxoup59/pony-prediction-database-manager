#include "main-window.hpp"
#include "ui_main-window.h"
#include <QDate>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  databaseManager("ponyprediction")
{
  ui->setupUi(this);
  if(!databaseManager.connect())
  {
    if(databaseManager.createDatabase())
      databaseManager.connect();
  }
  databaseManager.add(QDate(2014,01,01), QDate(2014,12,31), true);
}

MainWindow::~MainWindow()
{
  delete ui;
}
