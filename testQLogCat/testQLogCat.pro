#-------------------------------------------------
#
# Project created by QtCreator 2013-12-12T10:40:43
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testQLogCat
TEMPLATE = app

INCLUDEPATH += ../qlc

LIBS += -L../Bin/ \
        -lqlc

SOURCES += main.cpp\
        tester.cpp

HEADERS  += tester.h

FORMS    += tester.ui

CONFIG += thread
