include(mvvmdemo.pri)
include(mvvmdemo_dependencies.pri)

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../3plibs/advanceddockingsystem/release/ -lqtadvanceddockingd
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../3plibs/advanceddockingsystem/debug/ -lqtadvanceddockingd
else:unix: LIBS += -L$$OUT_PWD/../../3plibs/advanceddockingsystem/ -lqtadvanceddockingd

INCLUDEPATH += $$PWD/../../3plibs/advanceddockingsystem
DEPENDPATH += $$PWD/../../3plibs/advanceddockingsystem
