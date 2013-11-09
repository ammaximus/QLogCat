#-------------------------------------------------
#
# Project created by QtCreator 2013-11-08T08:45:56
#
#-------------------------------------------------

QT       += network

VERSION = 1.0.3

TARGET = qlc
TEMPLATE = lib

DESTDIR = ../Bin
if QT_VERSION < 0x040000{
    CONFIG += staticlib
}

DEFINES += QLC_LIBRARY

SOURCES += qlc.cpp

HEADERS += qlc.h\
        qlc_global.h

