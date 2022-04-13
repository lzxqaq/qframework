#ifndef CONNECTORCONTAINER_H
#define CONNECTORCONTAINER_H

#include <QObject>
#include <QList>
#include "../mvvm_global.h"
#include "ioc.h"

namespace Connector {

    //两个 Object 对象之间的关系，QObject1 和 QObject 2
    class MVVM_EXPORT ConnectorPair : public QObject {
        Q_OBJECT
    public:

        //绑定的时候调用的函数
        std::function<bool(QObject *obj1, QObject *obj2)> binding = nullptr;
        //解绑的时候调用的函数
        std::function<bool(QObject *obj1, QObject *obj2)> unbind = nullptr;
        //匹配两者是否可以绑定（解绑）的函数 如果未nullptr 默认都需要绑定（解绑）
        std::function<bool(QObject *obj1, QObject *obj2)> match = nullptr;

        ConnectorPair(QMetaObject metaObject1, QMetaObject metaObject2);
        ~ConnectorPair();

        //加入集合，并调用绑定函数，符合要求返回true，否则为false
        bool Add(QObject *obj);
        //移除集合，并调用解绑函数，符合要求返回true，否则为false
        bool Remove(QObject *obj);

    private:
        void onViewModelDestroyed(QObject *obj = nullptr);

    private:
        QMetaObject _metaObject1;   //QObject1原型
        QMetaObject _metaObject2;   //QObject2原型

        QList<QObject *> _objects1; //QObject1实体集合
        QList<QObject *> _objects2; //QObject2实体集合

        QHash<QObject*, QString> _names;
    };


    //统一管理 QObject 之间的信号和槽的容器
    //用于前期声明绑定关系，后面对象生成的时候自动绑定关系
    class MVVM_EXPORT IConnectorContainer : public IDIObj
    {
        Q_OBJECT
    public:
        explicit IConnectorContainer(QObject *parent = 0);
        ~IConnectorContainer();

        template<typename T1, typename T2>
        void binding(std::function<bool(QObject *obj1, QObject *obj2)> bindingFunc,
                     std::function<bool(QObject *obj1, QObject *obj2)> unbindFunc = nullptr,
                     std::function<bool(QObject *obj1, QObject *obj2)> matchFunc = nullptr)
        {
            auto metaObject1 = static_cast<T1*>(0)->staticMetaObject;
            auto metaObject2 = static_cast<T2*>(0)->staticMetaObject;
            auto pair = new ConnectorPair(metaObject1, metaObject2);
            pair->binding = bindingFunc;
            pair->unbind = unbindFunc;
            pair->match = matchFunc;
            _map.append(pair);
        }

        void Add(QObject *obj);
        void Remove(QObject *obj);
    private:
        QList<ConnectorPair *> _map;
    };

    class MVVM_EXPORT DefaultConnectorContainer : public IConnectorContainer
    {
        Q_OBJECT
    public:
        Q_INVOKABLE explicit DefaultConnectorContainer(QObject *parent = 0);
        ~DefaultConnectorContainer();
    };
}

#endif // CONNECTORCONTAINER_H
