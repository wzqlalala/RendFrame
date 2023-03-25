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
	class mCommonView;
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

		std::shared_ptr<mxr::Drawable> _drawable; //设置顶点属性；设置StateSet；将其添加进父节点中;
		std::shared_ptr<mxr::StateSet> _stateSet;//设置着色器Shader；设置渲染状态详见StateAttribute；设置纹理Texture；设置渲染模式；设置uniform值；

		shared_ptr<mxr::Texture> _diffuseMap;
		shared_ptr<mxr::Texture> _normalMap;
	};
}
