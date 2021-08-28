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

    void widgetItemClicked(QTableWidgetItem*);
    void widgetItemClicked();
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

    const QString styleLineEditEnabled = "color: rgb(238, 238, 236); background-color: rgb(46, 52, 54)"; // Enaled
    const QString styleDisabled = "color: rgb(238, 238, 236); background-color: rgb(85, 87, 83)"; // Disabled
    const QString styleBtnGreenEnabled = "background-color: green"; // enabled green
    const QString styleBtnDisabled = "";
    const QString styleBtnYellowEnabled = "background-color: yellow"; // enabled yellow
    const QString styleBtnRedEnabled = "background-color: red";
    const QString styleBtnEditarEnbled = "background-color: rgb(206, 92, 0)";
    const QString sytleBtnBlueEnabled = "background-color: blue"; // enabled blue
    const QString styleBtnLightBlueEnabled = "background-color: rgb(154, 209, 255)"; // light blue

    enum windowMode{
        Search,
        Select,
        Insert,
        Edit
    };
    void clearInputs();
    void setMode(windowMode wMode);
    short searchLastIndex();
    void enableToInsertNew();
    void init();
};
#endif // MAINWINDOW_H
