#include "localsocketreader.h"

LocalSocketReader::LocalSocketReader(QLocalSocket* ls, QObject *parent) :
    ls(ls), QObject(parent)
{
    Q_CHECK_PTR(ls);
    connect(ls,SIGNAL(readyRead()),SLOT(onReadyRead()));
    connect(ls,SIGNAL(stateChanged(QLocalSocket::LocalSocketState)),SLOT(onStateChanged(QLocalSocket::LocalSocketState)));
}

LocalSocketReader::~LocalSocketReader()
{
    // ls already deleted (mb)
}

void LocalSocketReader::onReadyRead()
{
    emit newData(ls->readAll());
}

void LocalSocketReader::onStateChanged(QLocalSocket::LocalSocketState state)
{
    switch (state) {
UnconnectedState:
ConnectingState:
    break;
ConnectedState:
    qDebug()<<"Slot connected";
    break;
ClosingState:
    qDebug()<<"Slot clozed";
    break;
    default:
        break;
    }
}
