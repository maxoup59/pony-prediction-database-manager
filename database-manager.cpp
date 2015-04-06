#include "database-manager.hpp"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QStringList>

const QString DatabaseManager::DIR = "C:/Users/Loic/coding/pony-prediction-data";
const QString DatabaseManager::DATABASE_NAME = "ponyPrediction";
const int DatabaseManager::COLUMNS_COUNT = 82;

DatabaseManager::DatabaseManager(const QString & databaseName) :
  QObject(),
  databaseName(databaseName),
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
    query.prepare("CREATE DATABASE " + databaseName + " COLLATE utf8_unicode_ci ");
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
  qDebug() << "Adding " + date.toString("yyyy-MM-dd") + " to database";
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
  // Create table
  if(ok)
  {
    statement = " DROP TABLE IF EXISTS " + tableName + " ; ";
    query.prepare(statement);
  }
  if(ok && !query.exec())
  {
    ok = false;
    error = "Couldn't drop table: " + tableName
        + database.lastError().text() + query.lastError().text();
  }
  if(ok)
  {
    statement = " CREATE TABLE ";
    statement += tableName + " ( ";
    for(int i = 0 ; i < columns.size() - 1 ; i++)
    {
      statement += " " + columns[i] + " text COLLATE utf8_unicode_ci, ";
    }
    statement += " " + columns[columns.size() - 1] + " text );";
    //qDebug() << statement;
    query.prepare(statement);
  }
  if(ok && !query.exec())
  {
    ok = false;
    error = "Couldn't create table: " + tableName
        + database.lastError().text() + query.lastError().text();
  }
  // Fill table
  file.setFileName(DIR + "/" + tableName + ".csv");
  if(!file.open(QIODevice::ReadOnly)) {
    qDebug() << file.errorString();
  }
  in.setDevice(&file);
  in.setCodec("UTF-8");
  while(!in.atEnd())
  {
    QString line = in.readLine();
    if(line[line.size()-1] == ';')
      line.remove(line.size()-1, 1);
    QStringList values = line.split(";");
    if(values.size() == COLUMNS_COUNT)
    {
      statement = " INSERT INTO " + tableName + " VALUES (";
      for(int i = 0 ; i < values.size() - 1 ; i++)
      {
        statement += " '" + values[i] + "' , ";
        //statement += "'bob', ";
      }
      statement += " '" + values[values.size()-1] + "' );";
      //statement += " 'bob'); ";
      query.prepare(statement);
      if(!query.exec())
      {
        qDebug() << "Couldn't insert into " + tableName
                    + database.lastError().text() + query.lastError().text();
        qDebug() << statement;
      }
    }
    else
    {
      qDebug() << "Not the right count of value: "
                  + QString::number(values.size())
                  + " instead of " + QString::number(COLUMNS_COUNT);
    }
  }
  file.close();
  // The end
  if(!ok)
  {
    qDebug() << error ;
  }
  database.close();
  qDebug() << "Done adding";
  addNext();
}

void DatabaseManager::add(const QDate & dateFirst, const QDate & dateLast)
{
  this->dateFirst = dateFirst;
  this->dateLast = dateLast;
  this->currentDate = dateFirst;
  add(currentDate);
}

void DatabaseManager::addNext()
{
  currentDate = currentDate.addDays(1);
  if(currentDate <= dateLast)
    add(currentDate);
  else
  {
    qDebug() << "Done adding with succes";
  }
}

void DatabaseManager::download(const QDate & date)
{
  qDebug() << "Download started for: " + date.toString("yyyy-MM-dd");
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
  connect(currentDownload, SIGNAL(finished()),
          SLOT(downloadFinished()));
  connect(currentDownload, SIGNAL(readyRead()),
          SLOT(downloadReadyRead()));
}

void DatabaseManager::download(const QDate & dateFirst, const QDate & dateLast)
{
  this->dateFirst = dateFirst;
  this->dateLast = dateLast;
  this->currentDate = dateFirst;
  download(currentDate);
}

void DatabaseManager::downloadNext()
{
  currentDate = currentDate.addDays(1);
  if(currentDate <= dateLast)
    download(currentDate);
  else
  {
    qDebug() << "Done downloading with succes";
    createDatabase();
    add(dateFirst, dateLast);
  }
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



void DatabaseManager::downloadAndAdd(const QDate & date)
{

}

void DatabaseManager::downloadAndAdd(const QDate & dateFirst, const QDate & dateLast)
{

}

void DatabaseManager::downloadAndAdd()
{

}
