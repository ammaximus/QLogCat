#include "addfilterdialog.h"
#include "ui_addfilterdialog.h"

AddFilterDialog::AddFilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFilterDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

AddFilterDialog::~AddFilterDialog()
{
    delete ui;
}

QLCFilter* AddFilterDialog::getFilter()
{
    QLCFilter *filter = new QLCFilter;
    filter->name = ui->leName->text();
    filter->textContains = ui->leText->text();
    filter->logTag = ui->leLog->text();
    filter->setLevelLimit(ui->cbLevel->currentIndex());

    return filter;
}

void AddFilterDialog::on_leName_textEdited(const QString &arg1)
{
    if (arg1!="")
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    else
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}
