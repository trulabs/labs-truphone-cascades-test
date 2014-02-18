#-------------------------------------------------
#
# Project created by QtCreator 2013-04-18T15:44:38
#
#-------------------------------------------------
QT       += network xml
QT       -= gui

INCLUDEPATH += include
INCLUDEPATH += ../test-cascades-lib-core/include
INCLUDEPATH += ../../externals/qxmpp/src/base
INCLUDEPATH += ../../externals/qxmpp/src/client

TARGET = test-cascades-lib-xmpp
TEMPLATE = lib

CONFIG += staticlib

DEFINES += TESTCASCADESLIB_LIBRARY

HEADERS +=\
    include/XmppHarness.h \
    include/XmppHelpCommand.h \
    include/XmppConnectCommand.h

SOURCES += \
    src/XmppHarness.cpp \
    src/XmppHelpCommand.cpp \
    src/XmppConnectCommand.cpp

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
