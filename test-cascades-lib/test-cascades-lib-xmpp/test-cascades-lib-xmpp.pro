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
    include/XmppCorrectCommand.h \
    include/XmppTestCommand.h \
    include/XmppPrintCommand.h \
    include/XmppDebugCommand.h \
    include/XmppCarbonsCommand.h

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
    src/XmppCorrectCommand.cpp \
    src/XmppTestCommand.cpp \
    src/XmppPrintCommand.cpp \
    src/XmppDebugCommand.cpp \
    src/XmppCarbonsCommand.cpp

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

TRANSLATIONS = \
    $${TARGET}_en.ts \
    $${TARGET}_fr.ts \
    $${TARGET}_it.ts \
    $${TARGET}_de.ts \
    $${TARGET}_es.ts \
    $${TARGET}_ru.ts \
    $${TARGET}_pl.ts \
    $${TARGET}_pt.ts \
    $${TARGET}_ar.ts \
    $${TARGET}.ts
