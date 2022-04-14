QT += sql gui

TEMPLATE = lib
DEFINES += PLUGINMANAGER_LIBRARY

CONFIG += c++11

SOURCES += \
    $$PWD/iplugin.cpp \
    $$PWD/pluginmanager.cpp \
    $$PWD/pluginmanagerprivate.cpp



HEADERS += \
    $$PWD/iplugin.h \
    $$PWD/iplugin_p.h \
    $$PWD/pluginmanager.h \
    $$PWD/pluginmanager_global.h \
    $$PWD/pluginmanagerprivate.h



# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
