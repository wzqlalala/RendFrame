#pragma once
#include <string>
#include <QHash>
#include "export.h"
#include "app.h"

namespace mxr
{
	class MXR_EXPORT ApplicationInstance
	{
	private:

	private:
		ApplicationInstance() {}
		~ApplicationInstance() {};

	public:
		static ApplicationInstance& GetInstance();
		void appendApplication(QString name, std::shared_ptr<mxr::Application> a)
		{
			//if (_apps.size() >= 1)
			//{
			//	_apps.begin().value()->GLContext()->setShareContext(a->GLContext());
			//}
			_apps[name] = a;
		};
		int removeApplication(QString name) { _apps.remove(name); return _apps.size(); };
		int getApplicationNum() { return _apps.size(); };
		std::shared_ptr<mxr::Application> getApplication(QString name) { return _apps[name]; };
		//QOpenGLContext *GLContext() const { return _context; }
		//QOpenGLContext *_context;
		//QSurface *_surface;

	private:

		QHash<QString, std::shared_ptr<mxr::Application>> _apps;
	};
}


