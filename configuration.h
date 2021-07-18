#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <iostream>
#include <QSettings>
#include <QString>
using namespace std;
class Configuration {
    QString fileName;
    QSettings *settings;
public:
    Configuration();
    Configuration(QString file);
    QString getFileName() { return fileName; }
    void setFileName(QString _fileName) { fileName = _fileName; }

    void readConfig();
    void saveConfig();
};

#endif // CONFIGURATION_H
