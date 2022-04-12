QT -= gui

TEMPLATE = lib
DEFINES += IOC_LIBRARY

CONFIG += c++11

SOURCES += \
    $$PWD/dicontainer.cpp \
    $$PWD/idiobj.cpp \
    $$PWD/idiobjbase.cpp

HEADERS += \
    $$PWD/dicontainer.h \
    $$PWD/idiobj.h \
    $$PWD/idiobjbase.h \
    $$PWD/ioc.h \
    $$PWD/ioc_global.h


# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
