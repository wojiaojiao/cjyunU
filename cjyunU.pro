#-------------------------------------------------
#
# Project created by QtCreator 2015-01-18T20:29:48
#
#-------------------------------------------------

QT       += core gui
QT       += webkitwidgets network widgets
QT       += script





greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cjyunU
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    wangpan.cpp \
    networkcookie.cpp

HEADERS  += widget.h \
    wangpan.h \
    networkcookie.h

FORMS    += widget.ui \
    wangpan.ui

RC_FILE += "myico.rc"
