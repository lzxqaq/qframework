#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QObject>
#include "pluginmanager_global.h"

class IPluginPrivate;

class PLUGINMANAGER_EXPORT IPlugin : public QObject
{
    Q_OBJECT
public:
    IPlugin();
    ~IPlugin() override;
    virtual void initialize();

private:
    IPluginPrivate *d;
};
#define PluginInterface_iid "PluginManager.IPlugin"

Q_DECLARE_INTERFACE(IPlugin,PluginInterface_iid)
#endif // PLUGININTERFACE_H
