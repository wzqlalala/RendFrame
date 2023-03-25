#pragma once
#include <string>
#include "export.h"

namespace mxr
{
	class MXR_EXPORT Application
	{
	private:

	public:
		Application() {}
		~Application() {};

	public:
		//Application(Application& application) = delete;
		//void operator=(const Application&) = delete;
		//static Application& GetInstance();
		//void setContext(QOpenGLContext *context) {_context = context;};
		//QOpenGLContext *GLContext() const { return _context; }
		//QOpenGLContext *_context;
		//QSurface *_surface;

	private:
		//QOpenGLContext *_context;
		//QHash<QString, QOpenGLContext*> _contexts;
	};
}


