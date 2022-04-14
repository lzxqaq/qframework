#include "viewmodel.h"

using namespace Mvvms;


ViewModelBase::ViewModelBase(QObject *parent)
    : IDIObj(parent)
{

}

void ViewModelBase::prepareParam(QVariant &param)
{
    prepare(param);
    _param = param;
}

void ViewModelBase::afterLoadViewModel()
{

}

QVariant ViewModelBase::view()
{
    return _view;
}

void ViewModelBase::setView(QVariant &value)
{
    _view = value;
}

void ViewModelBase::prepare(QVariant &param)
{
    Q_UNUSED(param)
}
