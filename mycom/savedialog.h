﻿#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include <QDialog>

namespace Ui {
    class saveDialog;
}

class saveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit saveDialog(QWidget *parent = 0);
    ~saveDialog();

private slots:
    void on_ySaveButton_clicked();

    void on_nSaveButton_clicked();

private:
    Ui::saveDialog *ui;
};

#endif // SAVEDIALOG_H
