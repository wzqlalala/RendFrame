#include "mPostRend.h"

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

		//glEnable(GL_FRAMEBUFFER_SRGB);

		//this->initializeOpenGLFunctions();

		/*mxr::Application::GetInstance()._context = context();



		mxr::Log::Init();
		_viewer = MakeAsset<mxr::Viewer>();
		_viewer->setSceneData(_root);

		_bgRend = new mBackGroundRender(_root);*/

		//glEnable(GL_DEPTH_TEST);
		//glLineWidth(5.0);
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
}