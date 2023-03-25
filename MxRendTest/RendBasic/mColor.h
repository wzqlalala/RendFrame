#pragma once

#include "rendbasic_global.h"

//Qt
#include <QList>
#include <QVector4D>

namespace MBasicFunction
{
	class RENDBASIC_EXPORT mColor
	{
	public:
		/*
		* 获取单例指针
		*/
		static mColor* getInstance();

		/*
		 * 默认构造函数
		 */
		mColor();

		/*
		* 析构函数
		*/
		~mColor();

		/*
		 * 通过部件ID来获取颜色
		 */
		QVector4D getPartColor(int partID);

		/*
		 * 获取节点的颜色
		 */
		QVector4D getNodeColor();

	private:
		//单例
		static mColor *_instance;

		//模型颜色的列表，根据partID来选颜色
		QList<QVector4D> _partColor;

		//节点的颜色
		QVector4D _nodeColor;
	};
}
