#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(RENDBASIC_LIB)
#  define RENDBASIC_EXPORT Q_DECL_EXPORT
# else
#  define RENDBASIC_EXPORT Q_DECL_IMPORT
# endif
#else
# define RENDBASIC_EXPORT
#endif
