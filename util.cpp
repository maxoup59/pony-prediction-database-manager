#include "util.hpp"
#include "main-window.hpp"
#include <QString>
#include <QThread>
#include <QFile>
#include <QXmlStreamReader>

MainWindow * Util::mainWindow = nullptr;

float Util::getRandomFloat(const float &min, const float &max)
{
  return ((static_cast <float>(qrand()))
          * (max - min)
          / (static_cast<float>(RAND_MAX)))
      + min;
}
int Util::getRandomInt(const int &min, const int &max)
{
  int r = getRandomFloat(min-1, max+1);
  if(r < min || r > max)
    r = getRandomInt(min, max);
  return r;
}
template<typename T> void Util::debug(const T &arg)
{
  qDebug() << arg;
}
void Util::debug(const QVector<int> & arg)
{
  QString str = " | ";
  for(int i = 0; i < arg.size() ; i++)
    str += QString::number(arg[i]) + " | ";
  qDebug() << str;
}

void Util::init(MainWindow * mainWindow)
{
  Util::mainWindow = mainWindow;
}



QString Util::getLineFromConf(const QString & id)
{
  QString output = "";
  QFile file("./conf.xml");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    qDebug() << "error eiurgyeirug";
    return QString();
  }
  QXmlStreamReader xml(&file);
  while (!xml.atEnd())
  {
    QXmlStreamReader::TokenType token = xml.readNext();
    if(token == QXmlStreamReader::StartElement)
    {
      if(xml.name() == id)
      {
        output = xml.readElementText();
      }
    }
  }
  return output;
}
