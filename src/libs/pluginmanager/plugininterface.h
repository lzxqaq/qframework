#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include "pluginmetadata.h"
#include <QObject>
#include <QtWidgets/QWidget>
#include <QHash>

class PluginInterface
{
public:
    virtual ~PluginInterface() {}

public:
    virtual void setInitData(QMap<QString,QVariant> &data) = 0;

    virtual QWidget *createWidget(QWidget *parent = Q_NULLPTR) = 0;

    virtual QMap<QString,QVariant>& getResultData() = 0;

    virtual void recMsgFromManager(PluginMetaData &msg) = 0;

    //connect to signals sendMsgToManager
    //virtual void sendMsgToManager(PluginMetaData &msg) = 0;
    virtual bool connectTosendMsgToManager(QObject *receiver, const char* pszSlot, bool isConnect = true) const = 0;
};
#define PluginInterface_iid "QtPlugin.PluginManager.PluginInterface"

Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)

/**
 * 插件间通信的接口
 * 暂时不支持跨线程
 **/
class PluginCI
{
public:
    /**
     * @method doCommand
     * @brief （模块）插件初始化时，其管理插件保存模块主窗口（对象）指针到m_pluginCIHash或其它位置。
     *         模块主窗口实现PluginCI接口，提供跨插件调用特定QObject的特定函数的功能。
     * @return 跨插件调用后，函数执行的结果。
     * @param msg：传递的参数
     **/
    virtual int doCommand(int plugin, PluginMetaData &msg) = 0;

    /**
     * @method connectCI
     * @brief 根据开发的约定，直接连接发送者的所有信号与接收者的所有槽。
     *        调用插件提供发送者，当前插件提供接收者。
     * @param senderName：发送者对象名称/类名，sender：发送者对象，reciverName：接收者对象名称/类型名（可为空）
     **/
    virtual void connectCI(int plugin, const QString &senderName, QObject *sender, const QString &reciverName) = 0;
    /**
     * @method connectCI
     * @brief 根据开发的约定，直接连接接收者的所有槽。调
     *        用插件提供接收者，当前插件提供发送者。
     * @param senderName：发送者对象名称/类名，sender：发送者对象，reciverName：接收者对象名称/类型名
     **/
    virtual void connectCI(int plugin, const QString &reciverName, QObject *reciver) = 0;

    /**
     * @method saveCI
     * @brief 将模块主窗口（对象）指针保存到另外的插件中
     * @param 参数描述
     **/
    inline void saveCI(int plugin, QObject *ci);

    inline int plugin() const;

    inline QHash<int, QObject *> pluginCIHash() const;

protected:
    /**
     * @variable m_pluginCIHash
     * @brief 插件Hash集合
     **/
    QHash<int, QObject*> m_pluginCIHash;
    /**
     * @variable m_plugin
     * @brief 插件名
     **/
    int m_plugin;
};

void PluginCI::saveCI(int plugin, QObject *ci)
{
    if(m_plugin != plugin && !m_pluginCIHash.contains(plugin)) {
        m_pluginCIHash.insert(plugin, ci);
    }
}

int PluginCI::plugin() const
{
    return m_plugin;
}

QHash<int, QObject *> PluginCI::pluginCIHash() const
{
    return m_pluginCIHash;
}

#endif // PLUGININTERFACE_H

