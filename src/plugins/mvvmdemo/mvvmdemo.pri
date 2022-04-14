QT += sql gui

TEMPLATE = lib
DEFINES += MVVMDEMO_LIBRARY

CONFIG += c++11

SOURCES += \

HEADERS += \
    $$PWD/mvvmdemo_global.h \




# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
