#-------------------------------------------------
#
# Project created by QtCreator 2017-06-29T08:34:17
#
#-------------------------------------------------

QT       += core gui

TARGET = mycom
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    win_qextserialport.cpp \
    qextserialport.cpp \
    qextserialbase.cpp \
    qextserialenumerator.cpp \
    savedialog.cpp

HEADERS  += mainwindow.h \
    win_qextserialport.h \
    qextserialport.h \
    qextserialbase.h \
    qextserialenumerator.h \
    savedialog.h

FORMS    += mainwindow.ui \
    savedialog.ui
