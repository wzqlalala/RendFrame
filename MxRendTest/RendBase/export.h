#pragma once

#include <QtCore/qglobal.h>
#include <qopenglfunctions.h>
#include <qopenglcontext.h>
#include <qopenglfunctions_4_5_core.h>
#include <qopenglfunctions_4_5_compatibility.h>
#include <qopenglextrafunctions.h>
#include <qopenglwindow.h>

#ifndef BUILD_STATIC
# if defined(MXR_EXPORT)
#  define MXR_EXPORT Q_DECL_EXPORT
# else
#  define MXR_EXPORT Q_DECL_IMPORT
# endif
#else
# define MXR_EXPORT
#endif

