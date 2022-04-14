#include "pluginmanager.h"
#include "pluginmanagerprivate.h"
#include "iplugin.h"
#include <QDir>
#include <QCoreApplication>
#include <QJsonArray>
#include <QDebug>

PluginManager* PluginManager ::s_instance = nullptr;

PluginManager ::PluginManager ()
{
    d = new PluginManagerPrivate();
}

PluginManager ::~PluginManager ()
{
    if(d)
        delete d;
}

void PluginManager ::loadAllPlugins()
{
    QDir pluginsdir = QDir(qApp->applicationDirPath());
    pluginsdir.cd("plugins");

    QFileInfoList pluginsInfo = pluginsdir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    //初始化插件中的元数据
    for(QFileInfo fileinfo : pluginsInfo)
        scanMetaData(fileinfo.absoluteFilePath());

    //加载插件
    for(QFileInfo fileinfo : pluginsInfo)
        loadPlugin(fileinfo.absoluteFilePath());
}

void PluginManager ::scanMetaData(const QString &filepath)
{
    //判断是否为库（后缀有效性）
    if(!QLibrary::isLibrary(filepath))
        return ;
    //获取元数据
    QPluginLoader *loader = new QPluginLoader(filepath);
    QJsonObject json = loader->metaData().value("MetaData").toObject();

    d->m_names.insert(filepath, json.value("Name").toVariant());
    d->m_versions.insert(filepath, json.value("Version").toVariant());
    d->m_dependencies.insert(filepath, json.value("Dependencies").toArray().toVariantList());

    delete loader;
    loader = nullptr;
}


void PluginManager ::loadPlugin(const QString &filepath)
{
    if(!QLibrary::isLibrary(filepath))
        return;

    //检测依赖
    if(!d->check(filepath))
        return;

    //加载插件
    QPluginLoader *loader = new QPluginLoader(filepath);

    if(loader->load())
    {
        // 如果继承自 Plugin，则认为是自己的插件（防止外部插件注入）。
        IPlugin *plugin = qobject_cast<IPlugin *>(loader->instance());
        if(plugin)
        {
            d->m_loaders.insert(filepath, loader);
            plugin->initialize();
        }
        else
        {
            delete loader;
            loader = nullptr;
        }
    }
}

void PluginManager ::unloadAllPlugins()
{
    for(QString filepath : d->m_loaders.keys())
        unloadPlugin(filepath);
}

void PluginManager ::unloadPlugin(const QString &filepath)
{
    QPluginLoader *loader = d->m_loaders.value(filepath);
    //卸载插件，并从内部数据结构中移除
    if(loader->unload())
    {
        d->m_loaders.remove(filepath);
        delete loader;
        loader = nullptr;
    }
}

QList<QPluginLoader *> PluginManager ::allPlugins()
{
    return d->m_loaders.values();
}

QPluginLoader* PluginManager ::getPlugin(const QString &name)
{
    return d->m_loaders.value(d->m_names.key(name));
}

QVariant PluginManager ::getPluginName(QPluginLoader *loader)
{
    if(loader)
        return d->m_names.value(d->m_loaders.key(loader));
    else
        return "";
}
