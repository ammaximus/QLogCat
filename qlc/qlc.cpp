#include "qlc.h"

//QProcess* QLC::logCat = new QProcess();
QLocalSocket* QLC::logCatSock = 0;
QLCEnv QLC::enviroment;

void QLC::configure()
{
    if (firstLaunch)    // Инициализация, разбор переменных окружения
        // TODO Переписать с использованием инициализатора функции статик
    {
        QString type = QString(getenv("QLC"));
        if (type==""){
            workFlow=QLC::Off;
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
                qlcAddress.setAddress(address);
            }
        }
        else if (type=="G"){
            logCatSock = new QLocalSocket();
            workFlow = QLC::LogCat;
            logCatSock->connectToServer("QLOGCAT",QLocalSocket::WriteOnly);

//            QString logCatAddress = QString(getenv("QLOGCAT"));
//            if (logCatAddress==""){
//                logCat->start("QLogCat");
//            }
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

QHostAddress QLC::qlcAddress = QHostAddress::LocalHost;

void QLC::setQLogCatAddress(QHostAddress address, int port)
{
    QLC::port = port;
    QLC::qlcAddress = address;
}

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
        QDataStream str(&msg, QIODevice::WriteOnly);
        str << QLC_COMPLEX << level << log << time << file << line;
        str << messageVector << cats;
        QLC::writeDatagramm(msg);
    }

    else if (workFlow == QLC::Console){
        for (int i=0; i<messageVector.size(); i++){
            std::cout << messageVector[i].toString().toLocal8Bit().constData();
        }
        std::cout << std::endl;
    }
    else if (workFlow == QLC::LogCat){
        if (logCatSock->isOpen()){
            QByteArray msg;
            QDataStream str(&msg, QIODevice::WriteOnly);
            str << QLC_COMPLEX << level << log << time << file << line;
            str << messageVector << cats;
            logCatSock->write(msg);
        }
        else
            qDebug()<<"Server not found";

    }
}

void QLC::writeDatagramm(QByteArray &msg)
{
    QUdpSocket socket;
    socket.writeDatagram(msg.data(),msg.size(),QHostAddress(qlcAddress), port);
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
    _messages.append(cat->toString());
    _cats.insert(cat->toString(), cat->dump());
    return *this;
}

QLCEnv::QLCEnv()
    :ls(0)
{
    std::cout << "ENVIROMENT CREATED" << std::endl;
}

QLCEnv::~QLCEnv()
{
    if(ls!=0);
        delete ls;
    std::cout<<"ENVIROMENT DELETED" << std::endl;
}
