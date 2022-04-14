QT += sql

DEFINES += ORM_LIBRARY

SOURCES += \
    $$PWD/connectionpool/connection.cpp \
    $$PWD/connectionpool/connectionpool.cpp \
    $$PWD/connectionpool/connectionpoolprivate.cpp \
    $$PWD/connectionpool/databaseconfig.cpp \
    $$PWD/connectionpool/poolconfig.cpp \
    $$PWD/dbutil/dbutil.cpp \
    $$PWD/dbutil/dbutilconfig.cpp \
    $$PWD/dbutil/sqlhandler.cpp


HEADERS += \
    $$PWD/connectionpool/connection.h \
    $$PWD/connectionpool/connectionpool.h \
    $$PWD/connectionpool/connectionpoolprivate.h \
    $$PWD/connectionpool/databaseconfig.h \
    $$PWD/connectionpool/poolStats.h \
    $$PWD/connectionpool/poolconfig.h \
    $$PWD/dbutil/dbutil.h \
    $$PWD/dbutil/dbutilconfig.h \
    $$PWD/dbutil/sqlhandler.h \
    $$PWD/orm_global.h
