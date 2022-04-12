#include "dbutil.h"
#include "connectionpool/connectionpool.h"

#include "dbutilconfig.h"
#include <QDebug>
#include <QTime>

DBUtil::DBUtil()
{
    QSqlDatabase db = ConnectionPool().getConnection()->database();
    m_query = new QSqlQuery(db);
}

DBUtil::~DBUtil()
{
    //不释放的话，会有以下异常
    //QODBCResult::exec: Unable to execute statement: "[Microsoft][ODBC SQL Server Driver]连接占线导致另一个 hstmt"
    delete m_query;
}

int DBUtil::insert(const QString &sql, const QVariantMap &params) {
    int id = -1;

    executeSql(sql, params, [&id](QSqlQuery *query) {
        id = query->lastInsertId().toInt(); // 插入行的主键
    });

    return id;
}

bool DBUtil::insertBatch(const QString &sql, const QList<QVariantMap> &params)
{
    bool flag = false;

    executeBatchSql(sql, params, [&flag](QSqlQuery *query) {
        flag = query->lastError().type() == QSqlError::NoError;
    });
    return flag;
}

bool DBUtil::update(const QString &sql, const QVariantMap &params) {
    bool result;

    executeSql(sql, params, [&result](QSqlQuery *query) {
        result = query->lastError().type() == QSqlError::NoError;
    });

    return result;
}

bool DBUtil::updateBatch(const QString &sql, const QList<QVariantMap> &params)
{
    bool result;

    executeBatchSql(sql, params, [&result](QSqlQuery *query) {
        result = query->lastError().type() == QSqlError::NoError;
    });

    return result;
}

int DBUtil::selectInt(const QString &sql, const QVariantMap &params) {
    return selectVariant(sql, params).toInt();
}

qint64 DBUtil::selectInt64(const QString &sql, const QVariantMap &params) {
    return selectVariant(sql, params).toLongLong();
}

QString DBUtil::selectString(const QString &sql, const QVariantMap &params) {
    return selectVariant(sql, params).toString();
}

QDate DBUtil::selectDate(const QString &sql, const QVariantMap &params) {
    return selectVariant(sql, params).toDate();
}

QDateTime DBUtil::selectDateTime(const QString &sql, const QVariantMap &params) {
    return selectVariant(sql, params).toDateTime();
}

QVariant DBUtil::selectVariant(const QString &sql, const QVariantMap &params) {
    QVariant result;

    executeSql(sql, params, [&result](QSqlQuery *query) {
        if (query->next()) {
            result = query->value(0);
        }
    });

    return result;
}

bool DBUtil::transaction()
{
    QSqlDatabase db = ConnectionPool().getConnection()->database();
    return db.transaction();
}

bool DBUtil::commit()
{
    QSqlDatabase db = ConnectionPool().getConnection()->database();
    return db.commit();
}

bool DBUtil::rollback()
{
    QSqlDatabase db = ConnectionPool().getConnection()->database();
    return db.rollback();
}

QString DBUtil::lastError()
{
    QString result;
    if (m_query->lastError().type() != QSqlError::NoError)
    {
        result = m_query->lastError().text().trimmed();
    }
    return result;
}

void DBUtil::executeSql(const QString &sql, const QVariantMap &params)
{
    m_query->setForwardOnly(true);//结果集仅向前，可以更有效地利用内存，它还将提高某些数据库的性能

    m_query->prepare(sql);
    bindValues(m_query, params);
    m_query->exec();

    debug(*m_query, params);
}

void DBUtil::executeBatchSql(const QString &sql, const QList<QVariantMap> &params)
{
    m_query->setForwardOnly(true);//结果集仅向前，可以更有效地利用内存，它还将提高某些数据库的性能
    m_query->prepare(sql);
    bindBatchValues(m_query, params);
    m_query->execBatch();

    debug(*m_query);
}


bool DBUtil::next()
{
    return m_query->next();
}

QVariant DBUtil::value(int i)
{
    return m_query->value(i);
}

QVariant DBUtil::value(const QString &name)
{
    return m_query->value(name);
}

QVariantList DBUtil::getResultList(int size)
{
    QVariantList result;
    for (int i = 0; i < size; i++)
    {
        result << value(i);
    }
    return result;
}

