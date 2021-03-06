#-------------------------------------------------
#
# Project created by QtCreator 2017-07-09T13:05:53
#
#-------------------------------------------------
include( $$PWD/../ShoeTcpServer.pri )

QT       += network

QT       -= gui

TEMPLATE = lib
CONFIG += shared

msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8

DEFINES += SHOECOMMLIBRARY_LIBRARY

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
    shoetcpserver.cpp \
    shoetcpclient.cpp \
    shoetcpsocket.cpp \
    shoecommdefination.cpp

HEADERS += \
        shoecommlibrary_global.h \ 
    shoecommdefination.h \
    shoetcpserver.h \
    shoetcpclient.h \
    shoetcpsocket.h


CONFIG(debug, debug|release) {
    LIBS += -lShoeUtilsLibraryd
    TARGET = ShoeCommLibraryd
} else {
    LIBS += -lShoeUtilsLibrary
    TARGET = ShoeCommLibrary
}


