#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")
#include "mBaseRender.h"
#include "app.h"
//#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <QObject>

namespace mxr
{
	class Shader;
	class Texture;
	class Uniform;
	class Drawable;
	class StateSet;
	class Group;
	class Texture;
}
namespace MViewBasic
{
	class mModelView;
	class mViewBase;
}
using namespace MViewBasic;
namespace MPostRend
{
	class mPostOneFrameRender;
	class RENDVIEW_EXPORT mPostAnimationRender
	{
	public:

		mPostAnimationRender(std::shared_ptr<mxr::Group> parent);

		~mPostAnimationRender();

	private:
		std::shared_ptr<mxr::Group> _parent;

		QHash<int, std::shared_ptr<mPostOneFrameRender>> _allFrameRender;

		int _animationID;


	};
}
