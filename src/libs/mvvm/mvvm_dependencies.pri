# ioc
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ioc/release/ -lioc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ioc/debug/ -lioc
else:unix: LIBS += -L$$OUT_PWD/../ioc/ -lioc

INCLUDEPATH += $$PWD/../ioc
DEPENDPATH += $$PWD/../ioc
