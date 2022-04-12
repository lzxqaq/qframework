#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H

#include <QDateTime>
#include <QDebug>
#include <QMutableListIterator>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QSharedPointer>
#include <QThread>
#include <QTimer>
#include <QSharedPointer>
#include "poolconfig.h"
#include "poolStats.h"
#include "connection.h"
#include "../orm_global.h"


class PoolConfig;
class Connection;
class ConnectionPoolPrivate;

class ORM_EXPORT ConnectionPool {
    static ConnectionPoolPrivate* pool;

public:
    ConnectionPool();

    explicit ConnectionPool(const QString& configFilePath);
    explicit ConnectionPool(const PoolConfig& poolConfig);

    QSharedPointer<Connection> getConnection(uint64_t waitTimeoutInMs = 0);
    void unBorrowConnection(QSharedPointer<Connection> con);

    PoolStats getPoolStats() const;

    void destroy();
};


#endif // CONNECTIONPOOL_H
