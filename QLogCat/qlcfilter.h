#ifndef QLCFILTER_H
#define QLCFILTER_H

#include "qlc.h"
#include <QTextCodec>

class QLCLine{  // Строка сообщения
public:
    static bool qt3;
    QVector <QVariant> messageVector;
    QMap<QString, QStringList> cats;
    QTime time;
    QLC::LevelType level;
    QString log;
    QString file;
    int lineNumber;

    QString getFullMessage();
};

class QLCFilter{
public:
    // TODO - compact bools, cose not any reasons found
    bool verbose;
    bool debug;
    bool info;
    bool warning;
    bool error;
    bool assert;
    bool fatal;

    QString name;         // Filter name
    QString textContains; // Filter by contained text
    QString logTag;       // Filter by log accessory

    QLCFilter();
    bool filtrate(QLCLine &line);
    bool setLevelLimit(int level);
};

#endif // QLCFILTER_H
