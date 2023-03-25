#pragma once
#include "rendbasic_global.h"

#include <QObject>
#include<qvector3d.h>
#include<qmatrix4x4.h>
#include<qvector2d.h>

namespace MViewBasic
{
	
	class RENDBASIC_EXPORT mViewToolClass : public QObject
	{
		Q_OBJECT

	public:
		mViewToolClass();
		~mViewToolClass();

		//工具函数
		static QVector3D GetGeoCenterPosition(QVector<QVector3D> vertex);				//获取旋转中心
		static float GetMaxRadius(QVector<QVector3D> vertex, QVector3D center);		//找最大的旋转半径
		static float GetMaxRadius(float left, float right, float bottom, float top, float back, float front, QVector3D center);//已知新的旋转中心和模型的最大尺寸，求旋转半径
		static float GetMaxDistance(QVector<QVector3D> vertex);						//找距离最大的两点之间的距离
		static QVector3D NormToModelPosition(QVector3D normPos, QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model);	//标准坐标转模型坐标
		static QVector3D ModelToNormPosition(QVector3D modelPos, QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model);	//模型坐标转标准坐标
		static QVector3D ModelToViewPosition(QVector3D modelPos, QMatrix4x4 view, QMatrix4x4 model);	//模型坐标转标准坐标
		static QVector3D ModelToModelPosition(QVector3D modelPos1, QMatrix4x4 projection1, QMatrix4x4 view1, QMatrix4x4 model1, QMatrix4x4 projection2, QMatrix4x4 view2, QMatrix4x4 model2);
		static QVector3D PixelToNormPosition(int Posx, int Posy, int scr_width, int scr_height, float depth = 0);	//像素坐标转换为标准坐标(深度值默认为0.5)
		static QVector3D PixelToModelPosition(int Posx, int Posy, QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model, float scr_width, int scr_height, float depth = 0);//像素坐标转换为模型的坐标
		static QVector2D PixelToOrthoPosition(int Posx, int Posy, float Left, float Right, float Bottom, float Top, float scr_width, int scr_height);//像素坐标转换为正交投影的坐标
		static QVector2D ModelToPixelPositon(QVector3D modelPos, QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model, float scr_width, int scr_height);//模型坐标转屏幕像素坐标
		
		//根据一个向量求得一个任意与它垂直的单位向量
        static QVector3D getNormVector(QVector3D vec);
		//根据一个向量求得与它垂直的4个单位向量，4个单位向量间隔90度
		static QVector<QVector3D> getFourNormVector(QVector3D vec);
	
	};
}
