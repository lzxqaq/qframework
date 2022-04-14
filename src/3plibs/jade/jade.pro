TEMPLATE = app

TARGET = jade
DESTDIR = bin

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += release embed_manifest_dll c++11
CONFIG -= debug

DEFINES += QT_DEPRECATED_WARNINGS

# QuaZIP
#INCLUDEPATH += C:/Development/quazip-0.7.3/quazip $$[QT_INSTALL_HEADERS]/QtZlib
#LIBS += -LC:/Development/quazip-0.7.3/quazip/release/ -lquazip

INCLUDEPATH += source source/drawing source/items source/properties

SOURCES += \
	source/drawing/DrawingArrow.cpp \
	source/drawing/DrawingCanvas.cpp \
	source/drawing/DrawingCurveItem.cpp \
	source/drawing/DrawingEllipseItem.cpp \
	source/drawing/DrawingItem.cpp \
	source/drawing/DrawingItemFactory.cpp \
	source/drawing/DrawingItemGroup.cpp \
	source/drawing/DrawingItemPoint.cpp \
	source/drawing/DrawingLineItem.cpp \
	source/drawing/DrawingPathItem.cpp \
	source/drawing/DrawingPolygonItem.cpp \
	source/drawing/DrawingPolylineItem.cpp \
	source/drawing/DrawingRectItem.cpp \
	source/drawing/DrawingTextEllipseItem.cpp \
	source/drawing/DrawingTextItem.cpp \
	source/drawing/DrawingTextRectItem.cpp \
	source/drawing/DrawingTypes.cpp \
	source/drawing/DrawingUndo.cpp \
	source/drawing/DrawingWidget.cpp \
	source/items/ElectricItems.cpp \
	source/items/LogicItems.cpp \
	source/properties/HelperWidgets.cpp \
	source/properties/PropertiesBrowser.cpp \
	source/properties/PropertiesWidgets.cpp \
	source/AboutDialog.cpp \
	source/ExportOptionsDialog.cpp \
	source/MainWindow.cpp \
	source/PreferencesDialog.cpp \
#	source/VsdxWriter.cpp \
	source/SvgWriter.cpp \
	source/main.cpp

HEADERS += \
	source/drawing/DrawingArrow.h \
	source/drawing/DrawingCanvas.h \
	source/drawing/DrawingCurveItem.h \
	source/drawing/DrawingEllipseItem.h \
	source/drawing/DrawingItem.h \
	source/drawing/DrawingItemFactory.h \
	source/drawing/DrawingItemGroup.h \
	source/drawing/DrawingItemPoint.h \
	source/drawing/DrawingLineItem.h \
	source/drawing/DrawingPathItem.h \
	source/drawing/DrawingPolygonItem.h \
	source/drawing/DrawingPolylineItem.h \
	source/drawing/DrawingRectItem.h \
	source/drawing/DrawingTextEllipseItem.h \
	source/drawing/DrawingTextItem.h \
	source/drawing/DrawingTextRectItem.h \
	source/drawing/DrawingTypes.h \
	source/drawing/DrawingUndo.h \
	source/drawing/DrawingWidget.h \
	source/items/ElectricItems.h \
	source/items/LogicItems.h \
	source/properties/HelperWidgets.h \
	source/properties/PropertiesBrowser.h \
	source/properties/PropertiesWidgets.h \
	source/AboutDialog.h \
	source/ExportOptionsDialog.h \
	source/MainWindow.h \
	source/PreferencesDialog.h \
#        source/VsdxWriter.h \
        source/SvgWriter.h

RESOURCES += icons/icons.qrc

RC_FILE = icons/icons.rc
OTHER_FILES += icons/icons.rc

OTHER_FILES += LICENSE README.md

!win32:MOC_DIR = release
!win32:OBJECTS_DIR = release
!win32:RCC_DIR = release

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
	icons/arrow/arrow_circle.png \
	icons/arrow/arrow_circle_filled.png \
	icons/arrow/arrow_concave.png \
	icons/arrow/arrow_concave_filled.png \
	icons/arrow/arrow_none.png \
	icons/arrow/arrow_normal.png \
	icons/arrow/arrow_triangle.png \
	icons/arrow/arrow_triangle_filled.png
