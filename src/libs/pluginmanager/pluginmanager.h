#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "pluginmanager_global.h"
#include "pluginmetadata.h"
#include <QObject>
#include <QPluginLoader>
#include <QVariant>

class PluginManagerPrivate;

class PLUGINMANAGER_EXPORT PluginManager : public QObject
{
    Q_OBJECT
public:

    PluginManager();
    ~PluginManager();

    static PluginManager *instance(){
        if(s_instance==nullptr)
            s_instance=new PluginManager();
        return s_instance;
    }

    //加载所有插件
    void loadAllPlugins();

    //扫描JSON文件中的插件元数据
    void scanMetaData(const QString &filepath);
    //加载其中某个插件
    void loadPlugin(const QString &filepath);
    //卸载所有插件
    void unloadAllPlugins();
    //卸载某个插件
    void unloadPlugin(const QString &filepath);
    //获取所有插件
    QList<QPluginLoader *> allPlugins();
    //获取某个插件
    QPluginLoader* getPlugin(const QString &name);
    //获取所有插件名称
    QList<QVariant> allPluginsName();
    //获取某个插件名称
    QVariant getPluginName(QPluginLoader *loader);

private:
    static PluginManager *s_instance;
    PluginManagerPrivate *d;

private slots:
    void recMsgFromManager(PluginMetaData&);

};

#endif
