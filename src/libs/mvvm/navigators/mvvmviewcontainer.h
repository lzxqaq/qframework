#ifndef MVVMVIEWCONTAINER_H
#define MVVMVIEWCONTAINER_H

#include "navigatorsetting.h"
#include "../views/mvvmview.h"
#include "../viewmodels/viewmodel.h"
#include "../mvvm_global.h"

namespace Mvvms {

    class MVVM_EXPORT IMvvmViewPair : public IDIObj
    {
        Q_OBJECT
    public:
        explicit IMvvmViewPair(QObject *parent = 0);
        ~IMvvmViewPair();

        QObject *getView();
        QObject *newView(Mvvms::INavigatorSetting *setting);
        void collectView(QObject *view);

        QString key;//这个是用于调试使用的查看该集合的类型
        bool isSingle = true;//是否单例
        std::function<QObject*(Mvvms::INavigatorSetting *setting)> createView = nullptr;//创建 view 的委托

    private:
        void onViewDestroyed(QObject *obj = nullptr);
        QList<QObject *> _viewObjects;//view 对象集合
    };

    class MVVM_EXPORT IMvvmViewContainer : public IDIObj {
        Q_OBJECT
            public:
        explicit IMvvmViewContainer(QObject *parent = 0);
                ~IMvvmViewContainer();
        bool collectViewModel(QObject *viewModelObj);

        template<typename TViewModel>
        QObject *findView() {
                    auto viewModelKey = QString::fromLatin1(static_cast<TViewModel*>(0)->staticMetaObject.className());
                    if(!_viewModelViewMap.contains(viewModelKey)) return nullptr;
                    return _viewModelViewMap[viewModelKey].getView();
        }

        template<typename TViewModel>
        TViewModel* findViewModel() {
            return ioc()->Find<TViewModel>();
        }

        template<typename TViewModel>
        QObject* callView(QObject* viewModel, Mvvms::INavigatorSetting *setting)
        {
            _viewModelNames[viewModel] = QString::fromLatin1(viewModel->metaObject()->className());

            auto viewModelKey = QString::fromLatin1(static_cast<TViewModel*>(0)->staticMetaObject.className());

            if(!_viewModelViewMap.contains(viewModelKey)) {
                qDebug() <<"ViewModelKey not exist";
                return nullptr;
            }

            return _viewModelViewMap[viewModelKey]->newView(setting);
        }

        template<typename TViewModel, typename TView>
        void bind(std::function<QObject*(Mvvms::INavigatorSetting *setting)> createViewFunc, bool isSingle) {
            auto viewModelKey = QString::fromLatin1(static_cast<TViewModel*>(0)->staticMetaObject.className());
            auto pair = ioc()->Resolve<IMvvmViewPair>();
            pair->isSingle = isSingle;
            pair->key = viewModelKey;
            pair->createView = createViewFunc;

            _viewModelViewMap[viewModelKey] = pair;

            //将 viewmodel 加到 IOC 容器里
            if(isSingle){
                this->ioc()->Bind<TViewModel>();
            }else{
                this->ioc()->BindTransient<TViewModel>();
            }
        }

    private:
        QHash<QString, IMvvmViewPair *> _viewModelViewMap;
        QHash<QObject*, QString> _viewModelNames;
    };

    class MVVM_EXPORT DefaultMvvmViewPair : public IMvvmViewPair
    {
        Q_OBJECT
    public:
        Q_INVOKABLE explicit DefaultMvvmViewPair(QObject *parent = 0);
        ~DefaultMvvmViewPair();
    };

    class MVVM_EXPORT DefaultMvvmViewContainer : public IMvvmViewContainer{
        Q_OBJECT
            public:
        Q_INVOKABLE explicit DefaultMvvmViewContainer(QObject *parent = 0);
        ~DefaultMvvmViewContainer();
    };
}

#endif // MVVMVIEWCONTAINER_H
