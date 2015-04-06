#include "database-manager.hpp"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QStringList>

const QString DatabaseManager::DIR = "C:/Users/Loic/coding/pony-prediction-data";

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
  // Init
  bool ok = true;
  QString error = "";
  QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL");
  QSqlQuery query;
  database.setHostName("localhost");
  database.setUserName("root");
  database.setPassword("");
  // Connect to database
  if(ok && !database.open())
  {
    ok = false;
    error = "Couldn't connect to database";
  }
  // Create database
  if(ok)
  {
    query.prepare("CREATE DATABASE " + databaseName);
  }
  if(ok && !query.exec())
  {
    ok = false;
    error = "Couldn't create database: " + databaseName
        + database.lastError().text() + query.lastError().text();
  }
  if(!ok)
  {
    qDebug() << error;
  }
  database.close();
}

void DatabaseManager::add(const QDate & date)
{
  // Init
  bool ok = true;
  QString error = "";
  QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL");
  QSqlQuery query;
  QString statement = "";
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
  // Loads columns
  QStringList columns;
  QFile file(DIR + "/columns.txt");
  if(!file.open(QIODevice::ReadOnly)) {
    qDebug() << file.errorString();
  }
  QTextStream in(&file);
  while(!in.atEnd()) {
    QString line = in.readLine();
    columns.append(line);
  }
  file.close();
  //qDebug() << columns;
  // Create table
  if(ok)
  {
    statement = " DROP TABLE IF EXISTS " + tableName + " ; ";
    query.prepare(statement);
  }
  if(ok && !query.exec())
  {
    ok = false;
    error = "Couldn't drop table: " + tableName + database.lastError().text() + query.lastError().text();
  }
  if(ok)
  {
    statement = " CREATE TABLE ";
    statement += tableName + " ( ";
    for(int i = 0 ; i < columns.size() - 1 ; i++)
    {
      statement += " " + columns[i] + " text, ";
    }
    statement += " " + columns[columns.size() - 1] + " text );";
    //qDebug() << statement;
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
  database.close();
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
  QString filename = DIR + "/day" + date.toString("yyyyMMdd") + ".csv";
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
