#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    init();
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(onActionExitTriggered()));
    connect(ui->pushButton_Delete, SIGNAL(pressed()), this, SLOT(on_pushButton_Delete_clicked()));    
}
void MainWindow::init() {
    config = new Configuration();
    config->readConfig();
    strPwdFile = config->getFileName().toStdString();

    bool ok = false;
    string title = "Key for file: " + strPwdFile;
    string tmpKey = QInputDialog::getText(this, title.c_str(),
        tr("Default key to decript passwords (eg. mykey@#@#, #$%keysecret)"
            "\nIf one incorrect key is set you wont see the correct password decription"
            "\nPlease do not forget this key!"), QLineEdit::Password, "", &ok).toStdString();
    if(ok) {
        str_key = tmpKey;
    }
    if(!ok && str_key.size() == 0){
        exit(-1);
    }
    if(!flagInit) {
        createTable();
        flagInit = true;
    } else {
        recreateTable();
    }
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
    Manager *manager = new Manager(config->getFileName().toStdString());

    str_pwd = manager->list(str_key);
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 4; j++) {
            QTableWidgetItem * newItem = new QTableWidgetItem(QString(str_pwd[i][j].c_str()));
            qtable->setItem(i, j, newItem);
        }
    }
    qtable->resizeColumnsToContents();
    int countColumn = qtable->columnCount();
    int table_with = 0;
    for(int i = 0; i < countColumn; i++) {
        table_with += qtable->columnWidth(i);
    }
    int wGrid = table_with;
    int dGT = ui->horizontalLayoutWidget->width() - wGrid;

    int widthforColumn = dGT / 4;
    for(int i = 0; i < 4; i++) {
        qtable->setColumnWidth(i, qtable->columnWidth(i) + widthforColumn);
    }
    qtable->setSelectionMode(QAbstractItemView::SingleSelection);
    qtable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->gridLayoutWidget->update();


    connect(qtable, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(widgetItemClicked(QTableWidgetItem*)));
    connect(qtable, SIGNAL(itemSelectionChanged()), this, SLOT(widgetItemClicked()));
}
void MainWindow::recreateTable() {
    int scrollValue = qtable->verticalScrollBar()->value();

    Manager *manager = new Manager(config->getFileName().toStdString());
    str_pwd = manager->list(str_key);
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 4; j++) {
            QTableWidgetItem * newItem = new QTableWidgetItem(QString(str_pwd[i][j].c_str()));
            qtable->setItem(i, j, newItem);
        }
    }
    search();
    int countColumn = qtable->columnCount();
    int table_with = 0;
    for(int i = 0; i < countColumn; i++) {
        table_with += qtable->columnWidth(i);
    }
    int wGrid = table_with;
    int dGT = ui->horizontalLayoutWidget->width() - wGrid;

    int widthforColumn = dGT / 4;
    for(int i = 0; i < 4; i++) {
        qtable->setColumnWidth(i, qtable->columnWidth(i) + widthforColumn);
    }
    qtable->verticalScrollBar()->setValue(scrollValue);
    qtable->setSelectionMode(QAbstractItemView::SingleSelection);
    qtable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->gridLayoutWidget->update();
}
void MainWindow::widgetItemClicked(QTableWidgetItem* item) {
    widgetItemClicked();
}
void MainWindow::widgetItemClicked() {
    QString qstrV(ui->editKey->text());
    if(qstrV != QString("")) {
       clearInputs();
    }
    setMode(windowMode::Select);
}
void MainWindow::clearInputs() {
    ui->spinBox->clear();
    ui->editDescription->clear();
    ui->editPassword->clear();
    ui->editKey->clear();
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
    Manager *manager = new Manager(config->getFileName().toStdString());
    manager->writePwd(*pwd, key.toStdString());
    for (int i = 0, length = qtable->rowCount(); i < length; i++) {
        if(qtable->item(i, 0)->text() == id) {
            break;
        }
    }
    recreateTable();
    setMode(windowMode::Search);
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
    setMode(windowMode::Search);
    ui->lineEdit_search->setFocus();
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
        setMode(windowMode::Edit);
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
    Manager *manager = new Manager(config->getFileName().toStdString());
    manager->writePwd(*pwd, key.toStdString());
    string str_id = std::to_string(pwd->id);
    qtable->setItem(pwd->id, 0, new  QTableWidgetItem(QString(str_id.c_str())));
    qtable->setItem(pwd->id, 1, new  QTableWidgetItem(QString(pwd->description)));
    qtable->setItem(pwd->id, 2, new  QTableWidgetItem(QString("*******")));
    qtable->setItem(pwd->id, 3, new  QTableWidgetItem(QString(pwd->pwd)));
    // Teste createTable()
    recreateTable();
    setMode(windowMode::Search);
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
    setMode(windowMode::Search);
}
/// edit - search - new
void MainWindow::setMode(windowMode mode) {
    QWidget *widget[] = {
        ui->pushButton_New,
        ui->pushButton_save,
        ui->pushButton_Editar,
        ui->pushButton_Search,
        ui->pushButton_Update,
        ui->pushButton_cancel,
        ui->pushButton_Delete,
        ui->lineEdit_search,
        ui->spinBox,
        ui->editKey,
        ui->editPassword,
        ui->editDescription
    };
    switch (mode) {
    case  windowMode::Search: // search mode
        for(auto &e : widget) { // disable all controls
            e->setEnabled(false);
            e->setStyleSheet(styleDisabled);
        }
        // enable only some controls
        ui->lineEdit_search->setEnabled(true);
        ui->lineEdit_search->setStyleSheet(styleLineEditEnabled);
        ui->pushButton_Search->setEnabled(true);
        ui->pushButton_Search->setStyleSheet(styleBtnLightBlueEnabled);
        ui->pushButton_New->setEnabled(true);
        ui->pushButton_New->setStyleSheet(styleBtnGreenEnabled);
        ui->pushButton_Editar->setEnabled(false);

        ui->action_New->setDisabled(false);
        ui->action_Delete->setDisabled(true);

        clearInputs();

        break;
    case windowMode::Select:
        for(auto &e : widget) { // disable all controls
            e->setEnabled(false);
            e->setStyleSheet(styleDisabled);
        }
        ui->pushButton_Editar->setEnabled(true);
        ui->pushButton_Editar->setStyleSheet(styleBtnEditarEnbled);
        ui->pushButton_Delete->setEnabled(true);
        ui->pushButton_Delete->setStyleSheet(styleBtnRedEnabled);
        ui->action_Delete->setEnabled(true);
        ui->pushButton_Search->setEnabled(true);
        ui->pushButton_Search->setStyleSheet(styleBtnLightBlueEnabled);
        ui->pushButton_New->setEnabled(true);
        ui->pushButton_New->setStyleSheet(styleBtnGreenEnabled);
        ui->action_New->setEnabled(true);
        ui->lineEdit_search->setEnabled(true);
        ui->lineEdit_search->setStyleSheet(styleLineEditEnabled);

        break;
    case windowMode::Edit: // edit mode
        // Disable some controls
        ui->lineEdit_search->setEnabled(false);
        ui->lineEdit_search->setStyleSheet(styleDisabled);
        ui->pushButton_Search->setEnabled(false);
        ui->pushButton_Search->setStyleSheet(styleBtnDisabled);
        ui->pushButton_New->setEnabled(false);
        ui->pushButton_New->setStyleSheet(styleDisabled);
        ui->action_New->setEnabled(false);
        ui->action_New->setDisabled(true);
        // enable some controls
        ui->pushButton_Editar->setEnabled(true);
        ui->pushButton_Editar->setStyleSheet(styleBtnEditarEnbled);
        ui->pushButton_Delete->setEnabled(true); // delete
        ui->pushButton_Delete->setStyleSheet(styleBtnRedEnabled); // delete
        ui->action_Delete->setEnabled(true); // delete
        ui->action_Delete->setDisabled(false); // delete
        ui->pushButton_cancel->setEnabled(false);
        ui->pushButton_cancel->setStyleSheet(styleDisabled);
        ui->pushButton_save->setEnabled(false);
        ui->pushButton_save->setStyleSheet(styleDisabled);


        ui->spinBox->setEnabled(true);
        ui->spinBox->setStyleSheet(styleLineEditEnabled);
        ui->editDescription->setEnabled(true);
        ui->editDescription->setStyleSheet(styleLineEditEnabled);
        ui->editPassword->setEnabled(true);
        ui->editPassword->setStyleSheet(styleLineEditEnabled);
        ui->editKey->setEnabled(true);
        ui->editKey->setStyleSheet(styleLineEditEnabled);
        ui->pushButton_cancel->setEnabled(true);
        ui->pushButton_cancel->setStyleSheet(styleBtnYellowEnabled);
        ui->pushButton_Update->setEnabled(true);
        ui->pushButton_Update->setStyleSheet(styleBtnYellowEnabled);
        break;
    case windowMode::Insert: // insert mode
        ui->pushButton_Delete->setEnabled(false); // delete
        ui->pushButton_Delete->setStyleSheet(styleBtnDisabled); // delete
        ui->pushButton_New->setEnabled(false);
        ui->pushButton_New->setStyleSheet(styleBtnDisabled);
        ui->pushButton_Editar->setEnabled(false);
        ui->pushButton_Editar->setStyleSheet(styleBtnDisabled);
        ui->action_New->setEnabled(false);
        ui->action_New->setDisabled(true);
        ui->action_Delete->setEnabled(false); // delete
        ui->action_Delete->setDisabled(true); // delete

        // enable some controls
        ui->spinBox->setEnabled(true);
        ui->spinBox->setStyleSheet(styleLineEditEnabled);
        ui->editDescription->setEnabled(true);
        ui->editDescription->setStyleSheet(styleLineEditEnabled);
        ui->editPassword->setEnabled(true);
        ui->editPassword->setStyleSheet(styleLineEditEnabled);
        ui->editKey->setEnabled(true);
        ui->editKey->setStyleSheet(styleLineEditEnabled);
        ui->pushButton_cancel->setEnabled(true);
        ui->pushButton_cancel->setStyleSheet(styleBtnYellowEnabled);
        ui->pushButton_save->setEnabled(true);
        ui->pushButton_save->setStyleSheet(sytleBtnBlueEnabled);
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
                                                             "\nDeveloped by Leandro Cadete da Silva"
                                                             "\nRepository: https://github.com/leandrocadete/qPasswordManager"));
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
    ui->editKey->setText(str_key.c_str());
    setMode(windowMode::Insert);
}
void MainWindow::on_pushButton_Delete_clicked(){
    QMessageBox msg(QMessageBox::Warning, "Caution", "Are you sure you want to delete the selected item!",
                   QMessageBox::Yes | QMessageBox::No, this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    msg.exec();
    msg.show();
    int r = msg.result();
    if(QMessageBox::Yes == r) {

        auto itens = qtable->selectedItems();
        QString idSelected = itens[0]->text();
        int size = itens.size();
        if(size > 0) {
            Pwd *pwd = new Pwd();
            pwd->id = idSelected.toInt(nullptr, 10);
            strcpy(pwd->description, "");
            strcpy(pwd->pwd, "");
            Manager *manager = new Manager(config->getFileName().toStdString());
            manager->writePwd(*pwd, str_key);
            for (int i = 0, length = qtable->rowCount(); i < length; i++) {
                if(qtable->item(i, 0)->text() == idSelected) {
                    break;
                }
            }
            recreateTable();
            setMode(windowMode::Search);
        }
    }
}
void MainWindow::on_action_Delete_triggered() {
    on_pushButton_Delete_clicked();
}
void MainWindow::on_action_Current_key_triggered() {
    init();
}
void MainWindow::on_actionConfiguration_triggered(){
    configDialog = new ConfigDialog(this);
    configDialog->setModal(true);
    configDialog->exec();
    auto r = configDialog->result();
    if(QDialog::Accepted == r) {        
        init();
    }
}
