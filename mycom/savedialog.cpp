#include "savedialog.h"
#include "ui_savedialog.h"

saveDialog::saveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::saveDialog)
{
    ui->setupUi(this);
}

saveDialog::~saveDialog()
{
    delete ui;
}

void saveDialog::on_ySaveButton_clicked()
{
    accept();
}

void saveDialog::on_nSaveButton_clicked()
{
    reject();
}
