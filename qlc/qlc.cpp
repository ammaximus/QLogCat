#include "qlc.h"


void QLC::configure()
{
    if (firstLaunch)    // Инициализация, разбор переменных окружения
    {
    QString type = QString(getenv("QLC"));
    if (type==""){
    }
    else if (type=="C"){
        workFlow = QLC::Console;
    }
    else if (type.contains("N")){
        workFlow = QLC::Net;
        if (type.length()==10){
            int address;
            type.remove(0,1);
            bool ok = false;
            address = type.toInt(&ok);
            if (ok)
            #ifdef QT3
                qlcAddress = address;
            #endif
            #ifdef QT4
                qlcAddress.setAddress(address);
            #endif
            }
        }
    firstLaunch = false;
    }
}

QLC::QLC(QString file, int line, LevelType type, QString log)
    :_type (type), _log(log), _file(file), _line(line) {}

QLC::~QLC()
{
    sendMessage(_messages, _cats, _type, _log, QTime::currentTime(), _file, _line);
}

// Статические переменные
QLC::FlowType QLC::workFlow = QLC::Off;  // Направление потока вывода
bool QLC::firstLaunch = true;        // Определяет необходимость инициализации

// Адрес по умолчанию - локальная машина
int QLC::port = QLC_DEFPORT;
#ifdef QT4
QHostAddress QLC::qlcAddress = QHostAddress::LocalHost;

void QLC::setQLogCatAddress(QHostAddress address, int port)
{
    QLC::port = port;
    QLC::qlcAddress = address;
}
#endif

#ifdef QT3
int QLC::qlcAddress = 0x7F000001;

void QLC::setQLogCatAddress(int address, int port)
{
    QLC::port = port;
    QLC::qlcAddress = address;
}
#endif

///////////////////////
// Отправка сообщения
///////////////////////

void QLC::sendMessage(QVector<QVariant> messageVector, QMap<QString, QStringList> cats, LevelType level, QString log, QTime time, QString file, int line)
{
    configure();
    if (workFlow == QLC::Off){
    // do nothing
    }

    else if (workFlow == QLC::Net){
    QByteArray msg;
    #ifdef QT4
    QDataStream str(&msg, QIODevice::WriteOnly);
    #endif
    #ifdef QT3
    QDataStream str(msg, IO_WriteOnly);
    str.setVersion(6);
    #endif
    str << QLC_COMPLEX << level << log << time << file << line;
    str << messageVector << cats;
    QLC::writeDatagramm(msg);
    }

    else if (workFlow == QLC::Console){
        for (int i=0; i<messageVector.size(); i++){
        #ifdef QT4
            std::cout << messageVector[i].toString().toLocal8Bit().constData();
        #endif
        #ifdef QT3
            std::cout << messageVector[i].toString().ascii(); // Дибильная МСВС кодировка
        #endif
        }
        std::cout << std::endl;
    }
}

void QLC::writeDatagramm(QByteArray &msg)
{
    #ifdef QT4
        QUdpSocket socket;
        socket.writeDatagram(msg.data(),msg.size(),QHostAddress(qlcAddress), port);
    #endif
    #ifdef QT3
        QSocketDevice socket(QSocketDevice::Datagram);
        socket.setBlocking(false);
        socket.writeBlock(msg,msg.size(),qlcAddress, port);
    #endif
}


///////////
// Функции оператора <<
///////////


QLC& QLC::operator <<(const QVariant var)
{
    _messages.append(var);
    return *this;
}

QLC &QLC::operator <<(const QLCattable *cat)
{
    _messages.append(cat->printDebug());
    _cats.insert(cat->printDebug(), cat->dump());
    return *this;
}
