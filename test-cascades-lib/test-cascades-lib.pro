INCLUDEPATH += include

TARGET = test-cascades-lib
TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += test-cascades-lib-core
SUBDIRS += test-cascades-lib-xmpp

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
    ../Doxyfile \
    ../Makefile
