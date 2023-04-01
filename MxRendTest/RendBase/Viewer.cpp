#include "Viewer.h"
#include "Group.h"

namespace mxr
{
	QTime *time = new QTime;
	Viewer::Viewer()
	{
		_visitor = MakeAsset<NodeVisitor>();
		_clear = MakeAsset<Clear>();
	}

	void Viewer::compile()
	{
		//qDebug() << __LINE__ << time->elapsed();
		//time->start();
		//qDebug() << __LINE__ << time->elapsed();
		_sceneData->accept(_visitor);//20֡�Ķ�������100ms����ͼ����97ms
		GLenum error = QOpenGLContext::currentContext()->functions()->glGetError();
		if (error != 0)
		{
			qDebug() << error;
		}
		//qDebug() << __LINE__ << time->elapsed();
		_visitor->compile();//20֡�Ķ�������43ms����ͼ����23ms
		error = QOpenGLContext::currentContext()->functions()->glGetError();
		if (error != 0)
		{
			qDebug() << error;
		}
		//qDebug() << __LINE__ << time->elapsed();
		_visitor->clear();
		error = QOpenGLContext::currentContext()->functions()->glGetError();
		if (error != 0)
		{
			qDebug() << error;
		}
		//qDebug() << __LINE__ << time->elapsed();
	}

	void Viewer::noClearRun()
	{
		this->compile();
		//����
		_visitor->run();
		GLenum error = QOpenGLContext::currentContext()->functions()->glGetError();
		if (error != 0)
		{
			qDebug() << error;
		}
		//qDebug() << __LINE__ << time->elapsed();
	}

	void Viewer::run()
	{
		this->compile();
		//����
		_clear->apply();
		_visitor->run();
		//qDebug() << __LINE__ << time->elapsed();

	}
	void Viewer::deleteAllData()
	{
		_visitor->RemoveAllData();
	}
}