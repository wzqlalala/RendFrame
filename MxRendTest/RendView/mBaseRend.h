#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLFramebufferObject>

#include"mMeshViewEnum.h"
#include "app.h"

#include "SpaceTree.h"

namespace MViewBasic
{
	class mModelView;
	class mViewBase;
}
namespace mxr
{
	class Group;
	class Viewer;
}
using namespace std;
using namespace MViewBasic;
namespace MPostRend
{
	class mPostRender;
}
namespace MPreRend
{
	class mPreRender;
}
namespace MBaseRend
{
	class mBackGroundRender;
	class mQuadRender;
	class mBaseRender;
	class mPostRender;
	class mFontRender;
	class mArrowRender;
	typedef QVector<shared_ptr<mBaseRender>> RenderArray;
	class RENDVIEW_EXPORT mBaseRend : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
	{
		Q_OBJECT

	public:
		mBaseRend(const QString& name);
		~mBaseRend();

		std::shared_ptr<mxr::Application> getApplication() { return _app; };

		shared_ptr<mxr::Group> getRoot() { return _root; };

		shared_ptr<mViewBase> getCamera() {return _modelView;};

		shared_ptr<mViewBase> getCamera1() { return _commonView; };

		void setCameraType(CameraType cameratype);

		shared_ptr<mBaseRender> getFirstBeforeRender() { 
			if (!_beforeRenderArray.empty()) {
				return _beforeRenderArray.first();
			}
			return nullptr;
		};

		shared_ptr<mBaseRender> getFirstAfterRender() {
			if (!_beforeRenderArray.empty()) {
				return _beforeRenderArray.first();
			}
			return nullptr;
		};

		shared_ptr<mFontRender> getFontRender() { return _fontRender; };

		shared_ptr<mArrowRender> getArrowRender() { return _arrowRender; };

		virtual shared_ptr<MPostRend::mPostRender> getPostRender() { return nullptr; };

		virtual shared_ptr<MPreRend::mPreRender> getPreRender() { return nullptr; };

		void addBeforeRender(shared_ptr<mBaseRender> baseRender);

		void removeBeforeRender(shared_ptr<mBaseRender> baseRender);

		void addAfterRender(shared_ptr<mBaseRender> baseRender);

		void removeAfterRender(shared_ptr<mBaseRender> baseRender);

		void clearRender();

		void setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers> buttons, CameraOperateMode cameraOperateMode);

