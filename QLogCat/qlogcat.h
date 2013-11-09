#ifndef QLOGCAT_H
#define QLOGCAT_H

#include <QWidget>
#include <QHostAddress>
#include <QUdpSocket>
#include <QList>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QListWidgetItem>
#include <QScrollBar>

#include "addfilterdialog.h"
#include "qlcfilter.h"

namespace Ui {
class QLogCat;
}

class QLogCat : public QWidget
{
    Q_OBJECT
    
public:
    explicit QLogCat(QWidget *parent = 0);
    ~QLogCat();
    
public slots:
    void readDatagramms();
private slots:
    void on_pbTest_clicked();
    void on_pbSettings_clicked();
    void on_cbSearchLevel_currentIndexChanged(int index);
    void on_leSearch_textEdited(const QString &text);
    void on_tblLog_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_pbAddFilter_clicked();
    void on_listFilter_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_pbClear_clicked();
    void on_cbQt3_stateChanged(int arg1);

private:
    Ui::QLogCat *ui;
    QUdpSocket *socket;
    QList<QLCLine> lines;
    QList<QLCFilter*> filters;
    QTextCodec *codec;

    void processMessage(QByteArray msg);
    void appendMessage(QVariant var, QLC::LevelType type, QString log, QTime time);
    void appendMessage(QString msg, QStringList list, QLC::LevelType type, QString log, QTime time);
    void appendMessage(QVector<QVariant> messages, QMap<QString,QStringList> cats, QLC::LevelType type, QString log, QTime time);

    void addLine(QLCLine line);
    void refresh();

    QLCFilter mainFilter;
    QLCFilter *customFilter;

    bool filtrateAll(QLCLine line);
};

#endif // QLOGCAT_H
