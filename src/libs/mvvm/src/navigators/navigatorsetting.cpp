#include "navigatorsetting.h"
using namespace Mvvms;

INavigatorSetting::INavigatorSetting(QObject *parent)
    :   IDIObj(parent)
{

}

DefaultNavigatorSetting::DefaultNavigatorSetting(QObject *parent)
    :   INavigatorSetting(parent)
{

}
