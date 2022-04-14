#include "idiobj.h"

using namespace Ioc;


IDIObj::IDIObj(QObject *parent)
    : IDIObjBase(parent)
{

}

DIContainer *IDIObj::ioc()
{
    return _ioc;
}

void IDIObj::setIoc(QObject *value)
{
    _ioc = (DIContainer*)value;
}
