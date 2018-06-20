#-------------------------------------------------
#
# Project created by QtCreator 2018-06-15T23:04:40
#
#-------------------------------------------------

QT       += core gui sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KMCSAssistant
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    rolemanawindow.cpp \
    roleformwindow.cpp \
    MyGlobalShortCut/MyGlobalShortCut.cpp \
    MyGlobalShortCut/MyWinEventFilter.cpp

HEADERS  += mainwindow.h \
    rolemanawindow.h \
    roleformwindow.h \
    MyGlobalShortCut/MyGlobalShortCut.h \
    MyGlobalShortCut/MyWinEventFilter.h

FORMS    += mainwindow.ui \
    rolemanawindow.ui \
    roleformwindow.ui

DISTFILES += \
    app.rc

RESOURCES += \
    res.qrc

TRANSLATIONS = KMCSAssistant_zh_CN.ts

RC_FILE = app.rc
