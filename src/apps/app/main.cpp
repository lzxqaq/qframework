#include <QCoreApplication>
#include <QDebug>

#include "pluginmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication app( argc, argv );

    PluginManager::instance()->loadAllPlugins();//插件管理器 加载所有插件

    app.exec();
}
