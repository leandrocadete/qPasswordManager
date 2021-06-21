#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include <fstream>
#include "Encrypt.h"
#include "manager.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    auto args = QCoreApplication::arguments();
    if (args.length() >2)
        str_key = args[1].toStdString();
    bool ok = str_key.size() > 0;
    if(!ok)
        str_key = QInputDialog::getText(this, tr("Key"), tr("Default key to decript passwords (eg. mykey@#@#, #$%keysecret)"
                                                            "\nIf one incorrect key is set you wont see the correct password decription"
                                                            "\nPlease do not forget this key!"), QLineEdit::Password, "", &ok).toStdString();
    if(!ok || str_key.size() == 0){
        cout << str_key << endl;
        exit(-1);
    }
    createTable();
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(onActionExitTriggered()));
}
MainWindow::~MainWindow() {
    delete ui;
}
void MainWindow::createTable() {
    qtable = new QTableWidget(100, 4, ui->gridLayoutWidget);
    qtable->setHorizontalHeaderItem(0, new QTableWidgetItem(QString("ID")));
    qtable->setHorizontalHeaderItem(1, new QTableWidgetItem(QString("Description")));
    qtable->setHorizontalHeaderItem(2, new QTableWidgetItem(QString("Encrypted pwd")));
    qtable->setHorizontalHeaderItem(3, new QTableWidgetItem(QString("pwd")));

    qtable->setMinimumSize(ui->gridLayoutWidget->size().width(), ui->gridLayoutWidget->size().height());
    Manager *manager = new Manager();

    str_pwd = manager->list(str_key, "pwd.db");
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 4; j++) {
            QTableWidgetItem * newItem = new QTableWidgetItem(QString(str_pwd[i][j].c_str()));
            qtable->setItem(i, j, newItem);
        }
    }
    qtable->resizeColumnsToContents();
    int wTable = qtable->size().width();
    int wGrid = ui->gridLayoutWidget->size().width();
    int dGT = wGrid - wTable;

    int widthforColumn = dGT / 4;
    for(int i = 0; i < 4; i++) {
        qtable->setColumnWidth(i, qtable->columnWidth(i) + widthforColumn);
    }
