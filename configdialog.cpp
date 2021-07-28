#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ConfigDialog) {
    ui->setupUi(this);
    config = new Configuration();
    config->readConfig();
    ui->lineEdit_FileName->setText(config->getFileName());



    // TODO: https://doc.qt.io/qt-5/modelview.html

    //dataBaseModel = new databaseFileModel(this);
    //ui->listView->setModel(dataBaseModel);

//    ui->listView->setModel()

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
