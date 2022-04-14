#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <QObject>
#include "ioc.h"
#include "../mvvm_global.h"

namespace Mvvms {

class MVVM_EXPORT ViewModelBase : public IDIObj {
    Q_OBJECT
public:
    explicit ViewModelBase(QObject *parent = 0);
    void prepareParam(QVariant &param);
    virtual void afterLoadViewModel();
    QVariant view();
    void setView(QVariant &value);
protected:
    virtual void prepare(QVariant &param);

    QVariant _view;
    QVariant _param;
};
typedef QSharedPointer<ViewModelBase> ViewModelBasePtr;
}
Q_DECLARE_METATYPE(Mvvms::ViewModelBasePtr);
#endif // VIEWMODEL_H
