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
		time->start();
		qDebug() << __LINE__ << time->elapsed();
		_sceneData->accept(_visitor);//20帧的动画花费100ms，云图花费97ms
		qDebug() << __LINE__ << time->elapsed();
		_visitor->compile();//20帧的动画花费43ms，云图花费23ms
		qDebug() << __LINE__ << time->elapsed();
		_visitor->clear();
		qDebug() << __LINE__ << time->elapsed();
	}

	void Viewer::noClearRun()
	{
		this->compile();
		//绘制
		_visitor->run();
	}

	void Viewer::run()
	{
		this->compile();
		//绘制
		_clear->apply();
		_visitor->run();
		qDebug() << __LINE__ << time->elapsed();

	}
}