QStringList DBUtil::selectStrings(const QString &sql, const QVariantMap &params)
{
    QStringList strings;

    executeSql(sql, params, [&strings](QSqlQuery *query) {
        while (query->next()) {
            strings.append(query->value(0).toString());
        }
    });

    return strings;
}



QVariantMap DBUtil::selectMap(const QString &sql, const QVariantMap &params)
{
    return selectMaps(sql, params).value(0);
}

QList<QVariantMap> DBUtil::selectMaps(const QString &sql, const QVariantMap &params)
{
    QList<QVariantMap> maps;

    executeSql(sql, params, [&maps, this](QSqlQuery *query) {
        maps = queryToMaps(query);
    });

    return maps;
}

QVariantList DBUtil::selectList(const QString &sql, const QVariantMap &params)
{
    return selectLists(sql, params).value(0);

}

QList<QVariantList> DBUtil::selectLists(const QString &sql, const QVariantMap &params)
{
    QList<QVariantList> lists;

    executeSql(sql, params, [&lists, this](QSqlQuery *query) {
        lists = queryToLists(query);
    });

    return lists;
}

void DBUtil::bindValues(QSqlQuery *query, const QVariantMap &params)
{
    for (QVariantMap::const_iterator i=params.constBegin(); i!=params.constEnd(); ++i)
    {
        query->bindValue(":" + i.key(), i.value());
    }
}

void DBUtil::bindBatchValues(QSqlQuery *query, const QList<QVariantMap> &params)
{
    foreach (auto param, params)
    {
        for (QVariantMap::const_iterator i=param.constBegin(); i!=param.constEnd(); ++i)
        {
            query->bindValue(":" + i.key(), i.value());
        }
    }
}

QStringList DBUtil::getFieldNames(const QSqlQuery &query)
{
    QSqlRecord record = query.record();
    QStringList names;
    int count = record.count();

    for (int i = 0; i < count; ++i) {
        names << record.fieldName(i);
    }

    return names;
}

QStringList DBUtil::getFieldNames() const
{
    QSqlRecord record = m_query->record();
    QStringList names;
    int count = record.count();

    for (int i = 0; i < count; ++i) {
        names << record.fieldName(i);
    }

    return names;
}

int DBUtil::getFieldSize(const QSqlQuery &query)
{
    QSqlRecord record = query.record();
    return record.count();
}

int DBUtil::getFieldSize() const
{
    QSqlRecord record = m_query->record();
    return record.count();
}

QList<QVariantMap> DBUtil::queryToMaps(QSqlQuery *query)
{
    QList<QVariantMap > rowMaps;
    QStringList fieldNames = getFieldNames(*query);

    while (query->next()) {
        QVariantMap rowMap;

        foreach (const QString &fieldName, fieldNames)
        {
            rowMap.insert(fieldName, query->value(fieldName));
        }

        rowMaps.append(rowMap);
    }

    return rowMaps;
}

QList<QVariantList> DBUtil::queryToLists(QSqlQuery *query)
{
    QList<QVariantList > rowLists;
    int fieldSize = getFieldSize(*query);

    while (query->next()) {
        QVariantList rowList;

        for (int i = 0; i < fieldSize; i++)
        {
            rowList.append(query->value(i));
        }

        rowLists.append(rowList);
    }
    return rowLists;
}

void DBUtil::debug(const QSqlQuery &query, const QVariantMap &params)
{
    if (DbUtilConfig::instance().getDebug())
    {
        if (query.lastError().type() != QSqlError::NoError)
        {
            qDebug().noquote() << "==> SQL Error: " << query.lastError().text().trimmed();
        }

        qDebug().noquote() << "==> SQL Query:" << query.lastQuery();

        if (params.size() > 0) {
            qDebug().noquote() << "==> SQL Params: " << params;
        }
    }
}

void DBUtil::debug(const QSqlQuery &query)
{
    if (DbUtilConfig::instance().getDebug())
    {
        if (query.lastError().type() != QSqlError::NoError)
        {
            qDebug().noquote() << "==> SQL Error: " << query.lastError().text().trimmed();
        }

        qDebug().noquote() << "==> SQL Query:" << query.lastQuery();
    }
}


