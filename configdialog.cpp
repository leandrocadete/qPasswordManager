#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ConfigDialog) {
    ui->setupUi(this);
    config = new Configuration();
    config->readConfig();
    ui->lineEdit_FileName->setText(config->getFileName());

    // TODO: https://doc.qt.io/qt-5/modelview.html
    connect(ui->pushButtonAdd, SIGNAL(clicked()), this, SLOT(addItem()));
    listDbFiles();
}
ConfigDialog::~ConfigDialog(){
    delete ui;
}
void ConfigDialog::on_lineEdit_textChanged(const QString &arg1) {
    std::cout << arg1.toStdString() << std::endl;
}
void ConfigDialog::on_buttonBox_accepted() {


    for(int i = 0; i < model->rowCount(); i++) {
        //qlst << model->data(model->index(i)).toString();
        QString qstr = model->data(model->index(i)).toString();
        if(!QFileInfo::exists(qstr)) {
            // create a file
            Manager::createEmptyFile(qstr.toStdString());
            cout << "File created: " << qstr.toStdString() << endl;
        }
    }

    config->setFileName(ui->lineEdit_FileName->text());
    config->saveConfig();
}
void ConfigDialog::listDbFiles() {
    model = new QStringListModel(this);

    QDir qd(QString(""));
    QStringList mList;
    QFileInfoList list = qd.entryInfoList(QStringList("*.db"));
    for(int i = 0; i < list.size(); i++) {
        mList << list[i].fileName();
    }
    model->setStringList(mList);
    ui->listView->setModel(model);
    connect(ui->listView, &QListView::clicked, this, &ConfigDialog::itemClicked);    
}

void ConfigDialog::itemClicked(const QModelIndex &index) {
    QVariant v_str = model->data(index, Qt::DisplayRole);
    v_str.convert(QVariant::String);
    QString f_str = v_str.toString();
    ui->lineEdit_FileName->setText(f_str);
}
void ConfigDialog::addItem(){
    model->insertRow(model->rowCount());
    QModelIndex mi = model->index(model->rowCount() -1, 0);
    model->setData(mi, QVariant("fileName.db"));
    ui->listView->setCurrentIndex(mi);
    ui->listView->edit(mi);
    if(!this->windowTitle().startsWith("*")) {
        this->setWindowTitle(this->windowTitle().prepend("* "));
    }
}


void ConfigDialog::on_buttonBox_clicked(QAbstractButton *button) {

    //QStringList qlst;
    if(button->text().contains("Save")) {
//        cout << "Will reject" << endl;

//        this->reject();
//        for(int i = 0; i < model->rowCount(); i++) {
//            //qlst << model->data(model->index(i)).toString();
//            QString qstr = model->data(model->index(i)).toString();
//            if(!QFileInfo::exists(qstr)) {
//                // create a file
//                Manager::createEmptyFile(qstr.toStdString());
//                cout << "File created: " << qstr.toStdString() << endl;
//            }
//        }
    }
}
