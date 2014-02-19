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
    include/XmppConnectCommand.h \
    include/XmppResourceStore.h \
    include/XmppPresenceCommand.h \
    include/XmppMessageCommand.h \
    include/XmppDisconnectCommand.h \
    include/XmppChatStateCommand.h \
    include/XmppChatMarkerCommand.h \
    include/XmppReachabilityCommand.h \
    include/XmppCorrectCommand.h

SOURCES += \
    src/XmppHarness.cpp \
    src/XmppHelpCommand.cpp \
    src/XmppConnectCommand.cpp \
    src/XmppResourceStore.cpp \
    src/XmppPresenceCommand.cpp \
    src/XmppMessageCommand.cpp \
    src/XmppDisconnectCommand.cpp \
    src/XmppChatStateCommand.cpp \
    src/XmppChatMarkerCommand.cpp \
    src/XmppReachabilityCommand.cpp \
    src/XmppCorrectCommand.cpp

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
