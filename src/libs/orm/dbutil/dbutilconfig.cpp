#include "dbutilconfig.h"

#include <QFile>
#include <QJsonObject>

DbUtilConfig::DbUtilConfig()
    : debug(false)
    , sqlFiles()
{
    QJsonDocument jsonConfig = readConfigFile(":res/dbutil.json");
    readJsonConfig(jsonConfig);
}

QJsonDocument DbUtilConfig::readConfigFile(const QString& configFilePath)
{
    QJsonDocument jsonConfig;

    QFile file(configFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("DbUtilConfig: Config file '%s' cannot be opened", qPrintable(configFilePath));
        jsonConfig = QJsonDocument();
    } else {
        jsonConfig = QJsonDocument::fromJson(file.readAll());
    }
    file.close();

    return jsonConfig;
}


void DbUtilConfig::readJsonConfig(const QJsonDocument& jsonConfig)
{
    QVariantMap configMap = jsonConfig.object().toVariantMap();
    QVariantMap dbutilConfig = configMap.value("dbutil", QVariantMap()).toMap();

    this->debug = dbutilConfig.value("debug", false).toBool();
    this->sqlFiles = dbutilConfig.value("sqlFiles", QStringList()).toStringList();
}

QStringList DbUtilConfig::getSqlFiles() const
{
    return sqlFiles;
}

void DbUtilConfig::setSqlFiles(const QStringList &value)
{
    sqlFiles = value;
}

DbUtilConfig &DbUtilConfig::instance()
{
    static DbUtilConfig instance;//静态局部变量，内存中只有一个，且只会被初始化一次
    return instance;
}

bool DbUtilConfig::getDebug() const
{
    return debug;
}

void DbUtilConfig::setDebug(bool value)
{
    debug = value;
}
