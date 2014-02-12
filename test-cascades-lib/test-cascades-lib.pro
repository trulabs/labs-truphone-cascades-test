#-------------------------------------------------
#
# Project created by QtCreator 2013-04-18T15:44:38
#
#-------------------------------------------------

QT       += network
QT       -= gui

INCLUDEPATH += include

TARGET = test-cascades-lib
TEMPLATE = lib

CONFIG += staticlib
CONFIG += qtestlib

DEFINES += TESTCASCADESLIB_LIBRARY

SOURCES += \
    src/CascadesHarness.cpp \
    src/Connection.cpp \
    src/Utils.cpp \
    src/ClickCommand.cpp \
    src/CommandFactory.cpp \
    src/TextCommand.cpp \
    src/SleepCommand.cpp \
    src/TabCommand.cpp \
    src/TestCommand.cpp \
    src/LongClickCommand.cpp \
    src/HelpCommand.cpp \
    src/RecordCommand.cpp \
    src/TapCommand.cpp \
    src/ActionCommand.cpp \
    src/QmlCommand.cpp \
    src/ToastCommand.cpp \
    src/PopCommand.cpp \
    src/ToggleCommand.cpp \
    src/DropDownCommand.cpp \
    src/KeyCommand.cpp \
    src/TouchCommand.cpp \
    src/SpyCommand.cpp \
    src/Server.cpp \
    src/ContactsCommand.cpp \
    src/PageCommand.cpp \
    src/ExitCommand.cpp \
    src/ListCommand.cpp \
    src/SegmentCommand.cpp \
    src/SystemDialogCommand.cpp \
    src/QuitCommand.cpp

HEADERS +=\
    include/CascadesTest.h \
    include/CascadesHarness.h \
    include/Connection.h \
    include/Command.h \
    include/ClickCommand.h \
    include/CommandFactory.h \
    include/Utils.h \
    include/TextCommand.h \
    include/SleepCommand.h \
    include/TabCommand.h \
    include/TestCommand.h \
    include/LongClickCommand.h \
    include/HelpCommand.h \
    include/RecordCommand.h \
    include/TapCommand.h \
    include/RecordCommandTouchHandler.h \
    include/RecordCommandKeyHandler.h \
    include/ActionCommand.h \
    include/QmlCommand.h \
    include/ToastCommand.h \
    include/PopCommand.h \
    include/RecordCommandActionHandler.h \
    include/ToggleCommand.h \
    include/RecordCommandToggleHandler.h \
    include/DropDownCommand.h \
    include/RecordCommandDropDownHandler.h \
    include/KeyCommand.h \
    include/TouchCommand.h \
    include/TouchCommandLPHandler.h \
    include/SpyCommand.h \
    include/Server.h \
    include/ContactsCommand.h \
    include/PageCommand.h \
    include/ExitCommand.h \
    include/ListCommand.h \
    include/SegmentCommand.h \
    include/SystemDialogCommand.h \
    include/QuitCommand.h

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