		void setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers> buttons, PickMode pickMode);

		void setMultiplyPickMode(MultiplyPickMode multiplyPickMode);

		void setPickFilter(MViewBasic::PickFilter pickFilter);

		PickMode *getCurrentPickMode() { return _pickMode; };//当前拾取模式

		MultiplyPickMode *getMultiplyPickMode() { return _multiplyPickMode; };//框选拾取模式

		PickFilter *getPickFilter() { return _pickFilter; };//拾取过滤模式

		PickFuntion getPickFuntion()
		{
			if (_mouseButton == Qt::LeftButton)
				return PickFuntion::AddPick;
			return PickFuntion::ReducePick;
		};//拾取过滤的功能

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

		virtual bool event(QEvent *e) override;

	public slots:

		//导入新模型后更新视角
		virtual void slotResetOrthoAndCamera() {};
		//模型中添加/删除后更新视角（不自适应）
		virtual void slotUpdateOrthoAndCamera() {};
		//模型中添加顶点后更新视角（需要自适应）
		virtual void slotUpdateOrthoAndCamera(QVector<QVector3D> addVertex) {};

	protected:
		void GetPointDepthAtMouse();

	protected:
		CameraOperateMode getCameraMode(Qt::MouseButton, Qt::KeyboardModifiers);


		PickMode getPickMode(Qt::MouseButton, Qt::KeyboardModifiers);

	public:
		//框选放大
		void SetZoomAtFrameCenter();//设置绕着选取框中心进行缩放（不连续）
		//视图自适应
		void FitView();
		//设置旋转中心为视图中心
		void SetRotateCenterToViewCenter();
		//设置旋转中心为视图上一点
		void SetRotateCenterToPoint();
		//设置旋转中心为几何中心
		void SetRotateCenterToModelCenter();
		//设置点击按钮绕着垂直屏幕轴旋转
		void SetRotate_ByButton(float angle);
		//设置屏幕中心缩放(按键进行缩放，不连续)
		void SetZoomAtViewCenter_ByButton(ScaleDirection);
		//初始化视角槽函数
		void SetPerspective(Perspective);
		//保存当前视角
		void SaveCurrentView();
		//调用保存的视角
		void CallSavedView();

		//设置屏幕中心缩放(鼠标移动进行缩放，连续)
		void SetZoomAtViewCenter_ByMove();
		//设置点击鼠标中键进行旋转类型
		void SetRotateType(RotateType);
		//设置点击鼠标中键进行平移
		void SetTranslateXY();

		//设置自定义角度旋转槽函数
		void slotSetRotate_ByButton(float angle);

		//获取模型参数
		virtual void GetModelSizePara(bool isModelCenter) {};//是否以模型几何中心为中心

		//显示隐藏更新视角
		//void slotUpdateOrthoAndCamera();

	protected:
		QOpenGLFramebufferObjectFormat format;
		QOpenGLFramebufferObject *FBO;

		QString _name;

		std::shared_ptr<mxr::Application> _app;

		std::shared_ptr<mxr::Group> _root;
		std::shared_ptr<mxr::Viewer> _viewer;

		std::shared_ptr<mxr::Group> _afterroot;
		std::shared_ptr<mxr::Viewer> _afterviewer;

		std::shared_ptr< mBackGroundRender> _bgRend;

		std::shared_ptr<mViewBase> _modelView;

		CameraType _cameraType{ CameraType::Camera1};

		std::shared_ptr<mViewBase> _commonView;

		RenderArray _beforeRenderArray;

		RenderArray _afterRenderArray;

		//文字渲染
		std::shared_ptr<mFontRender> _fontRender;

		//箭头渲染
		std::shared_ptr<mArrowRender> _arrowRender;

		//矩形框
		std::shared_ptr<mQuadRender> _quadRender;

		//窗口大小
		int SCR_WIDTH = 800, SCR_HEIGHT = 600;
		//鼠标在屏幕中的坐标
		GLint lastX{}, lastY{}, nowX{}, nowY{};
		//第一次鼠标点击获取的像素坐标
		GLint Posx_Firstmouse, Posy_Firstmouse;
		//多边形框的坐标点
		QVector<QVector2D> _polygonVertexs;

		//获取当前鼠标点的深度值（网格模型）//用于绕着某一点旋转
		float BufDepth = 0.0; //BufDepth范围（0, 1）

		//模型参数
		QVector3D _center_model; //模型中心
		float _maxRadius_model;//最大旋转半径(以模型中心)
		QVector3D _center_now; //当前旋转中心
		float _maxRadius_now;//最大旋转半径（以当前中心）
		Space::AABB _aabb; //模型在X,Y,Z方向上的最大参数


		//鼠标点击功能判断
		Qt::MouseButton _mouseButton = Qt::NoButton;
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

		ViewOperateMode *_viewOperateMode;

		CameraOperateMode *_cameraMode;

		PickMode *_pickMode;//当前拾取模式

		MultiplyPickMode *_multiplyPickMode;//框选拾取模式

		PickFilter *_pickFilter;//当前拾取过滤模式

		static QHash<QPair<Qt::MouseButton, Qt::KeyboardModifiers>, CameraOperateMode> _cameraKeys;

		static QHash<QPair<Qt::MouseButton, Qt::KeyboardModifiers>, PickMode> _pickKeys;//操作键位
	};

}
