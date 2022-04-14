#ifndef NAVIGATORVIEWMODEL_H
#define NAVIGATORVIEWMODEL_H

#include "../navigators/navigator.h"
#include "viewmodel.h"
#include "../mvvm_global.h"

namespace Mvvms {
class MVVM_EXPORT NavigatorViewModelBase : public ViewModelBase{
    Q_OBJECT
        public:
    explicit NavigatorViewModelBase(Mvvms::INavigator *navigator, QObject *parent = 0);
            ~NavigatorViewModelBase();
    QVariant result;
    INavigator *navigator;
};
typedef QSharedPointer<NavigatorViewModelBase> NavigatorViewModelBasePtr;
}
Q_DECLARE_METATYPE(Mvvms::NavigatorViewModelBasePtr)
#endif // NAVIGATORVIEWMODEL_H
