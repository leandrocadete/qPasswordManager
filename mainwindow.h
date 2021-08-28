#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QTableWidget>
#include <QInputDialog>
#include <QScrollBar>
#include "QMessageBox"
#include "Encrypt.h"
#include "manager.h"
#include "manager.h"
#include "configuration.h"
#include "configdialog.h"

using namespace std;
namespace Ui {
    class MainWindow;
}
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();    
private slots:
    void createTable();
    void recreateTable();
    void on_pushButton_Update_clicked();
    void on_pushButton_Search_clicked();
    void on_pushButton_Editar_clicked();
    void on_pushButton_New_clicked();
    void on_pushButton_save_clicked();
    void on_pushButton_cancel_clicked();
    void on_pushButton_Delete_clicked();

    void enableEdit(QTableWidgetItem*);
    void enableEdit();
    void search();
    string** search(string str, int* countResult);
    void on_actionAbout_triggered();
    void onActionExitTriggered();
    void on_action_New_triggered();

    void on_action_Current_key_triggered();
    void on_actionConfiguration_triggered();
    void on_action_Delete_triggered();

private:
    string str_key; // Default Key to decript the passwords
    QTableWidget *qtable;
    Ui::MainWindow *ui;
    string** str_pwd; // pwds
    bool flagInit = false;
    string strPwdFile;
    Configuration *config;
    ConfigDialog *configDialog;

    const QString lineEditEnabled = "color: rgb(238, 238, 236); background-color: rgb(46, 52, 54)"; // Enaled
    const QString disabled = "color: rgb(238, 238, 236); background-color: rgb(85, 87, 83)"; // Disabled
    const QString btnGreenEnabled = "background-color: green"; // enabled green
    const QString btnDisabled = ""; // Disabled green
    const QString btnYellowEnabled = "background-color: yellow"; // enabled yellow
    const QString btnRedEnabled = "background-color: red";
    const QString btnEditarEnbled = "background-color: rgb(206, 92, 0)";
    const QString btnBlueEnabled = "background-color: blue"; // enabled blue
    const QString btnLightBlueEnabled = "background-color: rgb(154, 209, 255)"; // light blue

    static const short SEARCH = 0;
    static const short INSERT_NEW = 1;
    static const short EDIT = 2;

    void setMode(short mode);
    short searchLastIndex();
    void enableToInsertNew();
    void init();
};
#endif // MAINWINDOW_H
