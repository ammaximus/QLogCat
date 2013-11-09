#ifndef QLC_H
#define QLC_H

#include "qlc_global.h"

#ifdef QT4
    #include <QHostAddress>
    #include <QUdpSocket>
    #include <QTime>
    #include <QStringList>
    #include <iostream>
#endif

#ifdef QT3
    #include <qsocketdevice.h>
    #include <qdatetime.h>
    #include <qstringlist.h>
    #include <qvaluevector.h>
    #include <qvariant.h>
#endif

#define QLC_DEFPORT 13093   // Default port
#define QLC_DEFTAG "System" // Default log

#define QLC_QT 1            // QVariant based message
#define QLC_CAT 2           // QLCattable based message
#define QLC_COMPLEX 3       // Comlex message

// Macrocontur
#define qlc MacroCall(__FILE__, __LINE__)
#define qlcv MacroCall(__FILE__, __LINE__, 0)
#define qlcd MacroCall(__FILE__, __LINE__, 1)
#define qlci MacroCall(__FILE__, __LINE__, 2)
#define qlcw MacroCall(__FILE__, __LINE__, 3)
#define qlce MacroCall(__FILE__, __LINE__, 4)
#define qlca MacroCall(__FILE__, __LINE__, 5)
#define qlcf MacroCall(__FILE__, __LINE__, 6)

/// /////
/// Interface allow log own object
/// /////

class QLCSHARED_EXPORT QLCattable
{
public:
    virtual QString printDebug()const =0;    // Short info message
    virtual QStringList dump()const =0;      // Object dump
};

/// ///////////
/// Main logger class
/// ///////////
class QLCSHARED_EXPORT QLC
{
public:
    // Уровень сообщения
    enum LevelType{
        Verbose=0,  
        Debug=1,    
        Info=2,     
        Warning=3,  
        Error=4,    
        Assert=5,   
        Fatal=6     
    };

    enum FlowType{
        Off,    
        Net,    
        Console 
    };

    static int port;
    static bool firstLaunch;
    static FlowType workFlow;

    static void configure();

#ifdef QT4
    static QHostAddress qlcAddress;
    static void setQLogCatAddress(QHostAddress address, int port=QLC_DEFPORT);
#endif
#ifdef QT3
    static int qlcAddress;
    static void setQLogCatAddress(int address, int port);
#endif

    QLC(QString file, int line, LevelType type, QString log = QLC_DEFTAG);  // Object constructor
    ~QLC(); // Destructor, which send information after <<

    QLC& operator<<(const QVariant var);
    QLC& operator<<(const QLCattable *cat);

protected:
    // Отправка сообщений
    static void sendMessage(QVector <QVariant> messageVector, QMap<QString, QStringList> _cats, LevelType level, QString log, QTime time, QString file, int line);
    static void writeDatagramm(QByteArray &msg);

    // Данные
    QVector <QVariant> _messages;
    QMap<QString, QStringList> _cats;
    QString _log;
    LevelType _type;
    QString _file;
    int _line;
};

class QLCSHARED_EXPORT MacroCall
{
    QLC::LevelType level;
    QString file;
    int line;
public:
    MacroCall(QString f, int l)
        :level(QLC::Debug), file(f), line(l){}

    MacroCall(QString f, int l, int type)
        :level(QLC::LevelType(type)), file(f), line(l){}

    QLC operator()(){
        return QLC(file, line, level);
    }
    QLC operator()(QString log){
        return QLC(file, line, level, log);
    }
};

#endif // QLC_H
