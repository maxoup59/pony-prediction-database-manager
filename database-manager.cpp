#include "database-manager.hpp"
#include <QEventLoop>;

const int DatabaseManager::COLUMNS_COUNT = 82;

DatabaseManager::DatabaseManager(const QString & databaseName) :
  QObject(),
  databaseName(databaseName),
  database(),
  dir("C:/Users/Loic/coding/pony-prediction-data"),
  connected(false)
{
}

DatabaseManager::~DatabaseManager()
{

}

bool DatabaseManager::connect()
{
  bool ok = true;
  QString error = "";
  database = QSqlDatabase::addDatabase("QMYSQL");
  database.setHostName("localhost");
  database.setUserName("root");
  database.setPassword("");
  database.setDatabaseName(databaseName);
  if(ok && !database.open())
  {
    ok = false;
    error = "Couldn't connect to database (" + databaseName + ") "
        + database.lastError().text();
  }
  if(!ok)
  {
    qDebug() << "ERROR: " + error;
    database.close();
  }
  else if(ok)
  {
    qDebug() << "Connected to database (" + databaseName + ")";
  }
  connected = ok;
  return ok;
}

bool DatabaseManager::createDatabase()
{
  bool ok = true;
  QString error = "";
  database = QSqlDatabase::addDatabase("QMYSQL");
  database.setHostName("localhost");
  database.setUserName("root");
  database.setPassword("");
  QSqlQuery query;
  if(ok && !database.open())
  {
    ok = false;
    error = "Couldn't connect to database (" + databaseName + ") "
        + database.lastError().text();
  }
  if(ok)
    query.prepare("CREATE DATABASE " + databaseName
                  + " COLLATE utf8_unicode_ci ");
  if(ok && !query.exec())
  {
    ok = false;
    error = "Couldn't create database (" + databaseName + ") "
        + database.lastError().text() + query.lastError().text();
  }
  if(!ok)
  {
    qDebug() << "ERROR: " + error;
  }
  else if(ok)
  {
    qDebug() << "Created database (" + databaseName + ")";
  }
  database.close();
  return ok;
}

void DatabaseManager::add(const QDate & date, const bool & doDrop)
{
  qDebug() << "Adding " + date.toString("yyyy-MM-dd") + " to database";
  QString tableName = "day" + date.toString("yyyyMMdd");
  QString filePath = dir + "/" + tableName + ".csv";
  bool ok = true;
  QString error = "";
  //
  if(ok && !connected)
  {
    ok = false;
    error = "Not connected to any database";
  }
  // Drop table
  if(ok && doDrop)
  {
    QSqlQuery query;
    QString statement = " DROP TABLE IF EXISTS " + tableName + ";";
    query.prepare(statement);
    if(!query.exec())
    {
      ok = false;
      error = "Couldn't drop table (" + tableName + ") "
          + database.lastError().text() + query.lastError().text();
    }
  }
  // Create table
  if(ok)
  {
    QSqlQuery query;
    QString statement = "";
    QStringList columns;
    QFile file(dir + "/columns.txt");
    if(!file.open(QIODevice::ReadOnly)) {
      ok = false;
      error = dir + "/columns.txt " + file.errorString();
    }
    if(ok)
    {
      QTextStream in(&file);
      while(!in.atEnd())
        columns.append(in.readLine());
      file.close();
      statement = " CREATE TABLE ";
      statement += tableName + " ( ";
      for(int i = 0 ; i < columns.size() - 1 ; i++)
        statement += " " + columns[i] + " text, ";
      statement += " " + columns[columns.size() - 1] + " text );";
      query.prepare(statement);
      if(ok && !query.exec())
      {
        ok = false;
        error = "Couldn't create table (" + tableName + ") "
            + database.lastError().text() + query.lastError().text();
      }
    }
  }
  // Fill table
  if(ok)
  {
    QSqlQuery query;
    QString statement = "";
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly)) {
      qDebug() << file.errorString();
    }
    QTextStream in(&file);
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
  }
  // The end
  if(!ok)
    qDebug() << "ERROR: " + error;
  else if(ok)
    qDebug() << "Done adding";
}

void DatabaseManager::add(const QDate & dateFirst, const QDate & dateLast,
                          const bool & doDrop)
{
  for(QDate date = dateFirst ; date <= dateLast ; date = date.addDays(1))
    add(date, doDrop);
}

void DatabaseManager::download(const QDate & date, const bool & doErase)
{
  qDebug() << "Download started for: " + date.toString("yyyy-MM-dd");
  bool ok = true;
  QString error = "";
  QFile output;
  QString url =
      "http://www.aspiturf.com/PTcoursejourdetinsertdatecsv.php?datejour="
      + date.toString("yyyy-MM-dd");
  QString filename = dir + "/day" + date.toString("yyyyMMdd") + ".csv";
  if(ok && !doErase && QFile::exists(filename))
  {
    ok = false;
    error = "The file already exists";
  }
  if(ok)
    output.setFileName(filename);
  if (ok && !output.open(QIODevice::WriteOnly))
  {
    ok = false;
    error = "Couldn't open file";
  }
  if(ok)
  {
    QNetworkRequest request(url);
    QNetworkAccessManager manager;
    QNetworkReply * download = manager.get(request);
    QEventLoop eventLoop;
    QObject::connect(download, SIGNAL(finished()),
                     &eventLoop, SLOT(quit()));
    eventLoop.exec(); // wait until download is done
    output.write(download->readAll());
  }
  output.close();
  if(!ok)
    qDebug() << "ERROR: " + error;
  else if(ok)
    qDebug() << "Download finished";
}

void DatabaseManager::download(const QDate & dateFirst, const QDate & dateLast,
                               const bool & doErase)
{
  for(QDate date = dateFirst ; date <= dateLast ; date = date.addDays(1))
    download(date, doErase);
}

void DatabaseManager::downloadAndAdd(const QDate & date, const bool & doErase)
{
  download(date, doErase);
  add(date, doErase);
}

void DatabaseManager::downloadAndAdd(const QDate & dateFirst,
                                     const QDate & dateLast,
                                     const bool & doErase)
{
  for(QDate date = dateFirst ; date <= dateLast ; date = date.addDays(1))
    downloadAndAdd(date, doErase);
}
