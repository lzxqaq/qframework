#include <QDateTime>
#include <QDebug>
#include <QMutableListIterator>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QSharedPointer>
#include <QThread>
#include <QTimer>
#include "connectionpool.h"
#include "connectionpoolprivate.h"
#ifdef _WIN32
#  define EXPORTIT __declspec( dllexport )
#else
#  define EXPORTIT extern
#endif
EXPORTIT QString appPathConn;

ConnectionPoolPrivate* ConnectionPool::pool = 0;

ConnectionPool::ConnectionPool(const QString& configFilePath)
: ConnectionPool(PoolConfig(configFilePath))
{}

ConnectionPool::ConnectionPool(const PoolConfig &poolConfig) {
    if (!pool) {
        ConnectionPoolPrivate::setupPool(poolConfig, nullptr);
        pool = &ConnectionPoolPrivate::getInstance();
    } else {
        qWarning("ConnectionPool: already initialized, skipping configuration");
    }
}

ConnectionPool::ConnectionPool() {
    if (!pool) {
        ConnectionPool( PoolConfig( appPathConn + QLatin1Char('/') + "db.json" ) );
        //qFatal("ConnectionPool: pool was not initialized");
    }
}

void ConnectionPool::destroy() {
    if (pool) {
        pool->stop = true;
    }
}

QSharedPointer<Connection> ConnectionPool::getConnection(uint64_t waitTimeoutInMs) {
    return pool->getConnection(waitTimeoutInMs);
}

void ConnectionPool::unBorrowConnection(QSharedPointer<Connection> con) {
    pool->unBorrowConnection(con);
}

PoolStats ConnectionPool::getPoolStats() const {
    return pool->getPoolStats();
}
