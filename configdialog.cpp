#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ConfigDialog) {
    ui->setupUi(this);
    config = new Configuration();
    config->readConfig();
    ui->lineEdit_FileName->setText(config->getFileName());

    // TODO: https://doc.qt.io/qt-5/modelview.html
    listDbFiles();
}
ConfigDialog::~ConfigDialog(){
    delete ui;
}
void ConfigDialog::on_lineEdit_textChanged(const QString &arg1) {
    std::cout << arg1.toStdString() << std::endl;
}
void ConfigDialog::on_buttonBox_accepted() {
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
