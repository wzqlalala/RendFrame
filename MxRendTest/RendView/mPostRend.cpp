#include "mPostRend.h"
#include "mPostRender.h"

////MxRender
#include <renderpch.h>

namespace MPostRend
{

	mPostRend::mPostRend(const QString& name):mBaseRend( name)
	{

		//_root = MakeAsset<mxr::Group>();

		//QSurfaceFormat format;
		//format.setMajorVersion(4);
		//format.setMinorVersion(5);
		//format.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);
		//format.setSamples(4);
		//setFormat(format);

		qDebug() << "Post Struct";
	}

	void mPostRend::initializeGL()
	{
		mBaseRend::initializeGL();

		//QOpenGLContext *context = QOpenGLContext::currentContext();
		QOpenGLContext *context = _app->GLContext();
		context->makeCurrent(context->surface());

		shared_ptr<mPostRender> postRender = make_shared<mPostRender>(_app, _root, this);
		this->addRender(postRender);
		//glEnable(GL_FRAMEBUFFER_SRGB);

		//this->initializeOpenGLFunctions();

		/*mxr::Application::GetInstance()._context = context();

		glEnable(GL_POINT_SPRITE);		//开启渲染点精灵功能
		glEnable(GL_PROGRAM_POINT_SIZE); //让顶点程序决定点块大小
		glEnable(GL_DEPTH_TEST);

		mxr::Log::Init();
		_viewer = MakeAsset<mxr::Viewer>();
		_viewer->setSceneData(_root);

		_bgRend = new mBackGroundRender(_root);*/

		//glEnable(GL_DEPTH_TEST);
		//glLineWidth(5.0);

		//_app->GLContext()->functions()->glEnable(GL_POINT_SPRITE);		//开启渲染点精灵功能
		//_app->GLContext()->functions()->glEnable(GL_PROGRAM_POINT_SIZE); //让顶点程序决定点块大小

		//glEnable(GL_POINT_SPRITE);		//开启渲染点精灵功能
		//glEnable(GL_PROGRAM_POINT_SIZE); //让顶点程序决定点块大小
		qDebug() << "Post Initial";
	}

	void mPostRend::paintGL()
	{
		//glLineWidth(5.0);
		mBaseRend::paintGL();
		GLenum error = QOpenGLContext::currentContext()->functions()->glGetError();
		if (error != 0)
		{
			qDebug() << error;
		}

	}

	void mPostRend::resizeGL(int w, int h)
	{
		mBaseRend::resizeGL(w, h);
	}
	void mPostRend::mousePressEvent(QMouseEvent *event)
	{
		mBaseRend::mousePressEvent(event);
	}
	void mPostRend::mouseReleaseEvent(QMouseEvent *event)
	{
		mBaseRend::mouseReleaseEvent(event);
	}
	void mPostRend::mouseMoveEvent(QMouseEvent *event)
	{
		mBaseRend::mouseMoveEvent(event);
	}
	void mPostRend::wheelEvent(QWheelEvent *event)
	{
		mBaseRend::wheelEvent(event);
	}

	mPostRend::~mPostRend()
	{

		qDebug() << "Post Distruct";
	}
	shared_ptr<mPostRender> mPostRend::getPostRender()
	{
		shared_ptr<mBaseRender> baseRender = getFirstRender();
		if (baseRender)
		{
			return dynamic_pointer_cast<mPostRender>(baseRender);
		}
		return nullptr;
	}
}