QT += sql gui

TEMPLATE = lib
DEFINES += ORMDEMO_LIBRARY

CONFIG += c++11

SOURCES += \



HEADERS += \
    $$PWD/ormdemo_global.h \




# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
