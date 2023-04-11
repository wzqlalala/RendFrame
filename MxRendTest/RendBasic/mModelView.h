#pragma once
#include "rendbasic_global.h"

#include <QObject>
#include<qmatrix4x4.h>
#include <QVector3D>
#include <QVector2D>
#include"mMeshViewEnum.h"

namespace MViewBasic
{
	class RENDBASIC_EXPORT mModelView : public QObject
	{
		Q_OBJECT

	public:
		mModelView(QObject *parent = 0);
		~mModelView();
	

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

		//设置屏幕长宽比例
		float _Ratio_Old;						//上一次的屏幕长宽比
		float _Ratio_New;						// 目前的屏幕长宽比

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
		void SetOrthoByRatio(float scr_width, float scr_height);	
		//通过屏幕长宽比还原正交投影
		void ReturnOrthoByRatio();		
		//设置PVM矩阵的值
		void SetPVMValue();	
		//获取PVM值
		QMatrix4x4 getPVMValue() { return _projection * _view * _model; };

		//旋转 平移 缩放
		void Translate(float xoffset, float yoffset);//平移
		void Rotate(float xoffset, float yoffset, RotateType rotateType = Rotate_XY, float sensitivity = 0.25);//旋转
		void Rotate_ByBotton(float angle);//绕着垂直屏幕的轴旋转固定角度	
		void ZoomAtMouse_Bywheel(int posX, int posY, int rollervalue, ScalePerspectice scalePerspectice, float sensitivity = 1.2);//跟随鼠标点缩放（滚轮缩放）
		void ZoomAtFrameCenter(int CenterposX, int CenterposY, float sensitivity = 2.0);//根据框选中心缩放
		void ZoomAtViewCenter_ByBotton(ScaleDirection scaleDirection, float sensitivity = 2);//根据视角中心缩放(鼠标点击一次缩放一次 不连续)
		void ZoomAtViewCenter_ByPercent(float percent);//根据视角中心缩放(按百分比缩放 例如缩放0.1)
		void ZoomAtViewCenter_ByMove(int FirstPosx, int FirstPosy, int nowPosx, int nowPosy,float sensitivity = 1.1);	//根据视角中心缩放（鼠标移动缩放，连续）
		//设置视角																													
		void SetPerspective(Perspective type);	//设置观察视角
		void FitView(QVector3D center);		//调整视角使模型充满整个视图
		void SaveCurrentView();//保存当前视角
		void CallSavedView();	//调用保存的视角
		
		//设置旋转中心
		void SetRotateCenterToPoint(QVector3D center, float maxRadius);	//设置旋转中心为选择的点
		void SetRotateCenterToViewCenter(QVector3D center,float maxRadius);	//设置旋转中心为视图中心				
		void SetRotateCenterToModelCenter(QVector3D center,float maxRadius);	//设置旋转中心为几何中心
		
		//模型发生变化后更新视角（保留上一次的旋转特性）
		void ResetOrthoAndCamera(QVector3D center,float maxRadius);//传入旋转中心和最大旋转半径
		//添加模型顶点后更新视角（视角自适应）
		void UpdateOrthoAndCamera(QVector<QVector3D> Allvertex,QVector<QVector3D> AddVertex);
		void UpdateOrthoAndCamera(float maxRadius,QVector<QVector3D> AddVertex);
		//添加/删除模型部分顶点后更新视角（视角无自适应）
		void UpdateOrthoAndCamera(QVector<QVector3D> Allvertex);
		void UpdateOrthoAndCamera(float maxRadius_modelcenter, float maxRadius_nowcenter);

		
	};
}

