/****************************************************************************
* Copyright (C) 2013 Sergey Pupko <s.d.pupko@gmail.com>
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#pragma once

#include <QObject>
#include <QMetaType>
#include <QSharedPointer>
#include <QVariant>
#include <typeinfo>
#include <QDebug>
#include "idiobjbase.h"

#include "ioc_global.h"

namespace Ioc
{
class IOC_EXPORT DIContainer : public QObject
{
    Q_OBJECT
public:
    explicit DIContainer(QObject *parent = 0);
    virtual ~DIContainer();

    //注册接口和实现的关系（单例）
    template <typename ResolvableType, typename Type>
    void Bind()
    {
        QObject* objectFromType = static_cast<Type*>(0); Q_UNUSED(objectFromType);
        QObject* objectFromResolvableType = static_cast<ResolvableType*>(0); Q_UNUSED(objectFromResolvableType);
        ResolvableType* resolvableTypeFromType = static_cast<Type*>(0); Q_UNUSED(resolvableTypeFromType);
        ClassBind(static_cast<ResolvableType*>(0)->staticMetaObject, static_cast<Type*>(0)->staticMetaObject, true);
    }

    //注册类型（单例）
    template <typename Type>
    void Bind()
    {
        QObject* objectFromType = static_cast<Type*>(0); Q_UNUSED(objectFromType);
        ClassBind(static_cast<Type*>(0)->staticMetaObject);
    }

    //注册接口和实现的关系（瞬态）
    template <typename ResolvableType, typename Type>
    void BindTransient()
    {
        QObject* objectFromType = static_cast<Type*>(0); Q_UNUSED(objectFromType);
        QObject* objectFromResolvableType = static_cast<ResolvableType*>(0); Q_UNUSED(objectFromResolvableType);
        ResolvableType* resolvableTypeFromType = static_cast<Type*>(0); Q_UNUSED(resolvableTypeFromType);
        ClassBind(static_cast<ResolvableType*>(0)->staticMetaObject, static_cast<Type*>(0)->staticMetaObject, false);
    }

    //注册类型（瞬态）
    template <typename Type>
    void BindTransient()
    {
        QObject* objectFromType = static_cast<Type*>(0); Q_UNUSED(objectFromType);
        ClassBind(static_cast<Type*>(0)->staticMetaObject, false);
    }

    //注册键值和值（单例）
    void Bind(const QString &key, const QVariant &value)
    {
        ValueBind(key, value);
    }


    //通过接口（类型）得到具体对象
    template <typename ResolvableType>
    ResolvableType* Resolve()
    {
        QObject* resolvableTypeToObjectCastCheck = static_cast<ResolvableType*>(0); Q_UNUSED(resolvableTypeToObjectCastCheck);
        return qobject_cast<ResolvableType*>(ResolveMetaobject(static_cast<ResolvableType*>(0)->staticMetaObject));
    }

    QObject* ResolveMetaobject(QMetaObject metaObject);

    template<typename TFindType>
    TFindType *Find() {
        return qobject_cast<TFindType*>(Find0(static_cast<TFindType*>(0)->staticMetaObject));
    }

    //回收容器生成的对象（多值）
    template<typename... Args>
    void Collects(Args&&... values) {
        constexpr size_t size = sizeof...(Args);
        Collects0(size, values...);
    }

    //回收容器生成的对象
    void Collect(QObject *value){
        Collect0(value);
    }

private:
    void ClassBind(const QMetaObject &resolvableTypeMeta, const QMetaObject &typeMeta, bool isSingletonInstance);
    void ClassBind(const QMetaObject &typeMeta, bool isSingletonInstance);
    void ValueBind(const QString &key, const QVariant &value);

    void Collect0(QObject *value);
    void Collects0(size_t size, ...);
    QObject* Find0(QMetaObject metaObject);

private:
    class P; QSharedPointer<P> _d;

    QHash<QString, QMetaObject> _metaObjects1;
};

typedef QSharedPointer<DIContainer> DIContainerPtr;
}

Q_DECLARE_METATYPE(Ioc::DIContainerPtr)
