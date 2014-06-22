#ifndef TESTER_H
#define TESTER_H

#include <QWidget>
#include <QThread>
#include "qlc.h"

namespace Ui {
class Tester;
}

class SomeThread:public QThread{
public:
    int number;
    static int counter;
    bool running;
    SomeThread()
        :number(counter++), running(true){}
    void run();
};

class Tester : public QWidget
{
    Q_OBJECT
    QVector<SomeThread*> threads;
    
public:
    explicit Tester(QWidget *parent = 0);
    ~Tester();
    
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::Tester *ui;
};

#endif // TESTER_H
