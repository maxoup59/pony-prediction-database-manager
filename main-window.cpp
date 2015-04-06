#include "main-window.hpp"
#include "ui_main-window.h"
#include <QDate>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  databaseManager("pony_prediction")
{
  ui->setupUi(this);
  if(!databaseManager.connect())
  {
    if(databaseManager.createDatabase())
      databaseManager.connect();
  }
  databaseManager.downloadAndAdd(QDate(2014,01,01), QDate(2014,01,2), true);
}

MainWindow::~MainWindow()
{
  delete ui;
}
