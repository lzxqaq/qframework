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

#include "dicontainer.h"

#include <QMetaObject>
#include <QMetaMethod>
#include <QDebug>

class CtorArg
{
public:
    CtorArg(QString name, void* data):
        _name(name.toUtf8()),
        _data(data),
        _hasValue(false)
    {
    }

    CtorArg(QString name, QVariant value):
        _name(name.toUtf8()),
        _value(value),
        _hasValue(true)
    {
    }

    QGenericArgument toQArg()
    {
        if (_hasValue)
        {
            return QGenericArgument(_name.data(), _value.constData());
        }

        return _name == ""
                   ? QGenericArgument()
                   : QGenericArgument(_name.data(), &_data);
    }

private:
    QByteArray _name;
    void *_data;
    QVariant _value;
    bool _hasValue;

    Q_DISABLE_COPY(CtorArg)
};
typedef QSharedPointer<CtorArg> CtorArgPtr;

using namespace Ioc;

class DIContainer::P : public QObject
{
public:
    explicit P(QObject *parent = 0) :
        QObject(parent)
    {
    }

    virtual ~P()
    {
        Dispose();
    }

    void Dispose(){
        //单例和瞬态之间的删除矛盾
        while (_transientInstances.count() != 0) {
            auto key = _transientInstances.constBegin().key();
            auto value = _transientInstances.constBegin().value();
            _transientInstances.remove(key);
            for (auto v : value)
            {
                MyCollect(v);
            }
        }
        while(_singleInstances.count() != 0)
        {
            auto key = _singleInstances.constBegin().key();
            auto value = _singleInstances.constBegin().value();
            _singleInstances.remove(key);
            MyCollect(value);
        }
        _singleTypeMap.clear();
    }

    void RegisterMetaObject(const QString &typeName, const QMetaObject &metaObject, bool isSingletonInstance)
    {
        if(ContainsMetaObject(typeName))//不允许多次注入
            return;
        _metaObjects.insert(typeName, metaObject);
        if(isSingletonInstance)
        {
            _singleTypeMap << typeName;
        }
    }

    bool ContainsMetaObject(const QString &typeName)
    {
        return _metaObjects.contains(typeName);
    }

    QMetaObject GetMeataObject(const QString &typeName)
    {
        return ContainsMetaObject(typeName)
                   ? _metaObjects.value(typeName)
                   : QMetaObject();
    }

    void RegisterValue(const QString &typeName, const QString &key, const QVariant &value)
    {
        if (!_objects.contains(typeName))
        {
            _objects.insert(typeName, QHash<QString, QVariant>());
        }

        _objects[typeName].insert(key, value);
    }

    bool ContainsValue(const QString &typeName, const QString &key)
    {
        if (!_objects.contains(typeName))
        {
            return false;
        }

        if (!_objects[typeName].contains(key))
        {
            return false;
        }

        return true;
    }

    QVariant GetValue(const QString &typeName, const QString &key)
    {
        return ContainsValue(typeName, key)
                   ? _objects[typeName].value(key)
                   : QVariant();
    }

