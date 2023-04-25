#include "mBaseRend.h"
#include "mBackGroundRender.h"
#include "mQuadRender.h"
#include "mBaseRender.h"
#include "mFontRender.h"
#include "mArrowRender.h"
//工具类
#include"mViewToolClass.h"
//视图类
#include "mModelView.h"
#include "mCommonView.h"
#include "mViewBase.h"
#include "mCameraModelView.h"
#include "mCameraCommonView.h"

//Qt
#include <QMouseEvent>
#include <QGuiApplication>
#include <QOpenGLFramebufferObject>

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

		_viewOperateMode = new ViewOperateMode; *_viewOperateMode = ViewOperateMode::NoViewOperate;
		_cameraMode = new CameraOperateMode; *_cameraMode = CameraOperateMode::NoCameraOperate;
		_pickMode = new PickMode;*_pickMode = PickMode::NoPick;//当前拾取模式
		_multiplyPickMode = new MultiplyPickMode; *_multiplyPickMode = MultiplyPickMode::QuadPick;//框选拾取模式
		_pickFilter = new PickFilter; *_pickFilter = PickFilter::PickGeoPoint;

		//QOpenGLContext *context = QOpenGLContext::currentContext();
		_app = MakeAsset<mxr::Application>();
		//_app->setContext(context);
		mxr::ApplicationInstance::GetInstance().appendApplication(name, _app);
	
		_beforeroot = MakeAsset<mxr::Group>();
		_root = MakeAsset<mxr::Group>();
		_afterroot = MakeAsset<mxr::Group>();

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
		//glEnable(GL_POINT_SPRITE);		//开启渲染点精灵功能（已经被核心模式废除）
		glEnable(GL_PROGRAM_POINT_SIZE); //让顶点程序决定点块大小
		_modelView = MakeAsset<mModelView>();
		//_modelView = MakeAsset<mModelView>();
		_commonView = MakeAsset<mCommonView>();
		
		mxr::Log::Init();

		_beforeviewer = MakeAsset<mxr::Viewer>();
		_beforeviewer->setSceneData(_beforeroot);
		_bgRend = MakeAsset<mBackGroundRender>(_app, _beforeroot);

		_viewer = MakeAsset<mxr::Viewer>();
		_viewer->setSceneData(_root);

		_afterviewer = MakeAsset<mxr::Viewer>();
		_afterviewer->setSceneData(_afterroot);

		_fontRender = MakeAsset<mFontRender>(_app, _root, this);
		this->addBeforeRender(_fontRender);
		_arrowRender = MakeAsset<mArrowRender>(_app, _root, this);
		this->addBeforeRender(_arrowRender);

		_quadRender = MakeAsset<mQuadRender>(_app, _afterroot, this);
		this->addAfterRender(_quadRender);
		
		makeCurrent();
		GLenum error = QOpenGLContext::currentContext()->functions()->glGetError();
		if (error != GL_NO_ERROR) {
			qDebug() << "OpenGL error:" << error;
		}
		format.setSamples(0);
		format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);	
		FBO = new QOpenGLFramebufferObject(size(), format);
		if (!FBO->bind()) {
			qDebug() << "Failed to bind FBO!";
		}
		error = QOpenGLContext::currentContext()->functions()->glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (error != GL_FRAMEBUFFER_COMPLETE) {
			qDebug() << "Framebuffer not complete!";
		}
		glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
		glEnable(GL_DEPTH_TEST);
		error = QOpenGLContext::currentContext()->functions()->glGetError();
		if (error != 0)
		{
			qDebug() << error;
		}
		
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
		
		//glClearColor(0, 0, 0, 1);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		error = QOpenGLContext::currentContext()->functions()->glGetError();
		if (error != 0)
		{
			qDebug() << error;
		}
		for (auto baseRender : _beforeRenderArray)
		{
			baseRender->updateUniform(_modelView, _commonView);
		}
		_beforeviewer->run();

		for (auto baseRender : _renderArray)
		{
			baseRender->updateUniform(_modelView, _commonView);
		}
		_viewer->noClearRun();

		for (auto baseRender : _afterRenderArray)
		{
			baseRender->updateUniform(_modelView, _commonView);
		}
		_afterviewer->noClearRun();
		glBindFramebuffer(GL_READ_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO->handle());
		glBlitFramebuffer(0, 0, width(), height(), 0, 0, FBO->width(), FBO->height(), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);	
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

		FBO = new QOpenGLFramebufferObject(size(), format);

		update();
	}
	void mBaseRend::mousePressEvent(QMouseEvent *event)
	{
		Posx_Firstmouse = event->pos().x();
		Posy_Firstmouse = event->pos().y();
		nowX = event->pos().x();
		nowY = event->pos().y();
		lastX = nowX;
		lastY = nowY;
		_mouseButton = event->button();
		Qt::KeyboardModifiers keyModifiers = event->modifiers();
		*_cameraMode = this->getCameraMode(_mouseButton, keyModifiers);
		if (*_cameraMode == CameraOperateMode::NoCameraOperate)
		{
			*_viewOperateMode = ViewOperateMode::NoViewOperate;
			*_pickMode = this->getPickMode(_mouseButton, keyModifiers);
			if (*_pickMode != PickMode::NoPick)
			{
				*_viewOperateMode = ViewOperateMode::PickOperate;
				if (*_pickMode == PickMode::SoloPick)
				{
					for (auto render : _beforeRenderArray)
					{
						if (render->getIsDragSomething(QVector2D(nowX, nowY)))
						{
							*_pickMode = PickMode::DragPick;
							break;
						}
					}
					for (auto render : _renderArray)
					{
						if (render->getIsDragSomething(QVector2D(nowX, nowY)))
						{
							*_pickMode = PickMode::DragPick;
							break;
						}
					}
					for (auto render : _afterRenderArray)
					{
						if (render->getIsDragSomething(QVector2D(nowX, nowY)))
						{
							*_pickMode = PickMode::DragPick;
							break;
						}
					}
				}
				else if (*_pickMode == PickMode::MultiplyPick)
				{
					_polygonVertexs.append(QVector2D(nowX, nowY));
				}
			}
		}
		else
		{
			*_viewOperateMode = ViewOperateMode::CameraOperate;
			if (*_cameraMode == CameraOperateMode::Zoom)
			{
				_polygonVertexs.append(QVector2D(nowX, nowY));
			}
		}

		update();
	}
	void mBaseRend::mouseReleaseEvent(QMouseEvent *event)
	{
		auto view = dynamic_pointer_cast<mModelView>(_modelView);
		if (view)
		{
			if (*_viewOperateMode == ViewOperateMode::CameraOperate)
			{
				if (*_cameraMode == CameraOperateMode::Zoom)
				{
					view->ZoomAtFrameCenter((Posx_Firstmouse + nowX) / 2, (Posy_Firstmouse + nowY) / 2);
				}
			}
			else if (*_viewOperateMode == ViewOperateMode::PickOperate)
			{
				if (*_pickMode == PickMode::SoloPick)
				{
					_polygonVertexs.append(QVector2D(nowX, nowY));
					for (auto render : _beforeRenderArray)
					{
						makeCurrent();
						/*
						FBO->bind();
						float depth = 0.0;
						QOpenGLContext::currentContext()->functions()->glReadPixels(nowX, SCR_HEIGHT - nowY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
						if (depth != 0)
						{
							qDebug() << "depth" << depth;
						}
						GLenum error = QOpenGLContext::currentContext()->functions()->glGetError();
						if (error != 0)
						{
							qDebug() << error;
						}
						render->startPick(_polygonVertexs);
						FBO->release();
						*/
						FBO->bind();
						render->startPick(_polygonVertexs);
						FBO->release();
					}
					for (auto render : _renderArray)
					{
						makeCurrent();
						FBO->bind();
						render->startPick(_polygonVertexs);
						FBO->release();
					}
					for (auto render : _afterRenderArray)
					{
						makeCurrent();
						FBO->bind();
						render->startPick(_polygonVertexs);
						FBO->release();
					}
				}
				else if (*_pickMode == PickMode::MultiplyPick)
				{
					for (auto render : _beforeRenderArray)
					{
						render->startPick(_polygonVertexs);
					}
					for (auto render : _renderArray)
					{
						render->startPick(_polygonVertexs);
					}
					for (auto render : _afterRenderArray)
					{
						render->startPick(_polygonVertexs);
					}
				}
			}
			_polygonVertexs.clear();
			*_viewOperateMode = ViewOperateMode::NoViewOperate;
			*_cameraMode = CameraOperateMode::NoCameraOperate;
			*_pickMode = PickMode::NoPick;
		}
		update();
	}

	void mBaseRend::mouseMoveEvent(QMouseEvent *event)
	{
		isMouseMove = true;
		nowX = event->pos().x();
		nowY = event->pos().y();
		//QCursor::setPos(geometry().center());
		GLint xoffset = lastX - nowX;//计算X方向偏移量(移动像素)
		GLint yoffset = nowY - lastY;// 计算Y方向偏移量
		lastX = nowX;
		lastY = nowY;

		auto view = dynamic_pointer_cast<mModelView>(_modelView);
		auto view1 = dynamic_pointer_cast<mCommonView>(_commonView);
		if (view)
		{
			if (*_viewOperateMode == ViewOperateMode::CameraOperate)
			{
				if (*_cameraMode == CameraOperateMode::Rotate)
				{
					if (ifRotateAtXY == true && ifGetRotateCenter == false)
					{
						view->Rotate(xoffset, yoffset, Rotate_XY);
						view1->Rotate(xoffset, yoffset, Rotate_XY);
					}
					else if (ifRotateAtZ == true && ifGetRotateCenter == false)
					{
						view->Rotate(xoffset, yoffset, Rotate_Z);
						view1->Rotate(xoffset, yoffset, Rotate_Z);
					}
				}
				else if (*_cameraMode == CameraOperateMode::Translate)
				{
					view->Translate(xoffset, yoffset);
				}
				else if (*_cameraMode == CameraOperateMode::Zoom)
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
				}
			}
			else if (*_viewOperateMode == ViewOperateMode::PickOperate)
			{
				if (*_pickMode == PickMode::DragPick)
				{
					for (auto render : _beforeRenderArray)
					{
						render->dragSomething(QVector2D(nowX, nowY));
					}
					for (auto render : _renderArray)
					{
						render->dragSomething(QVector2D(nowX, nowY));
					}
					for (auto render : _afterRenderArray)
					{
						render->dragSomething(QVector2D(nowX, nowY));
					}
				}
				else if (*_pickMode == PickMode::MultiplyPick)
				{
					float distance = 0.0;
					if (_polygonVertexs.size() > 0)
					{
						distance = _polygonVertexs.last().distanceToPoint(QVector2D(nowX, nowY));
					}
					if (*_multiplyPickMode == MultiplyPickMode::PolygonPick)
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
		}
		else
		{
			//lastX = geometry().center().x();
			//lastY = geometry().center().y();
			//QCursor::setPos(geometry().center());
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
		auto view = dynamic_pointer_cast<mModelView>(_modelView);
		if (view)
		{
			view->ZoomAtMouse_Bywheel(nowX, nowY, numDegrees.y(), Model);
		}
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
	void mBaseRend::setCameraType(CameraType cameratype)
	{
		_cameraType = cameratype;
		if (_cameraType == CameraType::Camera1)//普通视角
		{
			_commonView = MakeAsset<mCommonView>();
			auto view = MakeAsset<mModelView>();		
			view->SetOrthoByRatio(SCR_WIDTH, SCR_HEIGHT);
			view->ResetOrthoAndCamera(_modelView->_Center, _modelView->_MaxRadius);
			_modelView = view;
			this->setCursor(Qt::ArrowCursor);
		}
		else//漫游视角
		{
			_commonView = MakeAsset<mCameraCommonView>(this);
			auto view = MakeAsset<mCameraModelView>(this);
			view->SetOrthoByRatio(SCR_WIDTH, SCR_HEIGHT);
			view->ResetOrthoAndCamera(_modelView->_Center, _modelView->_MaxRadius);
			_modelView = view;
			this->setCursor(Qt::BlankCursor);
		}
	}
	void mBaseRend::addBeforeRender(shared_ptr<mBaseRender> baseRender)
	{
		if (_beforeRenderArray.contains(baseRender))
		{
			return;
		}
		mBaseRender *t = baseRender.get();
		QObject::connect(t, SIGNAL(update()), this, SLOT(update()));
		_beforeRenderArray.append(baseRender);
	}
	void mBaseRend::removeBeforeRender(shared_ptr<mBaseRender> baseRender)
	{
		_beforeRenderArray.removeOne(baseRender);
		baseRender.reset();
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
	void mBaseRend::addAfterRender(shared_ptr<mBaseRender> baseRender)
	{
		if (_afterRenderArray.contains(baseRender))
		{
			return;
		}
		mBaseRender *t = baseRender.get();
		QObject::connect(t, SIGNAL(update()), this, SLOT(update()));
		_afterRenderArray.append(baseRender);
	}
	void mBaseRend::removeAfterRender(shared_ptr<mBaseRender> baseRender)
	{
		_afterRenderArray.removeOne(baseRender);
		baseRender.reset();
	}
	void mBaseRend::clearRender()
	{
		for (auto baseRender : _beforeRenderArray)
		{
			baseRender.reset();
		}
		for (auto baseRender : _renderArray)
		{
			baseRender.reset();
		}
		for (auto baseRender : _afterRenderArray)
		{
			baseRender.reset();
		}
		_beforeRenderArray.clear();
		_renderArray.clear();
		_afterRenderArray.clear();
	}
	void mBaseRend::setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers> buttons, CameraOperateMode cameraOperateMode)
	{
		if (_cameraKeys.contains(buttons))
		{
			return;
		}
		auto iter = QHashIterator< QPair<Qt::MouseButton, Qt::KeyboardModifiers>, CameraOperateMode>(_cameraKeys);
		while (iter.hasNext())
		{
			iter.next();
			if (iter.value() == cameraOperateMode)
			{
				_cameraKeys.remove(iter.key());
				break;
			}
		}
		_cameraKeys[buttons] = cameraOperateMode;
	}
	void mBaseRend::setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers> buttons, PickMode pickMode)
	{
		if (_pickKeys.contains(buttons))
		{
			return;
		}
		auto iter = QHashIterator< QPair<Qt::MouseButton, Qt::KeyboardModifiers>, PickMode>(_pickKeys);
		while (iter.hasNext())
		{
			iter.next();
			if (iter.value() == pickMode)
			{
				_pickKeys.remove(iter.key());
				break;
			}
		}
		_pickKeys[buttons] = pickMode;
	}
	void mBaseRend::setMultiplyPickMode(MultiplyPickMode multiplyPickMode)
	{
		*_multiplyPickMode = multiplyPickMode;
	}
	void mBaseRend::setPickFilter(MViewBasic::PickFilter pickFilter)
	{
		*_pickFilter = pickFilter;
	}
	bool mBaseRend::event(QEvent * e)
	{
		auto view = dynamic_pointer_cast<mCameraModelView>(_modelView);
		auto view1 = dynamic_pointer_cast<mCameraCommonView>(_commonView);
		if (view)
		{
			view->handle(e);
			view1->handle(e);
		}
		return QWidget::event(e);
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
		return CameraOperateMode::NoCameraOperate;
	}
	PickMode mBaseRend::getPickMode(Qt::MouseButton mouseButton, Qt::KeyboardModifiers modifiers)
	{
		if (_pickKeys.contains({ mouseButton, modifiers }))
		{
			return _pickKeys.value({ mouseButton, modifiers });
		}
		return PickMode::NoPick;
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
		auto view = dynamic_pointer_cast<mModelView>(_modelView);
		if (view)
		{
			view->ZoomAtViewCenter_ByBotton(scaleDirection);
		}

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
		//float maxRadius = mViewToolClass::GetMaxRadius(_left, _right, _bottom, _top, _back, _front, ViewCenter);
		auto view = dynamic_pointer_cast<mModelView>(_modelView);
		if (view)
		{
			//view->SetRotateCenterToViewCenter(ViewCenter, maxRadius);
		}
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
		auto view = dynamic_pointer_cast<mModelView>(_modelView);
		if (view)
		{
			view->SetRotateCenterToModelCenter(_center_model, _maxRadius_model);
		}
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
		auto view = dynamic_pointer_cast<mModelView>(_modelView);
		if (view)
		{
			view->Rotate_ByBotton(angle);
		}
		auto view1 = dynamic_pointer_cast<mCommonView>(_commonView);
		if (view1)
		{
			view1->Rotate_ByBotton(angle);
		}
		update();
	}
	//设置视图*视角*类型
	void mBaseRend::SetPerspective(Perspective pers)
	{
		auto view = dynamic_pointer_cast<mModelView>(_modelView);
		if (view)
		{
			view->SetPerspective(pers);
		}
		auto view1 = dynamic_pointer_cast<mCommonView>(_commonView);
		if (view1)
		{
			view1->SetPerspective(pers);
		}
		update();
	}
	void mBaseRend::SaveCurrentView()
	{
		auto view = dynamic_pointer_cast<mModelView>(_modelView);
		if (view)
		{
			view->SaveCurrentView();
		}
		auto view1 = dynamic_pointer_cast<mCommonView>(_commonView);
		if (view1)
		{
			view1->SaveCurrentView();
		}
		update();

	}
	void mBaseRend::CallSavedView()
	{
		auto view = dynamic_pointer_cast<mModelView>(_modelView);
		if (view)
		{
			view->CallSavedView();
		}
		auto view1 = dynamic_pointer_cast<mCommonView>(_commonView);
		if (view1)
		{
			view1->CallSavedView();
		}
		//_lableRendController_common->appendLableRendData<X_Point_Common>("RotateCenter", { _modelView->_Center_Saved });
		update();
	}
	//设置视图大小自适应
	void mBaseRend::FitView()
	{
		auto view = dynamic_pointer_cast<mModelView>(_modelView);
		if (view)
		{
			view->FitView(_center_model);
		}
		update();
	}

	QHash<QPair<Qt::MouseButton, Qt::KeyboardModifiers>, CameraOperateMode> mBaseRend::_cameraKeys = //默认hypermesh
	{ {QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::MiddleButton,Qt::ControlModifier), CameraOperateMode::Zoom},
	  {QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton,Qt::ControlModifier), CameraOperateMode::Rotate},
	  {QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton,Qt::ControlModifier), CameraOperateMode::Translate}, };

	QHash<QPair<Qt::MouseButton, Qt::KeyboardModifiers>, PickMode> mBaseRend::_pickKeys = 
	{
		{QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton,Qt::ShiftModifier), PickMode::MultiplyPick},
		{QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton,Qt::ShiftModifier), PickMode::MultiplyPick},
		{QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton,Qt::NoModifier), PickMode::SoloPick},
		{QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton,Qt::NoModifier), PickMode::SoloPick},
	};
}