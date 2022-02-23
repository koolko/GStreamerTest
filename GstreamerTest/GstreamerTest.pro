#-------------------------------------------------
#
# Project created by QtCreator 2017-11-24T13:41:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GstreamerTest
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

INCLUDEPATH += /usr/include/gstreamer-1.0 \
               /usr/include/glib-2.0 \
               /usr/lib/i386-linux-gnu/glib-2.0/include \
               /usr/lib/x86_64-linux-gnu/glib-2.0/include \
               /usr/lib/x86_64-linux-gnu/gstreamer-1.0/include
               #/usr/lib/i386-linux-gnu/glib-2.0/include
               #/usr/include/libxml2

LIBS += -L/usr/lib -lgstreamer-1.0 -lgstvideo-1.0

LIBS += -L/usr/lib/i386-linux-gnu -lgobject-2.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    pipelinecontainer.cpp \
    mediaflow.cpp

HEADERS += \
        mainwindow.h \
    pipelinecontainer.h \
    mediaflow.h

FORMS += \
        mainwindow.ui
