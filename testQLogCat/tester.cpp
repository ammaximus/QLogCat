#include "tester.h"
#include "ui_tester.h"

int SomeThread::counter=0;

Tester::Tester(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tester)
{
    ui->setupUi(this);
}

Tester::~Tester()
{
    delete ui;
}

void Tester::on_pushButton_clicked()
{
    SomeThread *s = new SomeThread();
    s->start();
    threads.append(s);
}

void SomeThread::run()
{
    QString tId = "Thread " + QString::number(number);
    qlcm(tId) << "Start";
    sleep(2);
    while(running)
    {
        qlcm(tId) << "Running";
        qlci(tId) << "Start new cycle";
        int value = qrand();
        if (value%10==3)
            qlce(tId) << "Error";
        sleep(1);
    }
    qlcm(tId) <<  "Complete";
}

void Tester::on_pushButton_2_clicked()
{
    bool ok = false;
    int i = ui->leNumber->text().toInt(&ok);
    if (ok)
        if (i<threads.size())
        {
            threads[i]->running = false;
        }
}
