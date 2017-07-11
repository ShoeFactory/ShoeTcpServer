#ifndef SHOEMODELLIBRARY_GLOBAL_H
#define SHOEMODELLIBRARY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SHOEMODELLIBRARY_LIBRARY)
#  define SHOEMODELLIBRARYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SHOEMODELLIBRARYSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SHOEMODELLIBRARY_GLOBAL_H
