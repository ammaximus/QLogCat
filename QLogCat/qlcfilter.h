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
public: //! Так и не получилось обосновать целесообразность показа кусков,
    //! нужно отрефакториться в один признак
    bool verbose; // Признаки отображения сообщений по типам
    bool debug;
    bool info;
    bool warning;
    bool error;
    bool assert;
    bool fatal;

    QString name;         // Название фильтра
    QString textContains; // Содержит текст
    QString logTag;       // Принадлежит журналу

    QLCFilter();
    bool filtrate(QLCLine &line);
    bool setLevelLimit(int level);
};

#endif // QLCFILTER_H
