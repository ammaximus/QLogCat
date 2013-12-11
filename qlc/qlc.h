#ifndef QLC_H
#define QLC_H

#include "qglobal.h"
#if QT_VERSION >= 0x040000
    #define QT4

    #if defined(QLC_LIBRARY)
        #define QLCSHARED_EXPORT Q_DECL_EXPORT
    #else
        #  define QLCSHARED_EXPORT Q_DECL_IMPORT
    #endif
#else
    #define QT3
    #define QLCSHARED_EXPORT
    #define QVector QValueVector
#endif

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

#define QLC_DEFPORT 13093   // Порт по умолчанию
#define QLC_DEFTAG "System" // Журнал по умолчанию
#define QLC_QT 1            // Стандартный тип Qt - QVariant
#define QLC_CAT 2           // Тип QLCattable
#define QLC_COMPLEX 3       // Смесь из двух

// Макроконтур
#define qlc MacroCall(__FILE__, __LINE__)
#define qlcv MacroCall(__FILE__, __LINE__, 0)
#define qlcd MacroCall(__FILE__, __LINE__, 1)
#define qlci MacroCall(__FILE__, __LINE__, 2)
#define qlcw MacroCall(__FILE__, __LINE__, 3)
#define qlce MacroCall(__FILE__, __LINE__, 4)
#define qlca MacroCall(__FILE__, __LINE__, 5)
#define qlcf MacroCall(__FILE__, __LINE__, 6)
#define qlcm MacroCall(__FILE__, __LINE__, 7)

/// /////
/// Интерфейс, позволяющий выводить в отладчик целые объекты
/// /////

class QLCSHARED_EXPORT QLCattable
{
public:
    virtual QString printDebug()const =0;    // Вывести отладочное сообщение с краткой информацией
    virtual QStringList dump()const =0;      // Дамп содержимого объекта
};

/// ///////////
/// Класс отправки отладочных сообщений для QLogCat
/// ///////////
class QLCSHARED_EXPORT QLC
{
public:
    // Уровень сообщения
    enum LevelType{
        Verbose=0,  // Временное отладочное сообщение (не должно быть фиксировано)
        Debug=1,    // Отладка
        Info=2,     // Информирующее сообщение
        Warning=3,  // Предупреждение
        Error=4,    // Ошибка
        Assert=5,   // Утверждение
        Fatal=6,    // Фатальная ошибка
        Monitor=7   // Монитор
    };
    // Направление потока
    enum FlowType{
        Off,    // Выключена
        Net,    // По сети
        Console // В консоль
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

    QLC(QString file, int line, LevelType type, QString log = QLC_DEFTAG);  // Создание объекта
    ~QLC(); // То самое место, которое отправляет по завершению цепочки <<

    // Функции выдачи сообщений разного уровня
    // Функции оператора <<
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
