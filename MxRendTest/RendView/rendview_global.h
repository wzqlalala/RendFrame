#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(RENDVIEW_LIB)
#  define RENDVIEW_EXPORT Q_DECL_EXPORT
# else
#  define RENDVIEW_EXPORT Q_DECL_IMPORT
# endif
#else
# define RENDVIEW_EXPORT
#endif
