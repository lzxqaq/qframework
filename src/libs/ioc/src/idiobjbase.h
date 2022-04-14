#ifndef IDIOBJBASE_H
#define IDIOBJBASE_H

#include <QObject>
#include "ioc_global.h"

namespace Ioc {
//注入属性接口基类（抽象）
class IOC_EXPORT IDIObjBase : public QObject {
    Q_OBJECT
public:
    explicit IDIObjBase(QObject *parent = 0);
    virtual ~IDIObjBase();
    virtual void beforeDispose();
    virtual void setIoc(QObject *value) = 0;

};
}

#endif // IDIOBJBASE_H
