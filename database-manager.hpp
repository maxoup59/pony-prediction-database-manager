#pragma once

#include <QDate>

class DatabaseManager
{
  public:
    DatabaseManager();
    ~DatabaseManager();
    void createDatabase();
    void downloadAndAddTable(const QDate & date);
    void downloadAndAddTables(const QDate & dateFirst, const QDate & dateLast);
  private:
    QString databaseName;
};
