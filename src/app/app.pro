include(../../qtproject.pri)

include(../libs/qsimpleupdater/qsimpleupdater-include.pri)
include(../libs/singleapplication/singleapplication-include.pri)

include(../libs/extensionsystem/extensionsystem-include.pri)
TEMPLATE = app
TARGET = qtproject
DESTDIR = $$IDE_APP_PATH
QT       += core gui

SHARED_PATH = $$PWD/../../shared/

SOURCES += \
    main.cpp

RESOURCES += \
    app.qrc

HEADERS += \

LIBS *= \
    -l$$qtLibraryName(QSimpleUpdater) \
    -l$$qtLibraryName(SingleApplication) \
    -l$$qtLibraryName(ExtensionSystem)

target.path = $$INSTALL_APP_PATH
INSTALLS += target

include(../rpath.pri)

