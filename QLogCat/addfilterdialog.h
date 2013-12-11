#ifndef ADDFILTERDIALOG_H
#define ADDFILTERDIALOG_H

#include <QDialog>
#include <QPushButton>
#include "qlogcat.h"
#include "qlcfilter.h"

namespace Ui {
class AddFilterDialog;
}

class AddFilterDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddFilterDialog(QWidget *parent = 0);
    ~AddFilterDialog();

    QLCFilter *getFilter();
    
private slots:
    void on_leName_textEdited(const QString &arg1);

private:
    Ui::AddFilterDialog *ui;
};

#endif // ADDFILTERDIALOG_H
