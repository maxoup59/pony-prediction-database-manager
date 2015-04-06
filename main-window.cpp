#include "main-window.hpp"
#include "ui_main-window.h"
#include <QDate>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  databaseManager()
{
  ui->setupUi(this);
  databaseManager.createDatabase();
  //databaseManager.download(QDate(2014,01,01), QDate(2014,01,01));
  databaseManager.add(QDate(2014,01,01), QDate(2014,01,01));
}

MainWindow::~MainWindow()
{
  delete ui;
}
