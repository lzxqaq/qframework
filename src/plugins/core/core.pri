QT += sql gui

TEMPLATE = lib
DEFINES += CORE_LIBRARY

CONFIG += c++11

SOURCES += \

HEADERS += \
    $$PWD/core_global.h \


# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
