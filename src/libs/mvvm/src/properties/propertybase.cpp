#include "propertybase.h"

template<typename T>
Mvvms::PropertyBase<T>::PropertyBase(QObject *parent)
    :   QObject(parent)
{

}

Mvvms::PropertyString::PropertyString(QObject *parent)
    :   PropertyBase<QString>(parent)
{

}

void Mvvms::PropertyString::setValue(const QString &val)
{
    setBaseValue(val);
}

void Mvvms::PropertyString::send()
{
    emit valueChanged(value());
}

Mvvms::PropertyInt::PropertyInt(QObject *parent)
    :   PropertyBase<int>(parent)
{

}

void Mvvms::PropertyInt::setValue(const int &val)
{
    setBaseValue(val);
}

void Mvvms::PropertyInt::send()
{
    emit valueChanged(value());
}

Mvvms::PropertyDouble::PropertyDouble(QObject *parent)
    :   PropertyBase<double>(parent)
{

}

void Mvvms::PropertyDouble::setValue(const double &val)
{
    setBaseValue(val);
}

void Mvvms::PropertyDouble::send()
{
    emit valueChanged(value());
}

Mvvms::PropertyFloat::PropertyFloat(QObject *parent)
    :   PropertyBase<float>(parent)
{

}

void Mvvms::PropertyFloat::setValue(const float &val)
{
    setBaseValue(val);
}

void Mvvms::PropertyFloat::send()
{
    emit valueChanged(value());
}

Mvvms::PropertyBool::PropertyBool(QObject *parent)
    :   PropertyBase<bool>(parent)
{

}

void Mvvms::PropertyBool::setValue(const bool &val)
{
    setBaseValue(val);
}

void Mvvms::PropertyBool::send()
{
    emit valueChanged(value());
}

Mvvms::PropertyVariant::PropertyVariant(QObject *parent)
    :   PropertyBase<QVariant>(parent)
{

}

void Mvvms::PropertyVariant::setValue(const QVariant &val)
{
    setBaseValue(val);
}

void Mvvms::PropertyVariant::send()
{
    emit valueChanged(value());
}
