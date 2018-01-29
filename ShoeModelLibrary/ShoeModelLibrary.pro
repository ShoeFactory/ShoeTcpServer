#-------------------------------------------------
#
# Project created by QtCreator 2017-07-10T17:19:03
#
#-------------------------------------------------
include( $$PWD/../ShoeTcpServer.pri )

QT       -= gui

TEMPLATE = lib
CONFIG += shared

msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8

DEFINES += SHOEMODELLIBRARY_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    messagelogin.cpp \
    messagegps.cpp

HEADERS += \
        shoemodellibrary_global.h \ 
    messagelogin.h \
    messagegps.h


CONFIG(debug, debug|release) {
    TARGET = ShoeModelLibraryd
} else {
    TARGET = ShoeModelLibrary
}

