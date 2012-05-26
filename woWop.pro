#-------------------------------------------------
#
# Project created by QtCreator 2012-05-26T13:16:41
#
#-------------------------------------------------

QT       += core gui webkit

TARGET = woWop
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    treeitem.cpp \
    treemodel.cpp

HEADERS  += mainwindow.h \
    treeitem.h \
    treemodel.h

FORMS    += mainwindow.ui

LIBS    += -lmongoclient

INCLUDEPATH += /usr/local/include/mongo
