#-------------------------------------------------
#
# Project created by QtCreator 2015-04-05T19:41:32
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pony-prediction-database-manager
TEMPLATE = app


SOURCES += main.cpp\
        main-window.cpp \
    database-manager.cpp

HEADERS  += main-window.hpp \
    database-manager.hpp

FORMS    += main-window.ui

DISTFILES +=
