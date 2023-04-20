#pragma once
#include "rendbasic_global.h"

#include <QObject>
#include<qmatrix4x4.h>
#include <QVector3D>
#include <QVector2D>
#include"mMeshViewEnum.h"

namespace MViewBasic
{
	enum CameraMovement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		DISABLE,
		ENABLE
	};
	class RENDBASIC_EXPORT mViewBase : public QObject
	{
		Q_OBJECT

	public:
		mViewBase();
		~mViewBase();


	public:

		//屏幕的像素大小
		float SCR_WIDTH, SCR_HEIGHT;

		//摄像机视角参数
		QVector3D _Eye = QVector3D(0, 0, 1);
		QVector3D _Eye_0 = QVector3D(0, 0, 1);
		QVector3D _Center = QVector3D(0, 0, 0);
		QVector3D _Up = QVector3D(0, 1, 0);

		//最大旋转半径
		float _MaxRadius;

		

		//设置正交投影
		double _Left, _Right, _Bottom, _Top, _NearPlane, _FarPlane;				//正交投影全局变量
		double _Left_ini, _Right_ini, _Bottom_ini, _Top_ini;						//正交投影初始化的数据
		double _Left_res, _Right_res, _Bottom_res, _Top_res;						//窗口大小改变后保持模型特征的数据		

		//PVM矩阵
		QMatrix4x4 _projection, _view, _model;

		//保存的视角参数
		QVector3D _Eye_Saved, _Eye_0_Saved, _Center_Saved, _Up_Saved;
		double _MaxRadius_Saved;
		double _Left_Saved, _Right_Saved, _Bottom_Saved, _Top_Saved, _NearPlane_Saved, _FarPlane_Saved;
		double _Left_ini_Saved, _Right_ini_Saved, _Bottom_ini_Saved, _Top_ini_Saved;
		double _Left_res_Saved, _Right_res_Saved, _Bottom_res_Saved, _Top_res_Saved;
		QMatrix4x4 _projection_Saved, _view_Saved, _model_Saved;

		//缩放的比例
		double _ScaleProportion = 1;

		//当前正交投影的参数(用于移动缩放)
		float width_firstmouse, height_firstmouse;
		bool IfFirstMouse = false;
		//初始化模型视角
		void InitialModelView();
		//初始化正交投影
		void InitialOrtho();
		//初始化视角
		void InitialCamera();
		//通过屏幕长宽比设置正交投影
		virtual void SetOrthoByRatio(float scr_width, float scr_height);
		//通过屏幕长宽比还原正交投影
		void ReturnOrthoByRatio();
		//设置PVM矩阵的值
		virtual void SetPVMValue();
		//获取PVM值
		QMatrix4x4 getPVMValue() { return _projection * _view * _model; };

		virtual void ZoomAtMouse_Bywheel(int posX, int posY, int yoffset, ScalePerspectice scalePerspectice, float sensitivity = 1.2);

		virtual void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

		//模型发生变化后更新视角（保留上一次的旋转特性）
		virtual void ResetOrthoAndCamera(QVector3D center, float maxRadius);//传入旋转中心和最大旋转半径

		//旋转 平移 缩放


	};
}

