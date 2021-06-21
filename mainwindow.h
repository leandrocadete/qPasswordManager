#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QTableWidget>
#include <QInputDialog>
#include "manager.h"

using namespace std;
namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    string str_key; // Default Key to decript the passwords
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
    void enableEdit(QTableWidgetItem*);
    void enableEdit();
    void showMsg();
    void search();
    string** search(string str, int* countResult);
    void on_actionAbout_triggered();
    void onActionExitTriggered();
    void on_action_New_triggered();

private:
    QTableWidget *qtable;
    Ui::MainWindow *ui;
    string** str_pwd; // pwds

    static const short SEARCH = 0;
    static const short INSERT_NEW = 1;
    static const short EDIT = 2;

    void setMode(short mode);
    short searchLastIndex();
    void enableToInsertNew();
    //void new_password();
};
#endif // MAINWINDOW_H
