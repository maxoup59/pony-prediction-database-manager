#include "main-window.hpp"
#include "ui_main-window.h"

#include <QDate>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    databaseManager("ponyprediction")
{
    ui->setupUi(this);
    if(!databaseManager.connect())
    {
        if(databaseManager.createDatabase())
            databaseManager.connect();
    }
    bool doErase = false;
    if(Util::getLineFromConf("erase") == "true")
        doErase = true;
    if(Util::getLineFromConf("download") == "true" && Util::getLineFromConf("add") == "true")
        databaseManager.downloadAndAdd(QDate::fromString(Util::getLineFromConf("startDate"),"yyyy-MM-dd")
                                       , QDate::fromString(Util::getLineFromConf("endDate"),"yyyy-MM-dd")
                                       , doErase);
    else if(Util::getLineFromConf("download") == "false" && Util::getLineFromConf("add") == "true")
        databaseManager.add(QDate::fromString(Util::getLineFromConf("startDate"),"yyyy-MM-dd")
                            , QDate::fromString(Util::getLineFromConf("endDate"),"yyyy-MM-dd")
                            , doErase);
    else if(Util::getLineFromConf("download") == "true" && Util::getLineFromConf("add") == "false")
        databaseManager.download(QDate::fromString(Util::getLineFromConf("startDate"),"yyyy-MM-dd")
                                 , QDate::fromString(Util::getLineFromConf("endDate"),"yyyy-MM-dd")
                                 , doErase);
}

MainWindow::~MainWindow()
{
    delete ui;
}
