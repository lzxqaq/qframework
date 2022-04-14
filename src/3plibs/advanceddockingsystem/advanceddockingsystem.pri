ADS_OUT_ROOT = $${OUT_PWD}/..
CONFIG += c++14
CONFIG += debug_and_release
TARGET = $$qtLibraryTarget(qtadvanceddocking)
DEFINES += QT_DEPRECATED_WARNINGS
TEMPLATE = lib
#DESTDIR = $${ADS_OUT_ROOT}/lib
QT += core gui widgets


CONFIG += shared
DEFINES += ADS_SHARED_EXPORT


windows {
        # MinGW
        *-g++* {
                QMAKE_CXXFLAGS += -Wall -Wextra -pedantic
        }
        # MSVC
        *-msvc* {
                QMAKE_CXXFLAGS += /utf-8
        }
}

RESOURCES += src/ads.qrc

HEADERS += \
    src/ads_globals.h \
    src/DockAreaWidget.h \
    src/DockAreaTabBar.h \
    src/DockContainerWidget.h \
    src/DockManager.h \
    src/DockWidget.h \
    src/DockWidgetTab.h \
    src/DockingStateReader.h \
    src/FloatingDockContainer.h \
    src/FloatingDragPreview.h \
    src/DockOverlay.h \
    src/DockSplitter.h \
    src/DockAreaTitleBar_p.h \
    src/DockAreaTitleBar.h \
    src/ElidingLabel.h \
    src/IconProvider.h \
    src/DockComponentsFactory.h  \
    src/DockFocusController.h


SOURCES += \
    src/ads_globals.cpp \
    src/DockAreaWidget.cpp \
    src/DockAreaTabBar.cpp \
    src/DockContainerWidget.cpp \
    src/DockManager.cpp \
    src/DockWidget.cpp \
    src/DockingStateReader.cpp \
    src/DockWidgetTab.cpp \
    src/FloatingDockContainer.cpp \
    src/FloatingDragPreview.cpp \
    src/DockOverlay.cpp \
    src/DockSplitter.cpp \
    src/DockAreaTitleBar.cpp \
    src/ElidingLabel.cpp \
    src/IconProvider.cpp \
    src/DockComponentsFactory.cpp \
    src/DockFocusController.cpp


unix:!macx {
HEADERS += linux/FloatingWidgetTitleBar.h
SOURCES += linux/FloatingWidgetTitleBar.cpp
LIBS += -lxcb
QT += gui-private
}

isEmpty(PREFIX){
        PREFIX=../installed
        warning("Install Prefix not set")
}
#headers.path=$$PREFIX/include
#headers.files=$$HEADERS
#target.path=$$PREFIX/lib
#INSTALLS += headers target

#DISTFILES +=


!isEmpty(target.path): INSTALLS += target