//    qtable->resizeColumnToContents(2);
//    qtable->resizeColumnToContents(3);
//    qtable->resizeColumnToContents(4);
    qtable->setSelectionMode(QAbstractItemView::SingleSelection);
    qtable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->gridLayoutWidget->update();
    connect(qtable, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(enableEdit(QTableWidgetItem*)));
    connect(qtable, SIGNAL(itemSelectionChanged()), this, SLOT(enableEdit()));
    //connect(ui->pushButton_Search, SIGNAL(clicked()), this, SLOT(search()));
}
void MainWindow::recreateTable() {
    Manager *manager = new Manager();
    str_pwd = manager->list(str_key, "pwd.db");
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 4; j++) {
            QTableWidgetItem * newItem = new QTableWidgetItem(QString(str_pwd[i][j].c_str()));
            qtable->setItem(i, j, newItem);
        }
    }
    search();
    qtable->setMinimumSize(250, 50);
    qtable->resize(750, 200);
    qtable->resizeColumnsToContents();
    qtable->setSelectionMode(QAbstractItemView::SingleSelection);
    qtable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->gridLayoutWidget->update();
}
void MainWindow::enableEdit(QTableWidgetItem* item) {
    enableEdit();
}
void MainWindow::enableEdit() {
    ui->pushButton_Editar->setEnabled(true);
    ui->pushButton_Editar->setStyleSheet("background-color: rgb(206, 92, 0)");
    ui->pushButton_cancel->setEnabled(false);
    ui->pushButton_cancel->setStyleSheet("");
    ui->pushButton_save->setEnabled(false);
    ui->pushButton_save->setStyleSheet("");
}
void MainWindow::on_pushButton_Update_clicked() {
    auto id = ui->spinBox->text();
    auto description = ui->editDescription->text();
    auto password = ui->editPassword->text();
    QString key = ui->editKey->text();

    Pwd *pwd = new Pwd();
    pwd->id = id.toInt(nullptr, 10);
    strcpy(pwd->description, description.toStdString().c_str());
    strcpy(pwd->pwd, password.toStdString().c_str());
    Manager *manager = new Manager();
    manager->writePwd(*pwd, "pwd.db", key.toStdString());
    for (int i = 0, length = qtable->rowCount(); i < length; i++) {
        if(qtable->item(i, 0)->text() == id) {
            break;
        }
    }
    recreateTable();
    setMode(SEARCH);
}
void MainWindow::on_pushButton_Search_clicked() {
    search();
}
void MainWindow::search() {
    auto searchField = ui->lineEdit_search->text();
    int lengthStrPwd = 0;
    string** filtered_str_pwd = search(searchField.toStdString(), &lengthStrPwd);
    for (int i = 0, length = qtable->rowCount(); i < length; i++) {
        qtable->removeRow(0);
    }
    // add itens to the table
    qtable->setRowCount(lengthStrPwd);
    for (int i = 0; i < lengthStrPwd; ++i) {
        for (int j = 0; j < 4; j++) {
            QTableWidgetItem * newItem = new QTableWidgetItem(QString(filtered_str_pwd[i][j].c_str()));
            qtable->setItem(i, j, newItem);
        }
    }
    ui->gridLayoutWidget->update();
}
void MainWindow::on_pushButton_Editar_clicked() {
    auto itens = qtable->selectedItems();
    int size = itens.size();
    if (size > 0) {
        auto item = itens.at(0);
        int index = qtable->row(item);
        auto id = qtable->item(index, 0)->text();
        auto description = qtable->item(index, 1)->text();
        auto pwd = qtable->item(index, 3)->text();
        ui->spinBox->setValue(id.toInt(NULL, 10));
        ui->editDescription->setText(description);
        ui->editPassword->setText(pwd);
        ui->editKey->setText(str_key.c_str());
        setMode(EDIT);
    } else {
        QMessageBox *msg = new QMessageBox(this);
        msg->setText("There isn't any item selected on the table!");
        msg->show();
    }
}
void MainWindow::on_pushButton_New_clicked() {
    enableToInsertNew();
}
void MainWindow::on_pushButton_save_clicked() {
    auto id = ui->spinBox->text();
    auto description = ui->editDescription->text();
    auto password = ui->editPassword->text();
    auto key = ui->editKey->text();

    Pwd *pwd = new Pwd();
    pwd->id = id.toInt(nullptr, 10);
    strcpy(pwd->description, description.toStdString().c_str());
    strcpy(pwd->pwd, password.toStdString().c_str());
    Manager *manager = new Manager();
    manager->writePwd(*pwd, "pwd.db", key.toStdString());
    string str_id = std::to_string(pwd->id);
    qtable->setItem(pwd->id, 0, new  QTableWidgetItem(QString(str_id.c_str())));
    qtable->setItem(pwd->id, 1, new  QTableWidgetItem(QString(pwd->description)));
    qtable->setItem(pwd->id, 2, new  QTableWidgetItem(QString("*******")));
    qtable->setItem(pwd->id, 3, new  QTableWidgetItem(QString(pwd->pwd)));
    // Teste createTable()
    recreateTable();
    setMode(SEARCH);
}
void MainWindow::showMsg() {
    QMessageBox msg(QString("Title"), QString("Body message"),
                    QMessageBox::Warning, QMessageBox::Cancel, QMessageBox::Ok, QMessageBox::No,
                    this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    msg.show();
}
string** MainWindow::search(string str, int* countResult) {
    string** strs = new string*[100];
    bool flag1 = false, flag2 = false;
    int j = 0;
    for(int i = 0; i < 100; i++) {
        QString des = str_pwd[i][1].c_str();
        QString pwd = str_pwd[i][3].c_str();
        flag1 = des.contains(str.c_str(), Qt::CaseInsensitive);
        flag2 = pwd.contains(str.c_str(), Qt::CaseInsensitive);
        if(flag1 || flag2) {
            strs[j] = new string[4];
            strs[j] = str_pwd[i];
            j++;
        }
    }
    *countResult = j;
    return strs;
}

void MainWindow::on_pushButton_cancel_clicked() {
    setMode(SEARCH);
}
/// edit - search - new
void MainWindow::setMode(short mode) {
    QString lineEditEnabled = "color: rgb(238, 238, 236); background-color: rgb(46, 52, 54)"; // Enaled
    QString disabled = "color: rgb(238, 238, 236); background-color: rgb(85, 87, 83)"; // Disabled
    QString btnGreenEnabled = "background-color: green"; // enabled green
    QString btnDisabled = ""; // Disabled green
    QString btnYellowEnabled = "background-color: yellow"; // enabled yellow
    //QString btnOrangeEnabled = "background-color: orange"; // enabled orange
    QString btnBlueEnabled = "background-color: blue"; // enabled blue
    QString btnLightBlueEnabled = "background-color: rgb(154, 209, 255)"; // light blue

    QWidget *widget[] = {
        ui->pushButton_New,
        ui->pushButton_save,
        ui->pushButton_Editar,
        ui->pushButton_Search,
        ui->pushButton_Update,
        ui->pushButton_cancel,

        ui->lineEdit_search,
        ui->spinBox,
        ui->editKey,
        ui->editPassword,
        ui->editDescription
    };
    switch (mode) {
    case  SEARCH: // search mode
        for(auto &e : widget) { // disable all controls
            e->setEnabled(false);
            e->setStyleSheet(disabled);
        }
        // enable only some controls
        ui->lineEdit_search->setEnabled(true);
        ui->lineEdit_search->setStyleSheet(lineEditEnabled);
        ui->pushButton_Search->setEnabled(true);
        ui->pushButton_Search->setStyleSheet(btnLightBlueEnabled);
        ui->pushButton_New->setEnabled(true);
        ui->pushButton_New->setStyleSheet(btnGreenEnabled);
        ui->pushButton_Editar->setEnabled(false);
        ui->pushButton_Editar->setStyleSheet(btnDisabled);
        ui->spinBox->clear();
        ui->editDescription->clear();
        ui->editPassword->clear();
        ui->editKey->clear();
        break;
    case EDIT: // edit mode
        // Disable some controls
        ui->lineEdit_search->setEnabled(false);
        ui->lineEdit_search->setStyleSheet(disabled);
        ui->pushButton_Search->setEnabled(false);
        ui->pushButton_Search->setStyleSheet(btnDisabled);
        ui->pushButton_New->setEnabled(false);
        ui->pushButton_New->setStyleSheet(disabled);

        ui->pushButton_cancel->setEnabled(false);
        ui->pushButton_cancel->setStyleSheet(disabled);
        ui->pushButton_save->setEnabled(false);
        ui->pushButton_save->setStyleSheet(disabled);
        // enable some controls
        ui->spinBox->setEnabled(true);
        ui->spinBox->setStyleSheet(lineEditEnabled);
        ui->editDescription->setEnabled(true);
        ui->editDescription->setStyleSheet(lineEditEnabled);
        ui->editPassword->setEnabled(true);
        ui->editPassword->setStyleSheet(lineEditEnabled);
        ui->editKey->setEnabled(true);
        ui->editKey->setStyleSheet(lineEditEnabled);
        ui->pushButton_cancel->setEnabled(true);
        ui->pushButton_cancel->setStyleSheet(btnYellowEnabled);
        ui->pushButton_Update->setEnabled(true);
        ui->pushButton_Update->setStyleSheet(btnYellowEnabled);
        break;
    case INSERT_NEW: // insert mode
        ui->pushButton_New->setEnabled(false);
        ui->pushButton_New->setStyleSheet(btnDisabled);
        ui->pushButton_Editar->setEnabled(false);
        ui->pushButton_Editar->setStyleSheet(btnDisabled);
        // enable some controls
        ui->spinBox->setEnabled(true);
        ui->spinBox->setStyleSheet(lineEditEnabled);
        ui->editDescription->setEnabled(true);
        ui->editDescription->setStyleSheet(lineEditEnabled);
        ui->editPassword->setEnabled(true);
        ui->editPassword->setStyleSheet(lineEditEnabled);
        ui->editKey->setEnabled(true);
        ui->editKey->setStyleSheet(lineEditEnabled);
        ui->pushButton_cancel->setEnabled(true);
        ui->pushButton_cancel->setStyleSheet(btnYellowEnabled);
        ui->pushButton_save->setEnabled(true);
        ui->pushButton_save->setStyleSheet(btnBlueEnabled);
        break;
    default:
        cerr << "Invalid value for mode at MainWindow::setMode(short mode)";
    }
}
/**
 * search for the first empty index
 */
short MainWindow::searchLastIndex() {
    if(str_pwd[49][1] == "") {
        for(int i = 49; i >= 0; i--) {
            string str = str_pwd[i][1];
            if(str != "") return i +1;
        }
    } else {
        // test next one
        for(int i = 50; i < 100; i++) {
            string str = str_pwd[i][1];
            if(str == "") return i;
        }
    }
    return 0;
}

void MainWindow::on_actionAbout_triggered() {
   QMessageBox::about(this, tr("About Pwd Manager"), QString("Pwd Manager is a software designed to store passwords"
                                                             "\nDeveloped by Leandro Cadete da Silva"));

}
void MainWindow::onActionExitTriggered() {
    QMessageBox msg(QMessageBox::Warning, "Caution", "Are you sure, that you want to close!",
                   QMessageBox::Yes | QMessageBox::No, this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    msg.exec();
    msg.show();
    int r = msg.result();
    if(QMessageBox::Yes == r) {
        exit(0);
    }
    cout << QString(r).toStdString();
}

void MainWindow::on_action_New_triggered() {
    enableToInsertNew();
}

void MainWindow::enableToInsertNew() {
    ui->spinBox->clear();
    short lastIndex = searchLastIndex();
    ui->spinBox->setValue(lastIndex);
    ui->editDescription->clear();
    ui->editPassword->clear();
    ui->editKey->clear();
    setMode(INSERT_NEW);
}
