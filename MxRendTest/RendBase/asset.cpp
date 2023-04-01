
#include "asset.h"
#include "app.h"

namespace mxr
{
	IAsset::IAsset() : id(0)
	{
		//QOpenGLContext *_context = Application::GetInstance()._context;
		//QSurface *_surface = _context->surface();
		//_context->makeCurrent(_surface);
		//QOpenGLContext *_context = QOpenGLContext::currentContext();
		//if (_context == nullptr)
		//{
		//	return;
		//}
		initializeOpenGLFunctions();
		//CORE_ASERT(Application::GLContextActive(), "OpenGL context not found: {0}", utils::func_sig());
	}

	IAsset::IAsset(IAsset&& other) noexcept : id{ std::exchange(other.id, 0) } {}

	IAsset& IAsset::operator=(IAsset&& other) noexcept
	{
		if (this != &other)
		{
			this->id = std::exchange(other.id, 0);
		}
		return *this;
	}

}


