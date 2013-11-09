#include "qlogcat.h"
#include "ui_qlogcat.h"


QLogCat::QLogCat(QWidget *parent) :
    QWidget(parent), customFilter(0),
    ui(new Ui::QLogCat)
{
    ui->setupUi(this);
    socket = new QUdpSocket;
    socket->bind(13093);
    connect(socket,SIGNAL(readyRead()), SLOT(readDatagramms()));

    // Table formating
    ui->tblLog->setRowCount(0);
    ui->tblLog->setColumnCount(6);
    ui->tblLog->horizontalHeader()->resizeSection(0,20);
    ui->tblLog->horizontalHeader()->resizeSection(1,60);
    ui->tblLog->horizontalHeader()->resizeSection(2,60);
    ui->tblLog->horizontalHeader()->resizeSection(3,60);
    ui->tblLog->horizontalHeader()->resizeSection(4,60);
    ui->tblLog->horizontalHeader()->resizeSection(5,460);
    ui->tblLog->verticalHeader()->setDefaultSectionSize(20);
    ui->tblLog->verticalHeader()->setShown(false);
    ui->tblLog->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tblLog->setSelectionBehavior(QAbstractItemView::SelectRows);

    QLCFilter *f = new QLCFilter();
    f->name="none";
    filters.append(f); // Add empty filter
    ui->listFilter->addItem(filters.last()->name);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
}

QLogCat::~QLogCat()
{
    delete ui;
}

void QLogCat::readDatagramms()
{
    while(socket->hasPendingDatagrams()){
        QByteArray msg;
        msg.resize(socket->pendingDatagramSize());
        socket->readDatagram(msg.data(),msg.size());
        processMessage(msg);
    }
}

void QLogCat::processMessage(QByteArray msg)
{
    QDataStream str(&msg,QIODevice::ReadOnly);
    if (QLCLine::qt3) // Red hat / ASP Linux (Qt 3.3)
        str.setVersion(6);
    int messageType;
    str >> messageType;

    int levelType;
    QLCLine line;

    str >> levelType >> line.log >> line.time >> line.file >> line.lineNumber;
    line.level = QLC::LevelType(levelType);

    QStringList dump;
    QString message;

    QVector <QVariant> messages;
    QMap<QString, QStringList> cats;

    switch(messageType){
    case QLC_COMPLEX:
        str >> line.messageVector >> line.cats;
        addLine(line);
        break;
    default:
        Q_ASSERT_X(true, "LOGCAT", "Unexpected message type");
        // change to "do nothing" after debug!
        break;
    }
}

void QLogCat::addLine(QLCLine line)
{
    if (filtrateAll(line)){
        int elem = ui->tblLog->rowCount();
        ui->tblLog->insertRow(elem);

        QString letterType;
        switch(line.level){
        case QLC::Warning:
            letterType = "W";
            break;
        case QLC::Verbose:
            letterType = "V";
            break;
        case QLC::Debug:
            letterType = "D";
            break;
        case QLC::Info:
            letterType = "I";
            break;
        case QLC::Error:
            letterType = "E";
            break;
        case QLC::Assert:
            letterType = "A";
            break;
        case QLC::Fatal:
            letterType = "FE";
            break;
        }

        ui->tblLog->setItem(elem,0,new QTableWidgetItem(letterType));
        ui->tblLog->setItem(elem,1,new QTableWidgetItem(line.time.toString("mm:ss.zzz")));
        ui->tblLog->setItem(elem,2,new QTableWidgetItem(line.log));
        ui->tblLog->setItem(elem,3,new QTableWidgetItem(line.file));
        ui->tblLog->setItem(elem,4,new QTableWidgetItem(QString::number(line.lineNumber)));
        ui->tblLog->setItem(elem,5,new QTableWidgetItem(line.getFullMessage()));

        // TODO Autoscroll
        //ui->tblLog->verticalScrollBar()->setSliderPosition(ui->tblLog->verticalScrollBar()->maximum());
        // Почемуто отстает на 4
    }
}

void QLogCat::refresh()
{
    while (ui->tblLog->rowCount()>0)
        ui->tblLog->removeRow(0);
    foreach (QLCLine line, lines){
        addLine(line);
    }
}

bool QLogCat::filtrateAll(QLCLine line)
{
    bool result = mainFilter.filtrate(line);
    if (customFilter!=0)
        result = (result && customFilter->filtrate(line));
    return result;
}

void QLogCat::on_pbTest_clicked()
{
    qlc("Logname") << "Hello";
    qlcd("Hello") << "Message!";
    qlce("DasCool") << "Check int";
}

void QLogCat::on_pbSettings_clicked()
{
    QString addresses;
    foreach(QHostAddress address, QNetworkInterface::allAddresses()){
        addresses.append(address.toString());
        addresses.append(" - ");
        addresses.append(QString::number(address.toIPv4Address()));
        addresses.append("\n");
    }
    QMessageBox::information(this, tr("Local host address"), addresses);
}

void QLogCat::on_cbSearchLevel_currentIndexChanged(int index)
{
    mainFilter.setLevelLimit(index);
    refresh();
}

void QLogCat::on_leSearch_textEdited(const QString &text)
{
    mainFilter.textContains = text;
    refresh();
}

void QLogCat::on_tblLog_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if (currentRow<0 or currentColumn<0){
        ui->txtMessage->setText("");
        return;
    }
    ui->txtMessage->setText(ui->tblLog->item(currentRow,4)->text());
}

void QLogCat::on_listFilter_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    foreach (QLCFilter* filter, filters){
        if (filter->name == current->text()){
            customFilter = filter;
            refresh();
        }
    }
}

void QLogCat::on_pbAddFilter_clicked()
{
    AddFilterDialog dlg;
    if (dlg.exec() == QDialog::Accepted){
        filters.append(dlg.getFilter());
        ui->listFilter->addItem(filters.last()->name);
    }
}

void QLogCat::on_pbClear_clicked()
{
    lines.clear();
    refresh();
}

void QLogCat::on_cbQt3_stateChanged(int arg1)
{
    QLCLine::qt3 = (arg1==Qt::Checked);
}
