#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <QObject>
#include <QMetaType>
#include <QSharedPointer>
#include <QVariant>
#include <typeinfo>
#include <QDebug>
#include "ioc.h"
#include "../connector/connector.h"
#include "navigatorsetting.h"

#include "../views/mvvmview.h"
#include "../viewmodels/viewmodel.h"
#include "mvvmviewcontainer.h"
#include "../mvvm_global.h"

namespace Mvvms {
    //导航接口
    class MVVM_EXPORT INavigator : public IDIObj {
        Q_OBJECT
            public:
        explicit INavigator(
                    Connector::IConnectorContainer *connector,
                    Mvvms::IMvvmViewContainer *viewContainer,
                    QObject *parent = 0);
                ~INavigator();

        template<typename TViewModel>
        void Navigate(QWidget *parent = nullptr) {
            Navigate<TViewModel>(getSetting(parent));
        }

        template<typename TViewModel, typename TParam>
        void Navigate(TParam param, QWidget *parent = nullptr) {
            Navigate<TViewModel, TParam>(param, getSetting(parent));
        }

        template<typename TViewModel, typename TResult>
        TResult Navigate(QWidget *parent = nullptr) {
            return Navigate<TViewModel, TResult>(getSetting(parent));
        }

        template<typename TViewModel, typename TParam, typename TResult>
        TResult Navigate(TParam param, QWidget *parent = nullptr) {
            return Navigate<TViewModel, TParam, TResult>(param, getSetting(parent));
        }

        template<typename TViewModel>
        void Navigate(Mvvms::INavigatorSetting *setting) {
            auto viewModel = ioc()->Resolve<TViewModel>();
            Execute(viewModel, setting);
        }

        template<typename TViewModel, typename TParam>
        void Navigate(TParam param, Mvvms::INavigatorSetting *setting) {
            auto viewModel = ioc()->Resolve<TViewModel>();
            auto paramVar = QVariant::fromValue(param);
            viewModel->prepareParam(paramVar);
            Execute(viewModel, setting);
        }

        template<typename TViewModel, typename TResult>
        void Navigate(Mvvms::INavigatorSetting *setting) {
            auto viewModel = ioc()->Resolve<TViewModel>();
            Execute(viewModel, setting);
            QVariant result = viewModel->result;
            return result.value<TResult>();
        }

        template<typename TViewModel, typename TParam, typename TResult>
        TResult Navigate(TParam param, Mvvms::INavigatorSetting *setting) {
            auto viewModel = ioc()->Resolve<TViewModel>();
            auto paramVar = QVariant::fromValue(param);
            viewModel->prepareParam(paramVar);
            Execute(viewModel, setting);
            QVariant result = viewModel->result;
            return result.value<TResult>();
        }

        template<typename TViewModel>
        TViewModel *Call(QWidget *parent = nullptr) {
            return Call<TViewModel>(getSetting(parent));
        }

        template<typename TViewModel, typename TParam>
        TViewModel *Call(TParam param, QWidget *parent = nullptr) {
            return Call<TViewModel, TParam>(param, getSetting(parent));
        }

        template<typename TViewModel>
        TViewModel *Call(Mvvms::INavigatorSetting *setting) {
            auto viewModel = ioc()->Resolve<TViewModel>();
            Execute(viewModel, setting, false);
            return viewModel;
        }

        template<typename TViewModel, typename TParam>
        TViewModel *Call(TParam param, Mvvms::INavigatorSetting *setting) {
            auto viewModel = ioc()->Resolve<TViewModel>();
            auto paramVar = QVariant::fromValue(param);
            viewModel->prepareParam(paramVar);
            Execute(viewModel, setting, false);
            return viewModel;
        }

        template<typename TViewModel, typename TView>
        void Bind(std::function<QObject*(Mvvms::INavigatorSetting *setting)> createViewFunc) {
            _viewContainer->bind<TViewModel, TView>(createViewFunc, true);
        }

        template<typename TViewModel, typename TView>
        void BindTransient(std::function<QObject*(Mvvms::INavigatorSetting *setting)> createViewFunc) {
            _viewContainer->bind<TViewModel, TView>(createViewFunc, false);
        }

        template<typename TViewModel>
        QObject *FindView(){
            return _viewContainer->findView<TViewModel>();
        }

        template<typename TViewModel>
        TViewModel *FindViewModel(){
            return _viewContainer->findViewModel<TViewModel>();
        }

        void Collect(QObject *viewModelObj);

        IConnectorContainer *connector() const;

    protected:

        template<typename TViewModel>
        void Execute(TViewModel *viewModel, Mvvms::INavigatorSetting *setting, bool isNavigate = true) {
            _connector->Add(viewModel);
            auto viewObj = _viewContainer->callView<TViewModel>(viewModel, setting);

            if (viewObj == nullptr)
                return ;

            auto viewVar = QVariant::fromValue(viewObj);
            viewModel->setView(viewVar);
            if(auto mvvmView = dynamic_cast<MvvmView*>(viewObj)){
                mvvmView->LoadViewModel(viewModel, setting);
            }
        }

        Mvvms::INavigatorSetting *getSetting(QWidget *parent);

    private:

        IConnectorContainer *_connector;
        IMvvmViewContainer *_viewContainer;
    };

    typedef QSharedPointer<INavigator> NavigatorPtr;

    //导航的默认实现
    class MVVM_EXPORT DefaultNavigator : public INavigator {
        Q_OBJECT
    public:
        Q_INVOKABLE explicit DefaultNavigator(
            Connector::IConnectorContainer *connector,
            Mvvms::IMvvmViewContainer *viewContainer,
            QObject *parent = 0);
    };

}

Q_DECLARE_METATYPE(Mvvms::NavigatorPtr)

#endif // NAVIGATOR_H
