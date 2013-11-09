#ifndef QLC_GLOBAL_H
#define QLC_GLOBAL_H

#include <qglobal.h>
#if QT_VERSION >= 0x040000
    #define QT4

    #if defined(QLC_LIBRARY)
        #define QLCSHARED_EXPORT Q_DECL_EXPORT
    #else
        #  define QLCSHARED_EXPORT Q_DECL_IMPORT
    #endif
#else
    #define QT3
    #define QLSHARED_EXPORT
    #define QVector QValueVector
#endif

#endif // QLC_GLOBAL_H
