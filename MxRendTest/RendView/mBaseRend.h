#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_5_Core>

#include"mMeshViewEnum.h"
#include "app.h"

namespace MViewBasic
{
	class mModelView;
	class mCommonView;
}
namespace mxr
{
	class Group;
	class Viewer;
}
using namespace std;
using namespace MViewBasic;
namespace MBaseRend
{
	class mBackGroundRender;
	class mBaseRender;
	typedef QVector<shared_ptr<mBaseRender>> RenderArray;
	class RENDVIEW_EXPORT mBaseRend : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
	{
		Q_OBJECT

	public:
		mBaseRend(const QString& name);
		~mBaseRend();

		std::shared_ptr<mxr::Application> getApplication() { return _app; };

		shared_ptr<mxr::Group> getRoot() { return _root; };

		shared_ptr<mModelView> getCamera() {return _modelView;};

		void addRender(shared_ptr<mBaseRender> baseRender);

		void removeRender(shared_ptr<mBaseRender> baseRender);

		void clearRender();

	public:
		/*
		* 重写父类函数
		*/
		void initializeGL()  Q_DECL_OVERRIDE;
		void resizeGL(int w, int h) Q_DECL_OVERRIDE;
		void paintGL() Q_DECL_OVERRIDE;
		void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
		void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
		void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
		void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

	protected:
		void GetPointDepthAtMouse();

	public:
		//框选放大
		//void SetZoomAtFrameCenter();//设置绕着选取框中心进行缩放（不连续）
		//视图自适应
		//void FitView();
		//设置旋转中心为视图中心
		//void SetRotateCenterToViewCenter();
		//设置旋转中心为视图上一点
		//void SetRotateCenterToPoint();
		//设置旋转中心为几何中心
		//void SetRotateCenterToModelCenter();
		//设置点击按钮绕着垂直屏幕轴旋转
		//void SetRotate_ByButton(float angle);
		//设置屏幕中心缩放(按键进行缩放，不连续)
		//void SetZoomAtViewCenter_ByButton(ScaleDirection);
		//初始化视角槽函数
		//void SetPerspective(Perspective);
		//保存当前视角
		//void SaveCurrentView();
		//调用保存的视角
		//void CallSavedView();

		//设置屏幕中心缩放(鼠标移动进行缩放，连续)
		//void SetZoomAtViewCenter_ByMove();
		//设置点击鼠标中键进行旋转类型
		//void SetRotateType(RotateType);
		//设置点击鼠标中键进行平移
		//void SetTranslateXY();

		//设置自定义角度旋转槽函数
		//void slotSetRotate_ByButton(float angle);

		//显示隐藏更新视角
		//void slotUpdateOrthoAndCamera();

	protected:
		QString _name;

		std::shared_ptr<mxr::Application> _app;

		std::shared_ptr<mxr::Group> _root;

		std::shared_ptr<mxr::Viewer> _viewer;

		std::shared_ptr< mBackGroundRender> _bgRend;

		std::shared_ptr<mModelView> _modelView;

		std::shared_ptr<mCommonView> _commonView;

		RenderArray _renderArray;

		//窗口大小
		int SCR_WIDTH = 800, SCR_HEIGHT = 600;
		//鼠标在屏幕中的坐标
		GLint lastX{}, lastY{}, nowX{}, nowY{};
		//第一次鼠标点击获取的像素坐标
		GLint Posx_Firstmouse, Posy_Firstmouse;
		//矩形框的坐标
		QVector2D left_up, left_down, right_down, right_up;
		//获取当前鼠标点的深度值（网格模型）//用于绕着某一点旋转
		float BufDepth = 0.0; //BufDepth范围（0, 1）

		//模型参数
		QVector3D _center_model; //模型中心
		float _maxRadius_model;//最大旋转半径(以模型中心)
		QVector3D _center_now; //当前旋转中心
		float _maxRadius_now;//最大旋转半径（以当前中心）
		float _left, _right, _top, _bottom, _back, _front; //模型在X,Y,Z方向上的最大参数


		//鼠标点击功能判断
		bool isFirstMouse = true, isMiddleMousePress = false, isLeftMousePress = false, isRightMousePress = false;
		//鼠标移动判断
		bool isMouseMove = false;

		//视角功能判断
		bool ifGetRotateCenter = false; //是否获取旋转中心（当前鼠标的点击像素点坐标）
		bool ifRotateAtViewCenter = false;//是否绕着视角中心旋转
		bool ifRotateAtXY = true;//是否在XY平面旋转
		bool ifRotateAtZ = false;//是否在绕Z轴旋转
		bool ifTranslateXY = false; //是否在XY平面内移动
		bool ifZoomByMouseMove = false; //是够根据鼠标移动进行缩放
		bool ifZoomAtFrameCenter = false; //是够根据选取框中心进行缩放



	};
}
