#ifndef MVVM_GLOBAL_H
#define MVVM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLUGINMANAGER_LIBRARY)
#  define PLUGINMANAGER_EXPORT Q_DECL_EXPORT
#else
#  define PLUGINMANAGER_EXPORT Q_DECL_IMPORT
#endif

#endif // MVVM_GLOBAL_H
