#include "configuration.h"

Configuration::Configuration() {
    settings = new QSettings("config-qpassword-manager.ini", QSettings::IniFormat);
}
Configuration::Configuration(QString file) {
    settings = new QSettings(file, QSettings::IniFormat);
}
void Configuration::readConfig() {
    fileName = settings->value("pwd-file").toString();

    cout << "filename: " << fileName.toStdString() << endl;
    if(fileName == "") fileName = "pwd.db"; // set a default value

}
void Configuration::saveConfig() {
    settings->setValue("pwd-file", fileName);
}
