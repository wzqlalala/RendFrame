#pragma once
#include "rendbasic_global.h"

#include <QObject>
#include"qvector3d.h"
#include"qmatrix4x4.h"
#include"mMeshViewEnum.h"

namespace MViewBasic
{
	class RENDBASIC_EXPORT mCommonView : public QObject
	{
		Q_OBJECT

	public:
		mCommonView();
		~mCommonView();
		

	
		//设置视角
		QVector3D  _Eye, _Center, _Up;											//全局摄像机参数
	
		//设置正交投影
		float _Left, _Right, _Bottom, _Top, _NearPlane, _FarPlane;				//正交投影全局变量
		float _Left_ini, _Right_ini, _Bottom_ini, _Top_ini;						//正交投影初始化的数据

		//PVM矩阵
		QMatrix4x4 _projection, _view, _model;

		//保存的视角参数
		QVector3D _Eye_Saved, _Center_Saved, _Up_Saved;
		QMatrix4x4 _projection_Saved, _view_Saved, _model_Saved;

		void InitialCommonView();												//初始化通用视角
		void InitialOrtho();													//初始化正交投影(旋转中心)
		void InitialCamera();													//初始化相机
		void SetOrthoByRatio(float scr_width, float scr_height);				//通过屏幕长宽比设置正交投影
		void Rotate(float xoffset, float yoffset, RotateType rotateType = Rotate_XY, float sensitivity = 0.25);//旋转
		void Rotate_ByBotton(float angle);//绕着垂直屏幕的轴旋转固定角度
		void SaveCurrentView();//保存当前视角
		void CallSavedView();	//调用保存的视角
		void SetPVMValue();														//设置PVM矩阵的值

			

		//设置视角																													
		void SetPerspective(Perspective type);	//设置观察视角

	};
}
