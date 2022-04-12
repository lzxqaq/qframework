#include "mvvmview.h"
using namespace Mvvms;

MvvmView::MvvmView(){

}

MvvmView::~MvvmView()
{
    if (auto idObj = dynamic_cast<IDIObj *>(_viewModelObject)){
        idObj->ioc()->Collects(_setting, _viewModelObject);
    }
}

bool MvvmView::LoadViewModel(QObject *viewModel, INavigatorSetting *setting)
{
    auto flag = loadViewModel(viewModel, setting);
    _viewModelObject = viewModel;
    _setting = setting;
    return flag;
}

void MvvmView::ShowView()
{
    showView();
}

QObject *MvvmView::viewModelObject()
{
    return _viewModelObject;
}

void MvvmView::showView()
{

}

