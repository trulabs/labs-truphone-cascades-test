#-------------------------------------------------
#
# Project created by QtCreator 2013-04-25T11:01:04
#
#-------------------------------------------------

QT       += core

QT       -= gui

QT       += network

TARGET = test-cascades-cli
CONFIG   += console
CONFIG   -= app_bundle
DEFINES += BUFFER_ON_HOST

INCLUDEPATH += include \
    ../test-cascades-lib/

TEMPLATE = app


SOURCES += src/main.cpp \
    src/cli.cpp \
    ../test-cascades-lib/src/Buffer.cpp

HEADERS += \
    include/cli.h \
    ../test-cascades-lib/include/Buffer.h
