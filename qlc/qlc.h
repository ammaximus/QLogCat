#ifndef QLC_H
#define QLC_H

#if defined(QLC_LIBRARY)
#define QLCSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QLCSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QHostAddress>
#include <QUdpSocket>
#include <QTime>
#include <QStringList>
#include <iostream>
#include <QVariant>
#include <QProcess>
#include <QLocalSocket>
#include <QApplication>
/*TODO Идея следующая:
  В статической области завести хешмэп Localsockets.
  Ключом доступа будет являться ид потока, если не найдено будет создаваться новый.
  Таким образом, мы получаем ограниченное число потоков.
  Предыдущее наверное не поможет. В функции записи мы каждый раз создавали новый удп сокет.
  Здесь же унас один сокет и это может вызвать проблемы при многопоточном обращении.
  Скореевсего придется каждый раз создавать новый сокет на каждую транзакцию.
  Идея с потоками немного не выйдет, т.к. я пока не могу придумать как их удалаять
  Стоп. Статический класс энвайромент может мне помочь. Напишем объект статического
  класса который оказывает служебные действия! Стоит попробовыать еще успею зарядка есть.
*/
#define QLC_DEFPORT 13093   // Порт по умолчанию
#define QLC_DEFTAG "System" // Журнал по умолчанию
#define QLC_QT 1            // Стандартный тип Qt - QVariant
#define QLC_CAT 2           // Тип QLCattable
#define QLC_COMPLEX 3       // Смесь из двух

// Уровни сообщений
#define LEVEL_VERBOSE 0
#define LEVEL_DEBUG   1
#define LEVEL_INFO    2
#define LEVEL_WARNING 3
#define LEVEL_ERROR   4
#define LEVEL_ASSERT  5
#define LEVEL_FATAL   6
#define LEVEL_MONITOR 7

// Макроконтур
#define qlc MacroCall(__FILE__, __LINE__)
#define qlcv MacroCall(__FILE__, __LINE__, LEVEL_VERBOSE)
#define qlcd MacroCall(__FILE__, __LINE__, LEVEL_DEBUG)
#define qlci MacroCall(__FILE__, __LINE__, LEVEL_INFO)
#define qlcw MacroCall(__FILE__, __LINE__, LEVEL_WARNING)
#define qlce MacroCall(__FILE__, __LINE__, LEVEL_ERROR)
#define qlca MacroCall(__FILE__, __LINE__, LEVEL_ASSERT)
#define qlcf MacroCall(__FILE__, __LINE__, LEVEL_FATAL)
#define qlcm MacroCall(__FILE__, __LINE__, LEVEL_MONITOR)

/// /////
/// Интерфейс, позволяющий выводить в отладчик целые объекты
/// /////

class QLCSHARED_EXPORT QLCattable
{
public:
    virtual QString toString()const =0;    // Вывести отладочное сообщение с краткой информацией
    virtual QStringList dump()const =0;      // Дамп содержимого объекта
};

class QLCSHARED_EXPORT QLCEnv{
public:
    QLCEnv();
    ~QLCEnv();
    QLocalSocket *ls;
    void setLS(QLocalSocket *ls){
        Q_ASSERT(ls!=0);
        this->ls=ls;
    }
};

/// ///////////
/// Класс отправки отладочных сообщений для QLogCat
/// ///////////
class QLCSHARED_EXPORT QLC
{
public:
    // Уровень сообщения
    enum LevelType{
        Verbose=LEVEL_VERBOSE,  // Временное отладочное сообщение (не должно быть фиксировано)
        Debug=LEVEL_DEBUG,      // Отладка
        Info=LEVEL_INFO,        // Информирующее сообщение
        Warning=LEVEL_WARNING,  // Предупреждение
        Error=LEVEL_ERROR,      // Ошибка
        Assert=LEVEL_ASSERT,    // Утверждение
        Fatal=LEVEL_FATAL,      // Фатальная ошибка
        Monitor=LEVEL_MONITOR   // Монитор
    };
    // Направление потока
    enum FlowType{
        Off,    // Выключена
        Net,    // По сети
        Console, // В консоль
        LogCat  // Напрямую в QLogCat
    };

    static int port;
    static bool firstLaunch;
    static FlowType workFlow;
    static QLocalSocket *logCatSock;
    static QLCEnv enviroment;

    static void configure();

    static QHostAddress qlcAddress;
    static void setQLogCatAddress(QHostAddress address, int port=QLC_DEFPORT);

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
