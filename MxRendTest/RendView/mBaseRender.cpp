#include "mBaseRender.h"

namespace MBaseRend
{
	mBaseRender::mBaseRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent):_app(app),_parent(parent)
	{

	}

	mBaseRender::~mBaseRender()
	{
	}
}