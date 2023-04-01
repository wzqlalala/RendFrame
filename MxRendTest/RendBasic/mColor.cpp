#include "mColor.h"

//QVector4D
#include <QVector4D>

namespace MBasicFunction
{
	mColor* mColor::_instance = nullptr;

	mColor* mColor::getInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new mColor;
		}
		return _instance;
	}

	mColor::mColor()
	{
		//初始化部件颜色列表
		_partColor = QList<QVector4D>{ QVector4D(1.00f, 0.00f, 0.00f, 1.00f),
									   QVector4D(0.00f, 0.00f, 1.00f, 1.00f),
									  QVector4D(0.65f, 0.59f, 0.0f, 1.00f),
									  QVector4D(0.84f, 0.19f, 0.46f, 1.00f),
									  QVector4D(0.57f, 0.43f, 0.85f, 1.00f),
									  QVector4D(0.91f, 0.67f, 0.65f, 1.00f),
									  QVector4D(0.50f, 0.24f, 0.12f, 1.00f),
									  QVector4D(0.05f, 0.6f, 0.3f, 1.00f) };

		//初始化节点颜色
		_nodeColor = QVector4D{ 0.20f, 1.00f, 0.20f, 1.00f };


	}

	mColor::~mColor()
	{

	}

	QVector4D mColor::getPartColor(int partID)
	{
		//通过部件的ID对颜色列表的大小取余数，然后获取出颜色值
		int num = (partID-1) % _partColor.size();
		return _partColor.at(num);
	}

	QVector4D mColor::getNodeColor()
	{
		return _nodeColor;
	}
}
