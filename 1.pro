#-------------------------------------------------
#
# Project created by QtCreator 2014-08-10T05:26:08
#
#-------------------------------------------------

QT += core gui
QT += network
QT += webkitwidgets
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
RC_FILE = favicon.rc
TARGET = 1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    login.cpp

HEADERS  += mainwindow.h \
    login.h

FORMS    += \
    login.ui
