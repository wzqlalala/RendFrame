#include "mBaseRender.h"
#include "mBaseRend.h"

namespace MBaseRend
{
	mBaseRender::mBaseRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mBaseRend* baseRend):_app(app),_parent(parent),_baseRend(baseRend)
	{
	
	}

	mBaseRender::~mBaseRender()
	{
	}

}