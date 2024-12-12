#-------------------------------------------------
#
# Project created by QtCreator 2015-12-28T17:24:41
#
#-------------------------------------------------

QT       += core gui  network xml multimedia qml quick
QT += multimediawidgets
!macx{
CONFIG -= app_bundle
}
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = softphone_lib2222_demo
TEMPLATE = app
include(loogearphoneimpl/loogearphoneimpl.pri)
 CONFIG += c++11
SOURCES += main.cpp\
    videorenderinstance.cpp \
    videowidget.cpp \
        widget.cpp \
    videosurfacehandle.cpp \
    videorender.cpp

HEADERS  += widget.h \
    videorenderinstance.h \
    videosurfacehandle.h \
    videorender.h \
    videowidget.h

FORMS    += widget.ui \
    videowidget.ui
macx{
QMAKE_CXXFLAGS += -ObjC++
 CONFIG += c++11
 LIBS +=-lstdc++  -framework Foundation
SOURCES -=widget.cpp
OBJECTIVE_SOURCES += widget.mm
QMAKE_CXXFLAGS +=  -Wc++11-extensions -Wc++11-long-long
DEFINES += __STDC_CONSTANT_MACROS

    # ICON = $$PWD/res/macx/SparkleComm.icns
    QMAKE_INFO_PLIST =  $$PWD/res/macx/Info.plist
    OTHER_FILES += \
         $$PWD/res/macx/Info.plist

message("Current ICON:"$$ICON)

QMAKE_TARGET_BUNDLE_PREFIX = com.loogear
QMAKE_BUNDLE = softphone_lib2222_demo
}
QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
QMAKE_LFLAGS_CONSOLE = /SUBSYSTEM:CONSOLE,5.01
#include(install.pri)

# RESOURCES += \
#     demoqml/qml.qrc
