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
namespace MPostRend
{
	class RENDVIEW_EXPORT mPostRendStatus
	{

	public:
		//渲染模式
		PostMode _postMode = OneFrame;

		//是否显示变形前的图
		bool _isShowInitialShape{false};

		//节点或者网格
		NodeOrElement _nodeOrElement = PostNode;

		//显示模式
		ShowFuntion _showFunction = ElementFace;

		//拾取模式
		MViewBasic::PickFilter _pickFilter;

		//拾取的单元类型过滤器
		std::set<int> _pickElementTypeFilter;

		//框选还是点选
		MViewBasic::PickSoloOrMutiply _pickSoloOrMutiply;

		//渲染范围（0-全部部件范围（自定义）；1-渲染部件范围）
		int _rangeType = 0;

		//是否离散
		bool _isdispersed = true;

		//是否等差
		bool _isEquivariance = true;

		//是否渲染颜色表
		bool _isShowPostColorTable = false;

		//后处理颜色表文字大小
		float _postColorTableFontSize;

		//后处理颜色表位置行高度比例
		float _postColorTableRatio;

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

		//切平面的参数
		QVector<QVector4D> _cuttingPlanes;

		//是否实时显示平面
		bool _isShowTransparentPlane = true;

		//是否只显示被切的面，不显示模型
		bool _isOnlyShowCuttingPlane = false;

	};
}

