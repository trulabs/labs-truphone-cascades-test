#-------------------------------------------------
#
# Project created by QtCreator 2013-04-18T15:44:38
#
#-------------------------------------------------

QT       += network
QT       -= gui

INCLUDEPATH += include
INCLUDEPATH += ../test-cascades-lib-core/include
INCLUDEPATH += ../../externals/qxmpp/src/base
INCLUDEPATH += ../../externals/qxmpp/src/client

TARGET = test-cascades-lib-xmpp
TEMPLATE = lib

CONFIG += staticlib

DEFINES += TESTCASCADESLIB_LIBRARY

SOURCES += \
    src/XmppHarness.cpp

HEADERS +=\
    include/XmppHarness.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES += \
    ../README.md \
    ../Doxyfile
