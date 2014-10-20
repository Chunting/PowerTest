#-------------------------------------------------
#
# Project created by QtCreator 2014-01-18T14:11:24
#
#-------------------------------------------------

QT       += core gui
QT       += sql xml
QT       += printsupport
QT       += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PowerTest
TEMPLATE = app
CONFIG += console

SOURCES += main.cpp\
        mainwindow.cpp \
    flashitem.cpp \
    startitem.cpp \
    pixitem.cpp \
    systemwidget.cpp \
    busitem.cpp \
    lineitem.cpp \
    genitem.cpp \
    logindialog.cpp \
    pieview.cpp \
    readdata.cpp \
    showwidget.cpp \
    palette.cpp \
    system118_widget.cpp \
    newgenitem.cpp \
    loaditem.cpp \
    setdialog.cpp

HEADERS  += mainwindow.h \
    flashitem.h \
    startitem.h \
    pixitem.h \
    systemwidget.h \
    busitem.h \
    lineitem.h \
    genitem.h \
    logindialog.h \
    ui_logindialog.h \
    connection.h \
    pieview.h \
    ui_widget.h \
    readdata.h \
    showwidget.h \
    palette.h \
    system118_widget.h \
    newgenitem.h \
    loaditem.h \
    setdialog.h \
    ui_setdialog.h

FORMS    +=

RESOURCES += \
    images.qrc \
    data.qrc
