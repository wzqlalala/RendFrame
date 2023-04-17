#pragma once
//解决中文乱码
#pragma execution_character_set("utf-8")

#include <memory>

#include "app.h"

#include "mBaseRender.h"

namespace mxr
{
	class Drawable;
	class StateSet;
	class Texture;
	class Group;
	class Uniform;
	class Shader;
}
namespace MViewBasic
{
	class mCommonView;
	class mModelView;
}
using namespace MViewBasic;
namespace MBaseRend
{
	class mFontRender;
	class mGlobalAxisRender :public mBaseRender
	{
	public:
		mGlobalAxisRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, std::shared_ptr<mFontRender> fontRender);

		~mGlobalAxisRender();

		void updateUniform(std::shared_ptr<mModelView> modelView, std::shared_ptr<mCommonView> commonView) override;

	protected:

		static void InitialAxisData();

	protected:

		//VBO顶点数据
		static QVector<float> _VBOGlobalAxisData_Type;  //类型 0代表X轴 1为Y轴 2为Z轴
		static QVector<QVector3D>   _VBOGlobalAxisData_Vertex;
		//EBO索引数据
		static QVector<GLuint> _EBOGlobalAxisData;

		std::shared_ptr<mxr::Drawable> _drawable;
		std::shared_ptr<mxr::StateSet> _state;
		std::shared_ptr<mFontRender> _fontRender;
	};
}

