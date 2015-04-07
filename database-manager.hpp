#pragma once

#include <QDate>
#include <QNetworkRequest>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QStringList>
#include <QMutex>
#include <QThread>

class DatabaseManager : public QObject
{
    Q_OBJECT
  public:
    DatabaseManager(const QString & databaseName);
    ~DatabaseManager();
    bool connect();
    bool createDatabase();
    void add(const QDate & date, const bool & doDrop = false);
    void add(const QDate & dateFirst, const QDate & dateLast,
             const bool & doDrop = false);
    void download(const QDate & date, const bool & doErase = false);
    void download(const QDate & dateFirst, const QDate & dateLast,
                  const bool & doErase = false);
    void downloadAndAdd(const QDate & date, const bool & doErase = false);
    void downloadAndAdd(const QDate & dateFirst, const QDate & dateLast,
                        const bool & doErase = false);
  private:
    static const int COLUMNS_COUNT;
    QSqlDatabase database;
    QString databaseName;
    QString rawDataDir;
    QString columnsDir;
    bool connected;
};
