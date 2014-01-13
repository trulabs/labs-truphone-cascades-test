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

INCLUDEPATH += include \
    ../test-cascades-lib/include

TEMPLATE = app


SOURCES += src/main.cpp \
    src/cli.cpp

HEADERS += \
    include/cli.h
