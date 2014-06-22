#ifndef LOCALSOCKETREADER_H
#define LOCALSOCKETREADER_H

#include <QObject>
#include <QLocalSocket>
#include <iostream>

class LocalSocketReader :public QObject{
    Q_OBJECT
public:
    LocalSocketReader(QLocalSocket *ls, QObject *parent=0);
    ~LocalSocketReader();
    bool active;
signals:
    void newData(QByteArray data);
private slots:
    void onReadyRead();
    void onStateChanged(QLocalSocket::LocalSocketState state);
private:
    QLocalSocket *ls;
};


#endif // LOCALSOCKETREADER_H
