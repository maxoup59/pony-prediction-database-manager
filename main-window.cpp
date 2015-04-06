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
  databaseManager.downloadAndAddTables(QDate(2014,01,01), QDate(2014,01,31));
}

MainWindow::~MainWindow()
{
  delete ui;
}
