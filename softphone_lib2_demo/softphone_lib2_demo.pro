#-------------------------------------------------
#
# Project created by QtCreator 2015-12-28T17:24:41
#
#-------------------------------------------------

QT       += core gui  network xml multimedia qml quick
CONFIG -= app_bundle
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = softphone_lib2_demo
TEMPLATE = app
include(loogearphoneimpl/loogearphoneimpl.pri)
 CONFIG += c++11
SOURCES += main.cpp\
        widget.cpp \
    videosurfacehandle.cpp

HEADERS  += widget.h \
    videosurfacehandle.h

FORMS    += widget.ui
macx{
    QMAKE_CXXFLAGS += -ObjC++
 CONFIG += c++11
 LIBS +=-lstdc++  -framework Foundation

QMAKE_CXXFLAGS +=  -Wc++11-extensions -Wc++11-long-long
DEFINES += __STDC_CONSTANT_MACROS
}
#include(install.pri)

# RESOURCES += \
#     demoqml/qml.qrc