    QObject* ResolveByName(QString typeName)
    {
        if (typeName.endsWith("*"))
        {
            typeName.chop(1);
        }

        if (!ContainsMetaObject(typeName))
        {
            qDebug() << "DIContainer: " << typeName << " is not registered or may be you forgot specify namespace in constructor!";
            return NULL;
        }
        //如果是单例，且已经生成对象，直接返回该对象
        if(_singleTypeMap.contains(typeName) && _singleInstances.contains(typeName))
        {
            return _singleInstances.value(typeName);
        }

        QMetaObject metaObject = GetMeataObject(typeName);
        QMetaMethod constructorType = metaObject.constructor(0);
        QList<CtorArgPtr> ctorArguments;

        for (quint8 index = 0; index < 10; index++)
        {
            if (index >= constructorType.parameterTypes().count())
            {
                CtorArgPtr ctorArg(new CtorArg("", 0));
                ctorArguments << ctorArg;
                continue;
            }

            QString argType = constructorType.parameterTypes().at(index);
            QString argName = constructorType.parameterNames().at(index);

            if (argType == "QObject*" && argName == "parent")
            {
                CtorArgPtr ctorArg(new CtorArg("QObject*", 0));
                ctorArguments << ctorArg;
                continue;
            }

            if (ContainsValue(argType, argName))
            {
                CtorArgPtr ctorArg(new CtorArg(argType, GetValue(argType, argName)));
                ctorArguments << (ctorArg);
                continue;
            }

            QObject* argValue = ResolveByName(argType);

            if (argValue == NULL)
            {
                return NULL;
            }

            if (_singleTypeMap.contains(argType)){//单例
                CtorArgPtr ctorArg(new CtorArg(argType, static_cast<void *>(argValue)));
                ctorArguments << ctorArg;
            }
            else {//瞬态
                QVariant argVar;
                argVar.setValue(argValue);
                CtorArgPtr ctorArg(new CtorArg(argType, argVar));
                ctorArguments << ctorArg;
            }

        }

        QObject  *instance = metaObject.newInstance(ctorArguments[0]->toQArg(), ctorArguments[1]->toQArg(), ctorArguments[2]->toQArg(), ctorArguments[3]->toQArg(), ctorArguments[4]->toQArg(),
                                                   ctorArguments[5]->toQArg(),ctorArguments[6]->toQArg(),ctorArguments[7]->toQArg(),ctorArguments[8]->toQArg(),ctorArguments[9]->toQArg());

        if (!instance)
        {
            qDebug() << "DIContainer: could not create an instance of class " << metaObject.className();
            return NULL;
        }

        _instancesType.insert(instance, typeName);
        if (_singleTypeMap.contains(typeName))//单例
        {
            _singleInstances.insert(typeName, instance);
        }
        else {//瞬态
            if(!_transientInstances.contains(typeName))
            {
                QList<QObject*> list;
                _transientInstances.insert(typeName, list);
            }
            _transientInstances[typeName].append(instance);
        }

        if(AfterNewInstance != nullptr)
        {
            AfterNewInstance(instance);
        }
        return instance;
    }

    QObject* Find(const QString &typeName) {
        if (_singleTypeMap.contains(typeName)){
            if(_singleInstances.contains(typeName)) {
                return _singleInstances.value(typeName);
            }
        }
        else if (_transientInstances.contains(typeName)) {
            auto list = _transientInstances.value(typeName);
            if (list.count() > 0)
            {
                return list[0];
            }
        }
        return nullptr;
    }

    void Collect(QObject *value){
        if (!value || !_instancesType.contains(value)) return ;
        auto typeName = _instancesType[value];
        if(_transientInstances.contains(typeName)){
            if(_transientInstances[typeName].contains(value))
            {
                _transientInstances[typeName].removeOne(value);
                MyCollect(value);
            }
        }

    }

    std::function<void (QObject *instance)> AfterNewInstance;

private:
    void MyCollect(QObject *value){
        if (auto diObj = dynamic_cast<IDIObjBase *>(value))
        {
            diObj->beforeDispose();
        }
        value->deleteLater();
    }

    QHash<QString, QMetaObject> _metaObjects;
    QHash<QString, QHash<QString, QVariant> > _objects;

    QHash<QString, QObject*> _singleInstances;
    QHash<QString, QList<QObject*>> _transientInstances;
    QHash<QObject*, QString> _instancesType;
    QStringList _singleTypeMap;
};

DIContainer::DIContainer(QObject *parent) :
    QObject(parent),
    _d(QSharedPointer<P>(new P))
{
    _d->AfterNewInstance = [this](QObject* instance) {
        if (auto diObj = dynamic_cast<IDIObjBase*>(instance)){
            diObj->setIoc(this);
        }
    };
}

DIContainer::~DIContainer()
{
}

QObject *DIContainer::ResolveMetaobject(QMetaObject metaObject)
{
    return _d->ResolveByName(metaObject.className());
}

void DIContainer::ClassBind(const QMetaObject &resolvableTypeMeta, const QMetaObject &typeMeta, bool isSingletonInstance)
{
    _d->RegisterMetaObject(resolvableTypeMeta.className(), typeMeta, isSingletonInstance);
}

void DIContainer::ClassBind(const QMetaObject &typeMeta, bool isSingletonInstance)
{
    _d->RegisterMetaObject(typeMeta.className(), typeMeta, isSingletonInstance);
}

void DIContainer::ValueBind(const QString &key, const QVariant &value)
{
    _d->RegisterValue(value.typeName(), key, value);
}

void DIContainer::Collect0(QObject *value)
{
    _d->Collect(value);
}

void DIContainer::Collects0(size_t size, ...)
{
    va_list myArgs;
    va_start(myArgs, size);
    for (auto i = 0; i < size; i++)
    {
        auto value = va_arg(myArgs, QObject*);
        Collect0(value);
    }
    va_end(myArgs);
}

QObject *DIContainer::Find0(QMetaObject metaObject)
{
    return _d->Find(QString::fromLatin1(metaObject.className()));
}
