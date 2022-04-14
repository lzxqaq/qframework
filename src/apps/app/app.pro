include(../../../qtproject.pri)

TEMPLATE = app


SOURCES += \
        main.cpp



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/pluginmanager/release/ -lpluginmanager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/pluginmanager/debug/ -lpluginmanager
else:unix: LIBS += -L$$OUT_PWD/../../libs/pluginmanager/ -lpluginmanager

INCLUDEPATH += $$PWD/../../libs/pluginmanager
DEPENDPATH += $$PWD/../../libs/pluginmanager


