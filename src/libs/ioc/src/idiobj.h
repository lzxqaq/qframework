#ifndef IDIOBJ_H
#define IDIOBJ_H

#include <QObject>
#include "idiobjbase.h"
#include "dicontainer.h"
#include "ioc_global.h"

namespace Ioc {
    //注入屬性接口基类
class IOC_EXPORT IDIObj : public IDIObjBase {
    Q_OBJECT
        public:
    explicit IDIObj(QObject *parent = 0);

    //IOC容器
    DIContainer *ioc();
    void setIoc(QObject *value);

private:
    DIContainer* _ioc = nullptr;
};
}
#endif // IDIOBJ_H
