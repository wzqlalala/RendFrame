#include "mPreRend.h"

////MxRender
#include <renderpch.h>

namespace MPreRend
{

	mPreRend::mPreRend(const QString& name):mBaseRend( name)
	{

		//_root = MakeAsset<mxr::Group>();

		//QSurfaceFormat format;
		//format.setMajorVersion(4);
		//format.setMinorVersion(5);
		//format.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);
		//format.setSamples(4);
		//setFormat(format);

		qDebug() << "Pre Struct";
	}

	void mPreRend::initializeGL()
	{
		mBaseRend::initializeGL();

		//this->initializeOpenGLFunctions();

		/*mxr::Application::GetInstance()._context = context();



		mxr::Log::Init();
		_viewer = MakeAsset<mxr::Viewer>();
		_viewer->setSceneData(_root);

		_bgRend = new mBackGroundRender(_root);*/

		//glEnable(GL_DEPTH_TEST);

		qDebug() << "Pre Initial";
	}

	void mPreRend::paintGL()
	{
		mBaseRend::paintGL();
	}

	void mPreRend::resizeGL(int w, int h)
	{
		mBaseRend::resizeGL(w, h);
	}
	void mPreRend::mousePressEvent(QMouseEvent *event)
	{
		mBaseRend::mousePressEvent(event);
	}
	void mPreRend::mouseReleaseEvent(QMouseEvent *event)
	{
		mBaseRend::mouseReleaseEvent(event);
	}
	void mPreRend::mouseMoveEvent(QMouseEvent *event)
	{
		mBaseRend::mouseMoveEvent(event);
	}
	void mPreRend::wheelEvent(QWheelEvent *event)
	{
		mBaseRend::wheelEvent(event);
	}

	mPreRend::~mPreRend()
	{

		qDebug() << "Pre Distruct";
	}
}