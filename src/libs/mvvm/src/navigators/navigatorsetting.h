#ifndef NAVIGATORSETTING_H
#define NAVIGATORSETTING_H

#include "ioc.h"
#include "../mvvm_global.h"

namespace Mvvms {
//导航传递的参数的接口（设置）
class MVVM_EXPORT INavigatorSetting : public IDIObj {
    Q_OBJECT
        public:
                 explicit INavigatorSetting(QObject *parent = 0);
    QWidget *parent;
};

//导航传递的参数的默认实现（设置）
typedef QSharedPointer<INavigatorSetting> INavigatorSettingPtr;

class MVVM_EXPORT DefaultNavigatorSetting : public INavigatorSetting {
    Q_OBJECT
        public:
                 Q_INVOKABLE explicit DefaultNavigatorSetting(QObject *parent = 0);
};
}

Q_DECLARE_METATYPE(Mvvms::INavigatorSettingPtr)
#endif // NAVIGATORSETTING_H
