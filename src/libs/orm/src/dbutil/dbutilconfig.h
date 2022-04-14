
/******************************************************************************
 *
 * @file       dbutilconfig.h
 * @brief      用来读配置文件
 *
 * @author     lzx
 * @date       2021/09/01
 *
 * @history
 *****************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H
#include <QSettings>
#include <QJsonDocument>
#include <QString>

/**
 * @brief 单例模式，用来读写配置文件，包括 1.debug 是否需要打印日志信息 2.sql文件路径
 */
class DbUtilConfig
{
    Q_DISABLE_COPY(DbUtilConfig)

public:
    /**
     * @brief 获取唯一实例
     * @param
     * @return
     **/
    static DbUtilConfig& instance();
    bool getDebug() const;
    void setDebug(bool value);

    QStringList getSqlFiles() const;
    void setSqlFiles(const QStringList &value);

private:
    QJsonDocument readConfigFile(const QString& configFilePath);

    void readJsonConfig(const QJsonDocument& jsonConfig);

private:
    bool debug;
    QStringList sqlFiles;
    DbUtilConfig();
};

#endif // CONFIG_H
