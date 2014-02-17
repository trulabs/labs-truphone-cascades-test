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

# QXMPP settings
DEFINES += QXMPP_BUILD QXMPP_STATIC

HEADERS +=\
    $$files(../../externals/qxmpp/src/base/*.h) \
    $$files(../../externals/qxmpp/src/client/*.h) \
    include/XmppHarness.h

SOURCES += \
    $$files(../../externals/qxmpp/src/base/Q*.cpp) \
    $$files(../../externals/qxmpp/src/client/*.cpp) \
    src/XmppHarness.cpp

# DNS
qt_version = $$QT_MAJOR_VERSION
contains(qt_version, 4) {
    INSTALL_HEADERS += base/qdnslookup.h base/qdnslookup_p.h
    SOURCES += ../../externals/qxmpp/src/base/qdnslookup.cpp
    android:SOURCES += ../../externals/qxmpp/src/base/qdnslookup_stub.cpp
    else:symbian:SOURCES += ../../externals/qxmpp/src/base/qdnslookup_symbian.cpp
    else:unix:SOURCES += ../../externals/qxmpp/src/base/qdnslookup_unix.cpp
    else:win32:SOURCES += ../../externals/qxmpp/src/base/qdnslookup_win.cpp
    else:SOURCES += ../../externals/qxmpp/src/base/qdnslookup_stub.cpp
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
