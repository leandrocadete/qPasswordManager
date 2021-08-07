#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <iostream>
#include "configuration.h"
#include "databasefilemodel.h"
#include <QStringListModel>
#include <QFile>
#include <QDir>
#include <QStringList>
namespace Ui {
    class ConfigDialog;
}

class ConfigDialog : public QDialog {
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = nullptr);
    ~ConfigDialog();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);
    void on_buttonBox_accepted();

private:
    Ui::ConfigDialog *ui;
    Configuration *config;
    databaseFileModel *dataBaseModel;
    QStringListModel *model;

    void listDbFiles();
};

#endif // CONFIGDIALOG_H
