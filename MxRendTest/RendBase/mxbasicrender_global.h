#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(MXBASICRENDER_LIB)
#  define MXBASICRENDER_EXPORT Q_DECL_EXPORT
# else
#  define MXBASICRENDER_EXPORT Q_DECL_IMPORT
# endif
#else
# define MXBASICRENDER_EXPORT
#endif
