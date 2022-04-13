#include "connectorcontainer.h"
using namespace Connector;


ConnectorPair::ConnectorPair(QMetaObject metaObject1, QMetaObject metaObject2)
    :   _metaObject1(metaObject1)
    ,   _metaObject2(metaObject2)
{
}

ConnectorPair::~ConnectorPair()
{
    _objects1.clear();
    _objects2.clear();
    binding = nullptr;
    unbind = nullptr;
    match = nullptr;
}

//加入集合，并调用绑定函数，符合要求返回true，否则为false
bool ConnectorPair::Add(QObject *obj)
{
    if (obj == nullptr)
        return false;

    auto t = obj->metaObject()->className();

    if(t == _metaObject1.className()) {
        if (_objects1.contains(obj))
            return true;

        _objects1.append(obj);
        connect(obj, &QObject::destroyed, this, &ConnectorPair::onViewModelDestroyed);
        _names[obj] = QString::fromLatin1(t);
        if(binding != nullptr) {
            for (auto &obj2 : _objects2){
                auto flag = true;

                if (match != nullptr)
                {
                    flag = match(obj, obj2);
                }

                if (flag){
                    binding(obj, obj2);
                }
            }
        }
        return true;
    }
    else if(t == _metaObject2.className()) {
        if (_objects2.contains(obj))
            return true;

        _objects2.append(obj);
        connect(obj, &QObject::destroyed, this, &ConnectorPair::onViewModelDestroyed);
        _names[obj] = QString::fromLatin1(t);
        if(binding != nullptr) {
            for (auto &obj1 : _objects1){
                auto flag = true;

                if (match != nullptr)
                {
                    flag = match(obj1, obj);
                }

                if (flag){
                    binding(obj1, obj);
                }
            }
        }
        return true;
    }
    return false;
}

bool ConnectorPair::Remove(QObject *obj)
{
    if(obj == nullptr || !_names.contains(obj))
        return false;

    auto t = _names[obj];
    _names.remove(obj);

    if(t == QString::fromLatin1(_metaObject1.className())) {
        if (!_objects1.contains(obj))
            return false;

        if (unbind != nullptr)
        {
            for (auto &obj2 : _objects2)
            {
                auto flag = true;

                if(match != nullptr)
                    flag = match(obj, obj2);

                if (flag)
                    unbind(obj, obj2);

            }
        }
        _objects1.removeOne(obj);
        return true;
    }
    else if (t == QString::fromLatin1(_metaObject1.className())) {
        if (!_objects2.contains(obj))
            return false;
        if (unbind != nullptr){
            for (auto &obj1 : _objects1) {
                auto flag = true;

                if(match != nullptr)
                    flag = match(obj1, obj);

                if(flag)
                    unbind(obj1, obj);
            }
        }
        _objects2.removeOne(obj);
        return true;
    }
    return false;
}

void ConnectorPair::onViewModelDestroyed(QObject *obj)
{
    if (_names.contains(obj)) {
        _names.remove(obj);
    }
}

IConnectorContainer::IConnectorContainer(QObject *parent)
    : IDIObj(parent)
{
}

IConnectorContainer::~IConnectorContainer()
{
    qDeleteAll(_map);
    _map.clear();
}

void IConnectorContainer::Add(QObject *obj)
{
    for(auto &v : _map)
    {
        v->Add(obj);
    }
}

void IConnectorContainer::Remove(QObject *obj)
{
    for(auto &v : _map)
    {
        v->Remove(obj);
    }
}

DefaultConnectorContainer::DefaultConnectorContainer(QObject *parent)
    : IConnectorContainer(parent)
{

}

DefaultConnectorContainer::~DefaultConnectorContainer()
{

}
