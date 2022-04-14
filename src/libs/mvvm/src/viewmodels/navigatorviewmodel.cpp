#include "navigatorviewmodel.h"
using namespace Mvvms;

NavigatorViewModelBase::NavigatorViewModelBase(Mvvms::INavigator *navigator, QObject *parent)
    : ViewModelBase(parent)
{
    NavigatorViewModelBase::navigator = navigator;
}

NavigatorViewModelBase::~NavigatorViewModelBase()
{
    navigator->Collect(this);
}
