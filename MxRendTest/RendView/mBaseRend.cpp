#include "mBaseRend.h"
#include "mBackGroundRender.h"
#include "mQuadRender.h"
#include "mBaseRender.h"
//工具类
#include"mViewToolClass.h"
//视图类
#include "mModelView.h"
#include "mCommonView.h"

//Qt
#include <QMouseEvent>
#include <QGuiApplication>

////MxRender
#include <renderpch.h>
#include "mShaderManage.h"
#include "mTextureManage.h"
#include "appInstance.h"

namespace MBaseRend
{
	mBaseRend::mBaseRend(const QString& name): _name(name)
	{
		setMouseTracking(true);
		//QOpenGLContext *context = QOpenGLContext::currentContext();
		_app = MakeAsset<mxr::Application>();
		//_app->setContext(context);
		mxr::ApplicationInstance::GetInstance().appendApplication(name, _app);
	
		_root = MakeAsset<mxr::Group>();

		//QSurfaceFormat format;
		//format.setMajorVersion(4);
		//format.setMinorVersion(5);
		//format.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);
		//format.setSamples(4);
		//setFormat(format);
		//this->showMaximized();
		qDebug() << "Base Struct";
	}

	void mBaseRend::initializeGL()
	{
		//_app->setContext(context());
		//_context->makeCurrent(_context->surface());
		//QOpenGLContext *context = QOpenGLContext::currentContext();

		this->initializeOpenGLFunctions();
		QOpenGLContext *context = QOpenGLContext::currentContext();
		//qDebug() << "initializeGL" << QString::number(long long int(context->surface()), 16);
		_app->setContext(context);
		//glViewport(0, 0, this->width(), this->height());
		_modelView = MakeAsset<mModelView>();
		_commonView = MakeAsset<mCommonView>();

		/**/
		mxr::Log::Init();
		_viewer = MakeAsset<mxr::Viewer>();
		_viewer->setSceneData(_root);

		_bgRend = MakeAsset<mBackGroundRender>(_app, _root);
		_quadRender = MakeAsset<mQuadRender>(_app, _root);

		/**/
		//_app->GLContext()->functions()->glEnable(GL_POINT_SPRITE);		//开启渲染点精灵功能
		//_app->GLContext()->functions()->glEnable(GL_PROGRAM_POINT_SIZE); //让顶点程序决定点块大小

		glEnable(GL_POINT_SPRITE);		//开启渲染点精灵功能
		glEnable(GL_PROGRAM_POINT_SIZE); //让顶点程序决定点块大小
		glEnable(GL_DEPTH_TEST);

		qDebug() << "Base Initial";
	}

	void mBaseRend::paintGL()
	{
		QOpenGLContext *context = QOpenGLContext::currentContext();
		//glEnable(GL_POINT_SPRITE);		//开启渲染点精灵功能
		//glEnable(GL_PROGRAM_POINT_SIZE); //让顶点程序决定点块大小
		//qDebug() << "mBaseRend::paintGL()" << QString::number(long long int(context), 16);
		GLenum error = QOpenGLContext::currentContext()->functions()->glGetError();
		if (error != 0)
		{
			qDebug() << error;
		}
		_viewer->run();
		//glClearColor(0, 0, 0, 1);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		error = QOpenGLContext::currentContext()->functions()->glGetError();
		if (error != 0)
		{
			qDebug() << error;
		}
		for (auto baseRender : _renderArray)
		{
			baseRender->updateUniform(_modelView, _commonView);
		}
		_quadRender->draw(_cameraMode, _pickMode, _multiplyPickMode, _polygonVertexs, SCR_WIDTH, SCR_HEIGHT);
	}

