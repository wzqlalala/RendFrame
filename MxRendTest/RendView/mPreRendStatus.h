#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")

//MViewBasic
#include "mMeshViewEnum.h"

//MBasicFunction
#include "mBasicEnum.h"
#include "mBasicStructor.h"

#include <QVector4D>
#include <QVector>

using namespace MViewBasic;
using namespace MBasicFunction;
namespace MPreRend
{
	class RENDVIEW_EXPORT mPreRendStatus
	{

	public:
		//显示模式
		ShowFuntion _showFunction = ElementFace;

		//拾取模式
		MViewBasic::PickFilter _pickFilter;

		//拾取的单元类型过滤器
		std::set<int> _pickElementTypeFilter;

		//是否开启光照
		bool _lightIsOn = true;

		//光照位置跟随相机位置
		bool _lightIsDependOnCamera = true;

		//光照参数
		PostLight _postLight;

		//点的大小
		int _pointSize = 1;

		//线宽
		float _lineWidth = 1.0;

		//网格面线的颜色
		QVector4D _faceLineColor = QVector4D(0.0f, 0.0f, 0.0f, 1.0f);
	};
}

