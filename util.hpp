#pragma once

#include <random>
#include <QDebug>
#include <QVector>
#include <QString>

class MainWindow;

class Util// : public QObject
{
    // Q_OBJECT

  public:
    static float getRandomFloat(const float & min, const float & max);
    static int getRandomInt(const int & min, const int & max);
    template<typename T> static void debug(const T & arg);
    static void debug(const QVector<int> & arg);
    static void init(MainWindow * mainWindow);
    static QString getLineFromConf(const QString & id);
  private:
    static MainWindow * mainWindow;
    /*signals:
    static void bob();*/
};