	void mBaseRend::resizeGL(int w, int h)
	{
		QOpenGLContext *context = QOpenGLContext::currentContext();
		context->makeCurrent(context->surface());
		//qDebug() << "resizeGL" << QString::number(long long int(context->surface()), 16);
		//qDebug() << "resizeGL" << QString::number(long long int(context), 16);
		glViewport(0, 0, w, h);
		SCR_WIDTH = w;
		SCR_HEIGHT = h;

		_modelView->SetOrthoByRatio(w, h);
		_commonView->SetOrthoByRatio(w, h);
	}
	void mBaseRend::mousePressEvent(QMouseEvent *event)
	{
		Posx_Firstmouse = event->pos().x();
		Posy_Firstmouse = event->pos().y();
		//left_up = QVector2D(Posx_Firstmouse, Posy_Firstmouse);
		//left_down = QVector2D(Posx_Firstmouse, Posy_Firstmouse);
		//right_down = QVector2D(Posx_Firstmouse, Posy_Firstmouse);
		//right_up = QVector2D(Posx_Firstmouse, Posy_Firstmouse);
		nowX = event->pos().x();
		nowY = event->pos().y();
		lastX = nowX;
		lastY = nowY;
		_mouseButton = event->button();
		Qt::KeyboardModifiers keyModifiers = event->modifiers();
		_cameraMode = this->getCameraMode(_mouseButton, keyModifiers);
		if (_cameraMode == NoCameraOperate)
		{
			_viewOperateMode = NoViewOperate;
			_pickMode = this->getPickMode(_mouseButton, keyModifiers);
			if (_pickMode != NoPick)
			{
				_viewOperateMode = PickOperate;
				if (_pickMode == SoloPick)
				{
					for (auto render : _renderArray)
					{
						if (render->getIsDragSomething())
						{
							_pickMode = DragPick;
							break;
						}
					}
				}
				if (_pickMode == MultiplyPick)
				{
					_polygonVertexs.append(QVector2D(nowX, nowY));
				}
			}
		}
		else
		{
			_viewOperateMode = CameraOperate;
			if (_cameraMode == Zoom)
			{
				_polygonVertexs.append(QVector2D(nowX, nowY));
			}
		}

		update();
	}
	void mBaseRend::mouseReleaseEvent(QMouseEvent *event)
	{
		if (_viewOperateMode == CameraOperate)
		{
			if (_cameraMode == Zoom)
			{
				_modelView->ZoomAtFrameCenter((Posx_Firstmouse + nowX) / 2, (Posy_Firstmouse + nowY) / 2);
			}
		}
		else if (_viewOperateMode == PickOperate)
		{
			if (_pickMode != NoPick)
			{
				for (auto render : _renderArray)
				{
					render->setPickParameters(_pickMode, _multiplyPickMode, _polygonVertexs);
					render->startPick();
				}
			}
		}
		_polygonVertexs.clear();
		_viewOperateMode = NoViewOperate;
		_cameraMode = NoCameraOperate;
		_pickMode = NoPick;

		update();
	}

	void mBaseRend::mouseMoveEvent(QMouseEvent *event)
	{
		isMouseMove = true;
		nowX = event->pos().x();
		nowY = event->pos().y();

		//left_down = QVector2D(Posx_Firstmouse, nowY);
		//right_down = QVector2D(nowX, nowY);
		//right_up = QVector2D(nowX, Posy_Firstmouse);
		//roundCenter = QVector2D((nowX + Posx_Firstmouse) / 2.0, (nowY + Posy_Firstmouse) / 2.0);
		//roundPoint = QVector2D(nowX, nowY);
		GLint xoffset = lastX - nowX;//计算X方向偏移量(移动像素)
		GLint yoffset = nowY - lastY;// 计算Y方向偏移量
		lastX = nowX;
		lastY = nowY;

		if (_viewOperateMode == CameraOperate)
		{
			if (_cameraMode == Rotate)
			{
				if (ifRotateAtXY == true && ifGetRotateCenter == false)
				{
					_modelView->Rotate(xoffset, yoffset, Rotate_XY);
					_commonView->Rotate(xoffset, yoffset, Rotate_XY);
				}
				else if (ifRotateAtZ == true && ifGetRotateCenter == false)
				{
					_modelView->Rotate(xoffset, yoffset, Rotate_Z);
					_commonView->Rotate(xoffset, yoffset, Rotate_Z);
				}
			}
			else if (_cameraMode == Translate)
			{
				_modelView->Translate(xoffset, yoffset);			
			}
			else if (_cameraMode == Zoom)
			{
				float distance = 0.0;
				if (_polygonVertexs.size() > 0)
				{
					distance = _polygonVertexs.last().distanceToPoint(QVector2D(nowX, nowY));
				}
				if (distance > 20)
				{
					_polygonVertexs.append(QVector2D(nowX, nowY));
				}
				//_modelView->ZoomAtFrameCenter((Posx_Firstmouse + nowX) / 2, (Posy_Firstmouse + nowY) / 2);
				//_modelView->ZoomAtViewCenter_ByMove(Posx_Firstmouse, Posy_Firstmouse, nowX, nowY);
			}
			//else if (ifZoomByMouseMove == true && ifGetRotateCenter == false)
			//{
			//	_modelView->ZoomAtViewCenter_ByMove(Posx_Firstmouse, Posy_Firstmouse, nowX, nowY);
			//}
		}
		else if (_viewOperateMode == PickOperate)
		{
			if (_pickMode == MultiplyPick)
			{
				float distance = 0.0;
				if (_polygonVertexs.size() > 0)
				{
					distance = _polygonVertexs.last().distanceToPoint(QVector2D(nowX, nowY));
				}
				//qDebug() << _polygonVertexs.last() << QVector2D(nowX, nowY) << distance;
				if (_multiplyPickMode == PolygonPick)
				{
					if (distance > 20)
					{
						_polygonVertexs.append(QVector2D(nowX, nowY));
					}
				}
				else
				{
					_polygonVertexs.append(QVector2D(nowX, nowY));
				}
			}
		}
		update();

	}
	void mBaseRend::wheelEvent(QWheelEvent *event)
	{
		QPoint numDegrees = event->angleDelta();//获取滚轮的转角
		if (abs(numDegrees.y() - 0) < 0.0001)
		{
			return;
		}
		_modelView->ZoomAtMouse_Bywheel(nowX, nowY, numDegrees.y(), Model);
		update();
	}

