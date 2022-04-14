#include "navigator.h"
using namespace Mvvms;

INavigator::INavigator(IConnectorContainer *connector, IMvvmViewContainer *viewContainer, QObject *parent)
    :   IDIObj(parent),
    _connector(connector),
    _viewContainer(viewContainer)
{

}

INavigator::~INavigator()
{
    ioc()->Collects(_viewContainer, _connector);
}

void INavigator::Collect(QObject *viewModelObj)
{
    if(_viewContainer->collectViewModel(viewModelObj)){
        _connector->Remove(viewModelObj);
    }
}

IConnectorContainer *INavigator::connector() const
{
    return _connector;
}

INavigatorSetting *INavigator::getSetting(QWidget *parent)
{
    auto setting = ioc()->Resolve<Mvvms::INavigatorSetting>();
    setting->parent = parent;
    return setting;
}

DefaultNavigator::DefaultNavigator(IConnectorContainer *connector, IMvvmViewContainer *viewContainer, QObject *parent)
    :   INavigator(connector, viewContainer, parent)
{

}
