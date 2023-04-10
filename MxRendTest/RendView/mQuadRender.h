#pragma once

#include <QObject>

#include <memory>

#include "app.h"

#include "mMeshViewEnum.h"

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
	class mQuadRender : public QObject
	{
		Q_OBJECT
	public:
		mQuadRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> root);

		~mQuadRender();

		void draw(CameraOperateMode cameraMode,PickMode pickMode, MultiplyPickMode multiplyPickMode, QVector<QVector2D> poses, int w, int h);

	protected:
		void makeCurrent() { _app->GLContext()->makeCurrent(_app->GLContext()->surface()); };

		void doneCurrent() { _app->GLContext()->doneCurrent(); };

	protected:
		std::shared_ptr<mxr::Application> _app;
		std::shared_ptr<mxr::Group> _parent;

		std::shared_ptr<mxr::Drawable> _drawable;
		std::shared_ptr<mxr::StateSet> _stateSet;


	};
}