	mBaseRend::~mBaseRend()
	{
		int appInstance = mxr::ApplicationInstance::GetInstance().removeApplication(_name);
		if (appInstance == 0)
		{
			mxr::mShaderManage::GetInstance()->deleteAllShaders();
			mxr::mTextureManage::GetInstance()->deleteAllTextures();
		}
		qDebug() << "Base Distruct";

	}
	void mBaseRend::addRender(shared_ptr<mBaseRender> baseRender)
	{
		if (_renderArray.contains(baseRender))
		{
			return;
		}
		mBaseRender *t = baseRender.get();
		QObject::connect(t, SIGNAL(update()), this, SLOT(update()));
		_renderArray.append(baseRender);
	}
	void mBaseRend::removeRender(shared_ptr<mBaseRender> baseRender)
	{
		_renderArray.removeOne(baseRender);
		baseRender.reset();
	}
	void mBaseRend::clearRender()
	{
		for (auto baseRender : _renderArray)
		{
			baseRender.reset();
			//baseRender->updateUniform(_modelView, _commonView);
		}
		_renderArray.clear();
	}
	void mBaseRend::setMultiplyPickMode(MultiplyPickMode multiplyPickMode)
	{
		_multiplyPickMode = multiplyPickMode;
	}
	void mBaseRend::GetPointDepthAtMouse()
	{
		glReadPixels(nowX, SCR_HEIGHT - nowY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &BufDepth);//OpenGL像素坐标原点左下角 BufDepth范围（0,1）
	}
	CameraOperateMode mBaseRend::getCameraMode(Qt::MouseButton mouseButton, Qt::KeyboardModifiers modifiers)
	{
		if (_cameraKeys.contains({ mouseButton, modifiers }))
		{
			return _cameraKeys.value({ mouseButton, modifiers });
		}
		return NoCameraOperate;
	}
	PickMode mBaseRend::getPickMode(Qt::MouseButton mouseButton, Qt::KeyboardModifiers modifiers)
	{
		if (_pickKeys.contains({ mouseButton, modifiers }))
		{
			return _pickKeys.value({ mouseButton, modifiers });
		}
		return NoPick;
	}

	//设置视图*缩放*类型
	void mBaseRend::SetZoomAtViewCenter_ByMove()
	{
		ifRotateAtXY = false;
		ifRotateAtZ = false;
		ifTranslateXY = false;
		ifZoomByMouseMove = true;

	}

	//设置视图*旋转*类型
	void mBaseRend::SetRotateType(RotateType rotateType)
	{
		if (rotateType == Rotate_XY)
		{
			ifRotateAtXY = true;
			ifRotateAtZ = false;
			ifTranslateXY = false;
			ifZoomByMouseMove = false;
		}
		else if (rotateType == Rotate_Z)
		{
			ifRotateAtXY = false;
			ifRotateAtZ = true;
			ifTranslateXY = false;
			ifZoomByMouseMove = false;
		}
	}
	//设置视图*移动*类型
	void mBaseRend::SetTranslateXY()
	{

		ifRotateAtXY = false;
		ifRotateAtZ = false;
		ifTranslateXY = true;
		ifZoomByMouseMove = false;

	}

