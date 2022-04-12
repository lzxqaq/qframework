QT += gui core5compat

TEMPLATE = lib
DEFINES += MVVM_LIBRARY

CONFIG += c++11

SOURCES += \
    $$PWD/connector/connector.cpp \
    $$PWD/models/rowitem.cpp \
    $$PWD/models/rowitemmodel.cpp \
    $$PWD/navigators/mvvmviewcontainer.cpp \
    $$PWD/navigators/navigatorsetting.cpp \
    $$PWD/viewmodels/navigatorviewmodel.cpp \
    $$PWD/viewmodels/viewmodel.cpp \
    $$PWD/views/mvvmview.cpp

HEADERS += \
    $$PWD/appmvvm.h \
    $$PWD/connector/connector.h \
    $$PWD/connector/connectorcontainer.h \
    $$PWD/models/rowitem.h \
    $$PWD/models/rowitemmodel.h \
    $$PWD/mvvm_global.h \
    $$PWD/mvvms.h \
    $$PWD/navigators/mvvmviewcontainer.h \
    $$PWD/navigators/navigator.h \
    $$PWD/navigators/navigatorsetting.h \
    $$PWD/properties/propertybase.h \
    $$PWD/viewmodels/navigatorviewmodel.h \
    $$PWD/viewmodels/viewmodel.h \
    $$PWD/views/mvvmview.h \
    $$PWD/views/mvvmviewbase.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
