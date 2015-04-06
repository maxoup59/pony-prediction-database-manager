#pragma once

#include <QDate>
#include <QNetworkRequest>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QString>

class DatabaseManager : public QObject
{
    Q_OBJECT
  public:
    DatabaseManager();
    ~DatabaseManager();
    void createDatabase();
    void add(const QDate & date);
    void add(const QDate & dateFirst, const QDate & dateLast);
    void download(const QDate & date);
    void download(const QDate & dateFirst, const QDate & dateLast);
    void downloadNext();
  private slots:
    //void startNextDownload();
    //void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();
    void downloadReadyRead();
  private:
    static const QString DIR;
    QString databaseName;
    QNetworkAccessManager manager;
    QNetworkReply * currentDownload;
    QFile output;
    QDate dateFirst;
    QDate dateLast;
    QDate dateDownloading;
};
