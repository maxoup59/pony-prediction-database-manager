#include "database-manager.hpp"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

DatabaseManager::DatabaseManager() :
  QObject(),
  databaseName("pony_prediction_test"),
  manager(),
  currentDownload(),
  output()
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
    query.prepare("CREATE DATABASE IF NOT EXISTS " + databaseName);
  }
  if(ok && !query.exec())
  {
    ok = false;
    error = "Couldn't create database: " + databaseName;
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

void DatabaseManager::add(const QDate & date)
{
  // Init
  bool ok = true;
  QString error = "";
  QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL");
  QSqlQuery query;
  QString statement;
  QString tableName = "day" + date.toString("yyyyMMdd");
  database.setHostName("localhost");
  database.setUserName("root");
  database.setPassword("");
  database.setDatabaseName(databaseName);
  // Connect to database
  if(ok && !database.open())
  {
    ok = false;
    error = "Couldn't connect to database" + database.lastError().text();
  }
  // Create table
  if(ok)
  {
    statement = " CREATE TABLE ";
    statement += tableName + " ("
                             "date text"
                             ");";
  }
  if(ok)
  {
    query.prepare(statement);
  }
  if(ok && !query.exec())
  {
    ok = false;
    error = "Couldn't create table: " + tableName + database.lastError().text() + query.lastError().text();
  }
  // Fill table

  // The end
  if(!ok)
  {
    qDebug() << error ;
  }
}

void DatabaseManager::add(const QDate & dateFirst, const QDate & dateLast)
{
  add(dateFirst);
}

void DatabaseManager::download(const QDate & date)
{
  //qDebug() << "Download asked";
  QString url =
      "http://www.aspiturf.com/PTcoursejourdetinsertdatecsv.php?datejour="
      + date.toString("yyyy-MM-dd");
  QString dir = "C:/Users/Loic/coding/pony-prediction-data";
  QString filename = dir + "/day" + date.toString("yyyyMMdd") + ".csv";
  output.setFileName(filename);
  if (!output.open(QIODevice::WriteOnly))
  {
    qDebug() << "problem";
    return;
  }
  QNetworkRequest request(url);
  currentDownload = manager.get(request);
  /*connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)),
          SLOT(downloadProgress(qint64,qint64)));*/
  connect(currentDownload, SIGNAL(finished()),
          SLOT(downloadFinished()));
  connect(currentDownload, SIGNAL(readyRead()),
          SLOT(downloadReadyRead()));
  qDebug() << "Download started for: " + date.toString("yyyy-MM-dd");
}

void DatabaseManager::download(const QDate & dateFirst, const QDate & dateLast)
{
  this->dateFirst = dateFirst;
  this->dateLast = dateLast;
  this->dateDownloading = dateFirst;
  download(dateDownloading);
}

void DatabaseManager::downloadNext()
{
  dateDownloading = dateDownloading.addDays(1);
  if(dateDownloading <= dateLast)
    download(dateDownloading);
  else
    qDebug() << "Done downloading with succes";
}

void DatabaseManager::downloadFinished()
{
  qDebug() << "Download finished";
  output.close();
  downloadNext();
}

void DatabaseManager::downloadReadyRead()
{
  output.write(currentDownload->readAll());
}
