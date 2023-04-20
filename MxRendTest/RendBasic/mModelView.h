#pragma once
#include "rendbasic_global.h"

#include "mViewBase.h"

#include <QObject>
#include<qmatrix4x4.h>
#include <QVector3D>
#include <QVector2D>
#include"mMeshViewEnum.h"

namespace MViewBasic
{
	class RENDBASIC_EXPORT mModelView : public mViewBase
	{
	public:
		mModelView();
		~mModelView();
	

	public:					

		//设置屏幕长宽比例
		float _Ratio_Old;						//上一次的屏幕长宽比
		float _Ratio_New;						// 目前的屏幕长宽比

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
		void SetOrthoByRatio(float scr_width, float scr_height) override;	
		//通过屏幕长宽比还原正交投影
		void ReturnOrthoByRatio();		

		//旋转 平移 缩放
		void Translate(float xoffset, float yoffset);//平移
		void Rotate(float xoffset, float yoffset, RotateType rotateType = Rotate_XY, float sensitivity = 0.25);//旋转
		void Rotate_ByBotton(float angle);//绕着垂直屏幕的轴旋转固定角度	
		void ZoomAtMouse_Bywheel(int posX, int posY, int rollervalue, ScalePerspectice scalePerspectice, float sensitivity = 1.2) override;//跟随鼠标点缩放（滚轮缩放）
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
		void ResetOrthoAndCamera(QVector3D center,float maxRadius) override;//传入旋转中心和最大旋转半径
		//添加模型顶点后更新视角（视角自适应）
		void UpdateOrthoAndCamera(QVector<QVector3D> Allvertex,QVector<QVector3D> AddVertex);
		void UpdateOrthoAndCamera(float maxRadius,QVector<QVector3D> AddVertex);
		//添加/删除模型部分顶点后更新视角（视角无自适应）
		void UpdateOrthoAndCamera(QVector<QVector3D> Allvertex);
		void UpdateOrthoAndCamera(float maxRadius_modelcenter, float maxRadius_nowcenter);

		
	};
}

