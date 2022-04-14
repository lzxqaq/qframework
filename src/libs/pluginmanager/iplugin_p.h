#ifndef IPLUGIN_P_H
#define IPLUGIN_P_H

#include <QObject>
#include <QPluginLoader>
#include "iplugin.h"
#include "pluginmanager_global.h"

class PLUGINMANAGER_EXPORT IPluginPrivate : public QObject
{
    Q_OBJECT
public:
    IPluginPrivate(IPlugin *plugin);

    QPluginLoader loader;

private:
    IPlugin *q;
};


#endif // IPLUGIN_P_H
