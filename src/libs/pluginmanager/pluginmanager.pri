QT += sql gui

TEMPLATE = lib
DEFINES += PLUGINMANAGER_LIBRARY

CONFIG += c++11

SOURCES += \
    $$PWD/pluginmanager.cpp \
    $$PWD/pluginmanagerprivate.cpp



HEADERS += \
    $$PWD/plugininterface.h \
    $$PWD/pluginmanager.h \
    $$PWD/pluginmanager_global.h \
    $$PWD/pluginmanagerprivate.h \
    $$PWD/pluginmetadata.h



# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
