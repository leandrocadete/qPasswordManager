#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ConfigDialog) {
    ui->setupUi(this);
    config = new Configuration();
    config->readConfig();
    ui->lineEdit_FileName->setText(config->getFileName());

    msgDelete = new QMessageBox(QMessageBox::Warning,
                    tr("Caution!"),
                    tr("Are you sure you want to delete the selected database file?"),
                    QMessageBox::No | QMessageBox::Yes,
                    this,

                    Qt::MSWindowsFixedSizeDialogHint);
    msgDelete->setModal(true);
    connect(ui->pushButtonAdd, SIGNAL(clicked()), this, SLOT(addItem()));
    connect(ui->pushButtonDelete, SIGNAL(clicked()), this, SLOT(deleteItem()));
    listView = new CustomQListView(this);
    listView->setGeometry(30, 85, 471, 125);
    setTabOrder(ui->lineEdit_FileName, listView);
    listDbFiles();
    selectedIndex = -1;
}
ConfigDialog::~ConfigDialog(){
    delete msgDelete;
    delete ui;
}
void ConfigDialog::on_buttonBox_accepted() {
    for(int i = 0; i < model->rowCount(); i++) {
        QString qstr = model->data(model->index(i)).toString();
        if(!QFileInfo::exists(qstr)) {
            // create a file
            Manager::createEmptyFile(qstr.toStdString());
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
    QString f_str = v_str.toString();
    ui->lineEdit_FileName->setText(f_str);
    if(!this->windowTitle().startsWith("*")) {
        this->setWindowTitle(this->windowTitle().prepend("* "));
    }
    selectedIndex = index.row();
}
void ConfigDialog::itemClicked(int n) {
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

void ConfigDialog::deleteItem() {
    int r = msgDelete->exec();
    if(r == QMessageBox::Yes) {
        if(selectedIndex >= 0) {
            QVariant qv = model->data(listView->model()->index(selectedIndex, 0));
            QString fname = qv.toString();
            if(remove(fname.toStdString().c_str()) != 0){
                cerr << "Error deleting file: " << fname.toStdString().c_str() << endl;
            } else {
                model->removeRow(selectedIndex);
            }
        }
    }
}

