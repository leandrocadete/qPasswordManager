#-------------------------------------------------
#
# Project created by QtCreator 2019-01-19T13:01:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = crypt-manager-ui
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    Encrypt.cpp \
    manager.cpp \
    configuration.cpp \
    configdialog.cpp \
    databasefilemodel.cpp \
    customqlistview.cpp

HEADERS += \
    mainwindow.h \
    Encrypt.h \
    manager.h \
    split.h \
    configuration.h \
    configdialog.h \
    databasefilemodel.h \
    customqlistview.h

FORMS += \
        mainwindow.ui \
    configdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

