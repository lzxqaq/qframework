
/******************************************************************************
 *
 * @file       sqlhandler.h
 * @brief      用于加载 SQL 语句
 *
 * @author     lzx
 * @date       2021/09/01
 *
 * @history
 *****************************************************************************/


#ifndef SQLHANDLER_H
#define SQLHANDLER_H

#include <QHash>

#include "../orm_global.h"

class DbUtilConfig;
class SqlHandlerPrivate;

/**
 * @brief 单例模式，用来加载 SQL 语句
 *
 */
class ORM_EXPORT SqlHandler
{
    Q_DISABLE_COPY(SqlHandler)

public:
    static SqlHandler& instance();

    /**
     * @brief 获取sql
     * @param 相应sql的命名空间
     * @param 相应sql的id
     * @return sql字符串
     **/
    QString getSql(const QString &sqlNamespace, const QString &sqlId); // 取得 SQL 语句
private:
    SqlHandler();

    QHash<QString, QString> m_sqls; // Key 是 id, value 是 SQL 语句
    friend class SqlHandlerPrivate;

};

#endif // SQLHANDLER_H

/**
SQL 文件的定义
1. <sqls> 必须有 namespace
2. [<define>]*: <define> 必须在 <sql> 前定义，必须有 id 属性才有意义，否则不能被引用
3. [<sql>]*: <sql> 必须有 id 属性才有意义，<sql> 里可以用 <include defineId="define_id"> 引用 <define> 的内容

SQL 文件定义 Demo:
<sqls namespace="User">
    <define id="fields">id, username, password, email, mobile</define>

    <sql id="findByUserId">
        SELECT <include defineId="fields"/> FROM user WHERE id=%1
    </sql>

    <sql id="findAll">
        SELECT id, username, password, email, mobile FROM user
    </sql>

    <sql id="insert">
        INSERT INTO user (username, password, email, mobile)
        VALUES (:username, :password, :email, :mobile)
    </sql>

    <sql id="update">
        UPDATE user SET username=:username, password=:password,
            email=:email, mobile=:mobile
        WHERE id=:id
    </sql>
</sqls>

*/
