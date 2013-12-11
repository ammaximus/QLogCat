#-------------------------------------------------
#
# Project created by QtCreator 2013-10-18T15:46:00
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#Version
VERSION = 1.0.3

TARGET = QLogCat
TEMPLATE = app
INCLUDEPATH = ../qlc/

DESTDIR = ../Bin
LIBS += -L../Bin/ \
        -lqlc

SOURCES += main.cpp\
        qlogcat.cpp \
    addfilterdialog.cpp \
    qlcfilter.cpp

HEADERS  += qlogcat.h \
    addfilterdialog.h \
    qlcfilter.h

FORMS    += qlogcat.ui \
    addfilterdialog.ui

RESOURCES += \
    pic.qrc
