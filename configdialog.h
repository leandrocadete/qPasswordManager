#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <iostream>
#include "configuration.h"
#include "databasefilemodel.h"
#include <QStringListModel>
#include <QAbstractListModel>
#include <QFile>
#include <QDir>
#include <QStringList>
#include <QMessageBox>
#include <QListView>
#include "manager.h"
#include "customqlistview.h"

namespace Ui {
    class ConfigDialog;
}

class ConfigDialog : public QDialog {
    Q_OBJECT
public:
    explicit ConfigDialog(QWidget *parent = nullptr);
    ~ConfigDialog();
private slots:
    void on_buttonBox_accepted();
    void addItem();
    void itemClicked(const QModelIndex &index);
    void itemClicked(int n);
    void on_buttonBox_clicked(QAbstractButton *button);    
    void deleteItem();
private:
    Ui::ConfigDialog *ui;
    Configuration *config;
    databaseFileModel *dataBaseModel;
    QListView *listView;
    QStringListModel *model;
    int selectedIndex;
    QMessageBox *msgDelete;

    void listDbFiles();
};

#endif // CONFIGDIALOG_H
