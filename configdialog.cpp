#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ConfigDialog) {
    ui->setupUi(this);
    config = new Configuration();
    config->readConfig();
    ui->lineEdit_FileName->setText(config->getFileName());

    // TODO: https://doc.qt.io/qt-5/modelview.html
    connect(ui->pushButtonAdd, SIGNAL(clicked()), this, SLOT(addItem()));
    listView = new CustomQListView(this);
    listView->setGeometry(30, 85, 471, 125);
    listDbFiles();

}
ConfigDialog::~ConfigDialog(){
    delete ui;
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
    listView->setModel(model);

    connect(listView, SIGNAL(customChanged(int)), this, SLOT(itemClicked(int)));
}

void ConfigDialog::itemClicked(const QModelIndex &index) {
    QVariant v_str = model->data(index, Qt::DisplayRole);
    v_str.convert(QVariant::String);
    QString f_str = v_str.toString();
    ui->lineEdit_FileName->setText(f_str);
    if(!this->windowTitle().startsWith("*")) {
        this->setWindowTitle(this->windowTitle().prepend("* "));
    }
}
void ConfigDialog::itemClicked(int n) {
    cout << "Clicked("<< n <<")" << endl;
    itemClicked(listView->model()->index(n, 0));
}

void ConfigDialog::addItem(){
    model->insertRow(model->rowCount());
    QModelIndex mi = model->index(model->rowCount() -1, 0);
    model->setData(mi, QVariant("fileName.db"));
    listView->setCurrentIndex(mi);
    listView->edit(mi);
    if(!this->windowTitle().startsWith("*")) {
        this->setWindowTitle(this->windowTitle().prepend("* "));
    }
}


void ConfigDialog::on_buttonBox_clicked(QAbstractButton *button) {
    if(button->text().contains("Save")) {
    }
}
