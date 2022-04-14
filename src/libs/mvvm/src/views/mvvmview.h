#ifndef MVVMVIEW_H
#define MVVMVIEW_H

#include "mvvmviewbase.h"

namespace Mvvms {
    class MVVM_EXPORT MvvmView {
    public:
        explicit MvvmView();
        ~MvvmView();

        bool LoadViewModel(QObject *viewModel, INavigatorSetting *setting);
        void ShowView();
        QObject* viewModelObject();

    protected:
        virtual bool loadViewModel(QObject* viewModel, INavigatorSetting *setting) = 0;
        virtual void showView();

        QObject *_viewModelObject;
        INavigatorSetting *_setting = nullptr;
    };

}
#endif // MVVMVIEW_H
