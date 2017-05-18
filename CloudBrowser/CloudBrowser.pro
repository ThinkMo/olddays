#-------------------------------------------------
#
# Project created by QtCreator 2013-04-20T14:03:02
#
#-------------------------------------------------

QT       += core gui
QT      += webkit
QT      += network
TARGET = CloudBrowser
TEMPLATE = app

RC_FILE = cbrowser.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    webview.cpp \
    obj_openvlc.cpp \
    titlewidget.cpp \
    Log.cpp

HEADERS  += mainwindow.h \
    webview.h \
    obj_openvlc.h \
    titlewidget.h \
    Log.h

RESOURCES += \
    file.qrc

OTHER_FILES += \
    cbrowser.rc

