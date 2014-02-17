#-------------------------------------------------
#
# Project created by QtCreator 2013-04-18T15:44:38
#
#-------------------------------------------------

INCLUDEPATH += include

TARGET = test-cascades-lib
TEMPLATE = subdirs

CONFIG += ordered
SUBDIRS += test-cascades-lib-core
SUBDIRS += ../externals/qxmpp/src

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
