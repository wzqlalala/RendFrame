#pragma once

#include <QObject>

#include <memory>

#include "app.h"

#include "mMeshViewEnum.h"

#include "mBaseRender.h"

namespace mxr
{
	class Drawable;
	class Shader;
	class Group;
	class StateSet;
}
using namespace MViewBasic;
namespace MBaseRend
{
	class mQuadRender : public mBaseRender
	{
		Q_OBJECT
	public:
		mQuadRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> root, mBaseRend *baseRend);

		~mQuadRender();

		void updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView) override;

		//void draw(QVector<QVector2D> poses, int w, int h);

	protected:
		//void makeCurrent() { _app->GLContext()->makeCurrent(_app->GLContext()->surface()); };

		//void doneCurrent() { _app->GLContext()->doneCurrent(); };

	protected:
		//std::shared_ptr<mxr::Application> _app;
		//std::shared_ptr<mxr::Group> _parent;

		std::shared_ptr<mxr::Drawable> _drawable;
		std::shared_ptr<mxr::StateSet> _stateSet;

		CameraOperateMode *_cameraMode;//当前相机操作模式
		PickMode *_pickMode;//当前拾取模式
		MultiplyPickMode *_multiplyPickMode;//框选拾取模式


	};
}

