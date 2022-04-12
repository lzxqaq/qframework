#ifndef PLUGINMETADATA_H
#define PLUGINMETADATA_H

#include <QList>
#include <QString>
#include <QObject>

/**
 * 插件间消息封装体
 **/
struct PluginMetaData
{
    /**
     * 插件间发送者窗体名称/类型名
     **/
    QString from;
    /**
     * 插件间接收者窗体名称/类型名
     **/
    QString dest;

    QVariantList varList;
    QList<void*> autoList;
};
Q_DECLARE_METATYPE(PluginMetaData);

#endif // PLUGINMETADATA_H


