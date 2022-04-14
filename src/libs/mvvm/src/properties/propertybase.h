#ifndef PROPERTYBASE_H
#define PROPERTYBASE_H

#include <QObject>
#include <QVariant>
#include <QDebug>
#include "../mvvm_global.h"

namespace Mvvms {
    //通用属性模板
    template <typename T>
    class MVVM_EXPORT PropertyBase : public QObject
    {
    public:
        explicit PropertyBase(QObject *parent = 0);
        T value(){return _baseValue;}

    protected:
        void setBaseValue(const T &value) { setAndSend(value); }
        void setAndSend(const T &value) {
            if (!_settingFlag)//正在设置的标志，防止信号循环
            {
                _settingFlag = true;
                _baseValue = value;
                send();
                _settingFlag = false;
            }
        }

        virtual void send() = 0;

    private:
        T _baseValue;
        bool _settingFlag = false;//正在设置的标志，防止信号循环
    };

    //界面互相通知属性 QString
    class MVVM_EXPORT PropertyString : public PropertyBase<QString>
    {
        Q_OBJECT
            public:
                     explicit PropertyString(QObject *parent = 0);
    public Q_SLOTS:
        void setValue(const QString &val);
    Q_SIGNALS:
        void valueChanged(const QString &v);
    protected:
        void send() override;
    };

    //界面互相通知属性 QString
    class MVVM_EXPORT PropertyInt : public PropertyBase<int>
    {
        Q_OBJECT
            public:
                     explicit PropertyInt(QObject *parent = 0);
    public Q_SLOTS:
        void setValue(const int &val);
    Q_SIGNALS:
        void valueChanged(const int &v);
    protected:
        void send() override;
    };

    //界面互相通知属性 QString
    class MVVM_EXPORT PropertyDouble : public PropertyBase<double>
    {
        Q_OBJECT
            public:
                     explicit PropertyDouble(QObject *parent = 0);
    public Q_SLOTS:
        void setValue(const double &val);
    Q_SIGNALS:
        void valueChanged(const double &v);
    protected:
        void send() override;
    };

    //界面互相通知属性 QString
    class MVVM_EXPORT PropertyFloat : public PropertyBase<float>
    {
        Q_OBJECT
            public:
                     explicit PropertyFloat(QObject *parent = 0);
    public Q_SLOTS:
        void setValue(const float &val);
    Q_SIGNALS:
        void valueChanged(const float &v);
    protected:
        void send() override;
    };

    //界面互相通知属性 QString
    class MVVM_EXPORT PropertyBool : public PropertyBase<bool>
    {
        Q_OBJECT
            public:
                     explicit PropertyBool(QObject *parent = 0);
    public Q_SLOTS:
        void setValue(const bool &val);
    Q_SIGNALS:
        void valueChanged(const bool &v);
    protected:
        void send() override;
    };

    //界面互相通知属性 QString
    class MVVM_EXPORT PropertyVariant : public PropertyBase<QVariant>
    {
        Q_OBJECT
    public:
        explicit PropertyVariant(QObject *parent = 0);
    public Q_SLOTS:
        void setValue(const QVariant &val);
    Q_SIGNALS:
        void valueChanged(const QVariant &v);
    protected:
        void send() override;
    };

}

#endif // PROPERTYBASE_H
