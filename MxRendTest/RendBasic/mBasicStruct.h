#pragma once
#include"mMeshViewEnum.h"
#include <QObject>
#include"qmap.h"
#include"qvector3d.h"
#include <QVector2D>
#include"qpair.h"
namespace MViewBasic
{

	/*
	 * wzq文字结构体
	 */
	struct FontText
	{
		QVector2D pos;
		QString value;
		bool visual;
		FontText()
		{
			pos = QVector2D(0, 0);
			value = "";
			visual = true;
		}
	};
	//通用文字结构体
	struct Font_Common
	{
		int ID;
		QMap<QVector<float>, QString> Pos_Txt; //模型坐标-文本绑定
		QVector3D Color;
		float Size;
	};
	//固定文字结构体
	struct Font_Fixed
	{
		int ID;
		QMap<QVector<float>, QString> Pos_Txt; //模型坐标-文本绑定
		QVector3D Color;
		float Size;
	};
	//局部坐标轴文字结构体(箭头顶端文字)
	struct Font_LocalAxis
	{
		int ID;
		QMap<QVector<float>, QString> Pos_Txt; //模型坐标-文本绑定
		QVector3D NormVec;//方向向量（箭头矢量） 用于坐标转换
	};
	//载荷文字结构体(箭头顶端文字)
	struct Font_Load
	{
		int ID;
		QMap<QVector<float>, QPair<QString, QVector3D>> Pos_Txt_Normvec; //模型坐标-(文本绑定,法向量）
		QVector3D Color;	//颜色RGB
	}typedef Font_Vector, Font_RigidWall;

	//初始条件文字结构体
	struct Font_InitialCondition
	{
		int ID;
		QVector<QPair<QVector<float>, QPair<QString, QVector3D>>> Pos_Txt_Normvec; //模型坐标-(文本绑定,法向量）
		QVector3D Color;	//颜色RGB
	};
	//边界条件文字
	struct Font_BC
	{
		int ID;
		QMap<QVector<float>, QString> Pos_Txt; //模型坐标-文本绑定
		QVector3D Color;	//颜色RGB
		QVector3D NormVec;//方向向量（箭头矢量） 用于坐标转换
	};
	//连接文字
	struct Font_Connect
	{
		int ID;
		QMap<QVector<float>, QString> Pos_Txt; //模型坐标-文本绑定
		QVector3D Color;	//颜色RGB
	};

	//单箭头
	struct Arrow
	{
		int ID;
		QVector<QVector3D> Pos;//坐标
		QVector3D Direction; //方向
		QVector3D Color;//颜色RGB
		float Size; //大小
		bool isDepth; //是否深度测试
	};
}
