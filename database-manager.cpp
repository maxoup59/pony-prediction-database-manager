#include "database-manager.hpp"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

DatabaseManager::DatabaseManager() :
  databaseName("pony_prediction_test")
{

}

DatabaseManager::~DatabaseManager()
{

}

void DatabaseManager::createDatabase()
{
  // Create database
  bool ok = true;
  QString error = "";
  QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL");
  QSqlQuery query;
  database.setHostName("localhost");
  database.setUserName("root");
  database.setPassword("");
  if(ok && !database.open())
  {
    ok = false;
    error = "Couldn't connect to database";
  }
  if(ok)
  {
    query.prepare("CREATE DATABASE IF NOT EXISTS "+databaseName);
  }
  if(ok && !query.exec())
  {
    ok = false;
    error = "Couldn't create database: "+databaseName;
  }
  /*if(ok)
  {
    database.setDatabaseName(databaseName);
  }*/
  if(!ok)
  {
    qDebug() << error;
  }
}

void DatabaseManager::downloadAndAddTable(const QDate & date)
{

}

void DatabaseManager::downloadAndAddTables(const QDate & dateFirst, const QDate & dateLast)
{
  downloadAndAddTable(dateFirst);
}