	void mBaseRend::SetZoomAtFrameCenter()
	{
		ifZoomAtFrameCenter = true;
	}
	void mBaseRend::SetZoomAtViewCenter_ByButton(ScaleDirection scaleDirection)
	{
		_modelView->ZoomAtViewCenter_ByBotton(scaleDirection);

		update();
	}
	//设置视图*旋转中心*类型
	void mBaseRend::SetRotateCenterToPoint()
	{
		ifGetRotateCenter = true;
		ifRotateAtViewCenter = false;
		//_lableRendController_common->deleteLableRendData<X_Point_Common>("RotateCenter");

	}
	void mBaseRend::SetRotateCenterToViewCenter()
	{
		ifGetRotateCenter = false;
		ifRotateAtViewCenter = true;
		//获取屏幕中心的坐标
		QVector3D ViewCenter = mViewToolClass::NormToModelPosition(QVector3D(0, 0, 0), _modelView->_projection, _modelView->_view, _modelView->_model);
		//找新的旋转半径
		float maxRadius = mViewToolClass::GetMaxRadius(_left, _right, _bottom, _top, _back, _front, ViewCenter);
		_modelView->SetRotateCenterToViewCenter(ViewCenter, maxRadius);
		_center_now = ViewCenter;
		//传递旋转中心数据
		QVector3D centerPos = mViewToolClass::PixelToModelPosition(SCR_WIDTH / 2, SCR_HEIGHT / 2, _modelView->_projection, _modelView->_view, _modelView->_model, SCR_WIDTH, SCR_HEIGHT);
		//_lableRendController_common->appendLableRendData<X_Point_Common>("RotateCenter", { centerPos });
		update();


	}
	void mBaseRend::SetRotateCenterToModelCenter()
	{
		ifGetRotateCenter = false;
		ifRotateAtViewCenter = false;
		_modelView->SetRotateCenterToModelCenter(_center_model, _maxRadius_model);
		_center_now = _center_model;
		//传递旋转中心数据
		//_lableRendController_common->appendLableRendData<X_Point_Common>("RotateCenter", { _center_model });
		update();


	}
	void mBaseRend::slotSetRotate_ByButton(float angle)
	{
		SetRotate_ByButton(angle);
	}
	void mBaseRend::SetRotate_ByButton(float angle)
	{
		_modelView->Rotate_ByBotton(angle);
		_commonView->Rotate_ByBotton(angle);
		update();
	}
	//设置视图*视角*类型
	void mBaseRend::SetPerspective(Perspective pers)
	{
		_modelView->SetPerspective(pers);
		_commonView->SetPerspective(pers);
		update();
	}
	void mBaseRend::SaveCurrentView()
	{
		_modelView->SaveCurrentView();
		_commonView->SaveCurrentView();
		update();

	}
	void mBaseRend::CallSavedView()
	{
		_modelView->CallSavedView();
		_commonView->CallSavedView();
		//_lableRendController_common->appendLableRendData<X_Point_Common>("RotateCenter", { _modelView->_Center_Saved });
		update();
	}
	//设置视图大小自适应
	void mBaseRend::FitView()
	{
		_modelView->FitView(_center_model);
		update();
	}

	QHash<QPair<Qt::MouseButton, Qt::KeyboardModifiers>, CameraOperateMode> mBaseRend::_cameraKeys = //默认hypermesh
	{ {QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::MiddleButton,Qt::ControlModifier), Zoom},
	  {QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton,Qt::ControlModifier), Rotate},
	  {QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton,Qt::ControlModifier), Translate}, };

	QHash<QPair<Qt::MouseButton, Qt::KeyboardModifiers>, PickMode> mBaseRend::_pickKeys = 
	{
		{QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton,Qt::ShiftModifier), MultiplyPick},
		{QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton,Qt::ShiftModifier), MultiplyPick},
		{QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton,Qt::NoModifier), SoloPick},
		{QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton,Qt::NoModifier), SoloPick},
	};
}