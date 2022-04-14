#ifndef MVVM_GLOBAL_H
#define MVVM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MVVM_LIBRARY)
#  define MVVM_EXPORT Q_DECL_EXPORT
#else
#  define MVVM_EXPORT Q_DECL_IMPORT
#endif

#endif // MVVM_GLOBAL_H
