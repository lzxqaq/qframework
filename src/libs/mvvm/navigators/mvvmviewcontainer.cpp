#include "mvvmviewcontainer.h"

Mvvms::IMvvmViewPair::IMvvmViewPair(QObject *parent)
{

}

Mvvms::IMvvmViewPair::~IMvvmViewPair()
{
    createView = nullptr;
    qDeleteAll(_viewObjects);
}

QObject *Mvvms::IMvvmViewPair::getView()
{
    return _viewObjects.count() > 0 ? _viewObjects.at(0) : nullptr;
}

QObject *Mvvms::IMvvmViewPair::newView(INavigatorSetting *setting)
{
    if(isSingle && _viewObjects.count() > 0) {//单例
        return _viewObjects[0];
    }
    if(createView != nullptr)
    {
        auto viewObj = createView(setting);
        _viewObjects.append(viewObj);
        connect(viewObj, &QObject::destroyed, this, &IMvvmViewPair::onViewDestroyed);
        return viewObj;
    }
    return nullptr;
}

void Mvvms::IMvvmViewPair::collectView(QObject *view)
{
    if(isSingle) return;

    if(_viewObjects.contains(view)){
        view->deleteLater();
    }
}

void Mvvms::IMvvmViewPair::onViewDestroyed(QObject *obj)
{
    if(obj != nullptr)
        _viewObjects.removeOne(obj);
}

Mvvms::IMvvmViewContainer::IMvvmViewContainer(QObject *parent)
    : IDIObj(parent)
{

}

Mvvms::IMvvmViewContainer::~IMvvmViewContainer()
{
    _viewModelNames.clear();

    while(_viewModelViewMap.count() != 0){
        auto key = _viewModelViewMap.constBegin().key();
        auto value = _viewModelViewMap.constBegin().value();
        _viewModelViewMap.remove(key);
        value->deleteLater();
    }
    _viewModelViewMap.clear();
}

bool Mvvms::IMvvmViewContainer::collectViewModel(QObject *viewModelObj)
{
    if(viewModelObj == nullptr || !_viewModelNames.contains(viewModelObj)) return false;
    auto viewModelKey = _viewModelNames[viewModelObj];
    if(!_viewModelViewMap.contains(viewModelKey)) return false;
    if(auto viewModel = dynamic_cast<ViewModelBase *>(viewModelObj)){
        _viewModelViewMap[viewModelKey]->collectView(viewModel->view().value<QObject*>());
        ioc()->Collects(viewModel);
        return true;
    }
    return false;
}

Mvvms::DefaultMvvmViewPair::DefaultMvvmViewPair(QObject *parent)
    : IMvvmViewPair(parent)
{

}

Mvvms::DefaultMvvmViewPair::~DefaultMvvmViewPair()
{

}

Mvvms::DefaultMvvmViewContainer::DefaultMvvmViewContainer(QObject *parent)
    : IMvvmViewContainer(parent)
{

}

Mvvms::DefaultMvvmViewContainer::~DefaultMvvmViewContainer()
{

}
