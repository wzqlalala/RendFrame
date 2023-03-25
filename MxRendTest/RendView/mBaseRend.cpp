#include "mBaseRend.h"
#include "mBackGroundRender.h"
#include "mBaseRender.h"
//������
#include"mViewToolClass.h"
//��ͼ��
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

		qDebug() << "Base Struct";
	}

	void mBaseRend::initializeGL()
	{
		//_app->setContext(context());
		//_context->makeCurrent(_context->surface());

		this->initializeOpenGLFunctions();

		_modelView = MakeAsset<mModelView>();
		_commonView = MakeAsset<mCommonView>();

		/**/
		mxr::Log::Init();
		_viewer = MakeAsset<mxr::Viewer>();
		_viewer->setSceneData(_root);

		_bgRend = MakeAsset<mBackGroundRender>(_app, _root);

		/**/

		glEnable(GL_POINT_SPRITE);		//������Ⱦ�㾫�鹦��
		glEnable(GL_PROGRAM_POINT_SIZE); //�ö�������������С
		glEnable(GL_DEPTH_TEST);

		qDebug() << "Base Initial";
	}

	void mBaseRend::paintGL()
	{
		for (auto baseRender : _renderArray)
		{
			baseRender->updateUniform(_modelView, _commonView);
		}
		_viewer->run();
	}

	void mBaseRend::resizeGL(int w, int h)
	{
		glViewport(0, 0, w, h);
		SCR_WIDTH = w;
		SCR_HEIGHT = h;

		_modelView->SetOrthoByRatio(w, h);
		_commonView->SetOrthoByRatio(w, h);
	}
	void mBaseRend::mousePressEvent(QMouseEvent *event)
	{
		if (event->button() == Qt::MiddleButton)
		{
			isFirstMouse = true;
			isMiddleMousePress = true;

			//��������ƶ����ŵĲ���
			if (ifZoomByMouseMove == true)
			{
				//��һ�����������������
				Posx_Firstmouse = event->pos().x();
				Posy_Firstmouse = event->pos().y();
				_modelView->IfFirstMouse = true;
			}
			//����������Ļ������ת(ÿ�ε������м����»�ȡ��Ļ��������)
			if (ifRotateAtViewCenter == true)
			{
				//SetRotateCenterToViewCenter();
			}

			//_meshRend->TestFunction();

			//�����ź�
			//mGlobalSignals::getInstance()->viewMiddleButtonPressSig();
		}

		else if (event->button() == Qt::LeftButton)
		{
			isFirstMouse = true;
			isLeftMousePress = GL_TRUE;
			Posx_Firstmouse = event->pos().x();
			Posy_Firstmouse = event->pos().y();

			//��ѡ�Ŵ�
			if (ifZoomAtFrameCenter == true)
			{
				left_up = QVector2D(Posx_Firstmouse, Posy_Firstmouse);
				left_down = QVector2D(Posx_Firstmouse, Posy_Firstmouse);
				right_down = QVector2D(Posx_Firstmouse, Posy_Firstmouse);
				right_up = QVector2D(Posx_Firstmouse, Posy_Firstmouse);
				//mMeshStaticData::_pickSoloOrMutiply = NoPick;
			}
			else
			{
				//mMeshStaticData::SetPickFuntion(AddPick);
				//mMeshStaticData::_pickSoloOrMutiply = NoPick;
				//if ((mMeshStaticData::_pickFilter == PickAdjustNum))
				{

				}
				if (event->modifiers() == Qt::ControlModifier)//��ѡ
				{
					//mMeshStaticData::_pickSoloOrMutiply = SoloPick;
				}
				else if (event->modifiers() == Qt::ShiftModifier)//���ο�ѡ
				{
					//mMeshStaticData::_pickSoloOrMutiply = MultiplyPick;
					//if (mMeshStaticData::_mutiplyPickStatus)
					{
						left_up = QVector2D(Posx_Firstmouse, Posy_Firstmouse);
						left_down = QVector2D(Posx_Firstmouse, Posy_Firstmouse);
						right_down = QVector2D(Posx_Firstmouse, Posy_Firstmouse);
						right_up = QVector2D(Posx_Firstmouse, Posy_Firstmouse);
					}
				}
				else if (event->modifiers() == Qt::AltModifier)//Բ�ο�ѡ
				{
					//mMeshStaticData::_pickSoloOrMutiply = RoundPick;
					//if (mMeshStaticData::_mutiplyPickStatus)
					{
						//roundCenter = QVector2D(Posx_Firstmouse, Posy_Firstmouse);
						//roundPoint = QVector2D(Posx_Firstmouse, Posy_Firstmouse);
					}
				}
			}

			//�����ת����Ϊ��Ļ���ģ���ôÿ��ͶӰ�任��Ҫ��ȡ����λ�ã�
			//if (ifRotateAtViewCenter == true && ifZoomAtFrameCenter == false && mMeshStaticData::_pickSoloOrMutiply == NoPick)
			{
				//SetRotateCenterToViewCenter();
			}


		}

		else if (event->button() == Qt::RightButton)
		{
			isFirstMouse = true;
			isRightMousePress = true;
			Posx_Firstmouse = event->pos().x();
			Posy_Firstmouse = event->pos().y();
			//mMeshStaticData::SetPickFuntion(ReducePick);
			//mMeshStaticData::_pickSoloOrMutiply = NoPick;
			if (event->modifiers() == Qt::ControlModifier)//��ѡ
			{
				//mMeshStaticData::_pickSoloOrMutiply = SoloPick;
			}
			else if (event->modifiers() == Qt::ShiftModifier)//��ѡ
			{
				//mMeshStaticData::_pickSoloOrMutiply = MultiplyPick;
				//if (mMeshStaticData::_mutiplyPickStatus)
				{
					left_up = QVector2D(Posx_Firstmouse, Posy_Firstmouse);
					left_down = QVector2D(Posx_Firstmouse, Posy_Firstmouse);
					right_down = QVector2D(Posx_Firstmouse, Posy_Firstmouse);
					right_up = QVector2D(Posx_Firstmouse, Posy_Firstmouse);
				}
			}
			else if (event->modifiers() == Qt::AltModifier)//Բ�ο�ѡ
			{
				//mMeshStaticData::_pickSoloOrMutiply = RoundPick;
				//if (mMeshStaticData::_mutiplyPickStatus)
				//{
				//	roundCenter = QVector2D(Posx_Firstmouse, Posy_Firstmouse);
				//	roundPoint = QVector2D(Posx_Firstmouse, Posy_Firstmouse);
				//}
			}
		}

		update();
	}
	void mBaseRend::mouseReleaseEvent(QMouseEvent *event)
	{
		if (event->button() == Qt::MiddleButton)
		{
			isMiddleMousePress = GL_FALSE;
			isFirstMouse = GL_TRUE;

		}

		if (event->button() == Qt::LeftButton)
		{
			nowX = event->pos().x();
			nowY = event->pos().y();
			//if ((mMeshStaticData::_pickFilter == PickAdjustNum))
			{
				//if (_tempMeshRend->PickOnLineNum(0, event->pos()))//�����
				{
					//mGlobalSignals::getInstance()->finishPickingAdjustNumSig(0);
				}
			}
			//else if ((mMeshStaticData::_pickFilter == PickBiasNum))
			{
				//if (_tempMeshRend->PickOnLineNum(0, event->pos()))//�����
				{
					//mGlobalSignals::getInstance()->finishPickingBiasNumSig(0);
				}
			}
			//else if (mMeshStaticData::_pickSoloOrMutiply == SoloPick)
			{
				//float depth;
				//makeCurrent();
				//glReadPixels(event->pos().x(), SCR_HEIGHT - event->pos().y(), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
				//if (mMeshStaticData::_pickObject == Mesh)
				{
					//_meshRend->SoloPickOnModel(event->pos(), depth);
				}
				//else if (mMeshStaticData::_pickObject == Geometry)
				{
					//_geoRend->SoloPickOnModel(event->pos());
				}
			}
			//else if (mMeshStaticData::_mutiplyPickStatus)
			{
				//if (mMeshStaticData::_pickSoloOrMutiply == MultiplyPick)
				{
					int Width = fabs(nowX - Posx_Firstmouse);
					int Height = fabs(nowY - Posy_Firstmouse);
					int PosX = 0.5*(nowX + Posx_Firstmouse);
					int PosY = 0.5*(nowY + Posy_Firstmouse);
					//if (mMeshStaticData::_pickObject == Mesh)
					{
						//_meshRend->MultiplyPickOnModel(PosX, PosY, Width, Height, _quadRend->pickQuad);
					}
					//else if (mMeshStaticData::_pickObject == Geometry)
					{
						//_geoRend->MultiplyPickOnModel(PosX, PosY, Width, Height, _quadRend->pickQuad);
					}
				}
				//else if (mMeshStaticData::_pickSoloOrMutiply == RoundPick)
				{
					//if (mMeshStaticData::_pickObject == Mesh)
					{
						//_meshRend->RoundPickOnModel((nowX + Posx_Firstmouse) / 2.0, (nowY + Posy_Firstmouse) / 2.0, nowX, nowY);
					}
					//else if (mMeshStaticData::_pickObject == Geometry)
					{
						//_geoRend->RoundPickOnModel((nowX + Posx_Firstmouse) / 2.0, (nowY + Posy_Firstmouse) / 2.0, nowX, nowY);
					}
				}
			}
			//�����ź�
			//mGlobalSignals::getInstance()->finishPickingSig();
			//���ݾ��ο����Ľ������ţ�����ͷź󴫵�����
			if (ifZoomAtFrameCenter == true)
			{
				_modelView->ZoomAtFrameCenter((Posx_Firstmouse + nowX) / 2, (Posy_Firstmouse + nowY) / 2);
				//HideOrShowAllFont();
				//_meshModelRulerRend->UpdateNum();
				ifZoomAtFrameCenter = false;
			}
			//�����ȡ���ص����꣬����������ת����
			if (ifGetRotateCenter == true)
			{
				int posx = event->pos().x(); //QT��������ԭ�����Ͻ�
				int posy = event->pos().y();
				float Depth = 0.0;
				if (abs(BufDepth - 1.0) < 0.1)
					Depth = 0;
				else
					Depth = (BufDepth - 0.5) * 2;//�����ֵ��Χ��0.1ת����-1.1
				//���ӵ��ģ������
				QVector3D Center = mViewToolClass::PixelToModelPosition(posx, posy, _modelView->_projection, _modelView->_view, _modelView->_model, SCR_WIDTH, SCR_HEIGHT, Depth);
				//��ȡ�����ת�뾶
				float maxRadius = mViewToolClass::GetMaxRadius(_left, _right, _bottom, _top, _back, _front, Center);
				_modelView->SetRotateCenterToPoint(Center, maxRadius);
				_center_now = Center;
				//mRenderData::GetInstance()->appendRotateCenterData("RotateCenter", Center);
				ifGetRotateCenter = false;
			}
			else
			{
				//������ת��������
				//mRenderData::GetInstance()->setRotateCenterShowState("RotateCenter", false);//����
			}


			isFirstMouse = GL_TRUE;
			isLeftMousePress = GL_FALSE;
		}


		if (event->button() == Qt::RightButton)
		{
			nowX = event->pos().x();
			nowY = event->pos().y();
			//if ((mMeshStaticData::_pickFilter == PickAdjustNum))
			{
				//if (_tempMeshRend->PickOnLineNum(1, event->pos()))//�Ҽ���
				{
					//mGlobalSignals::getInstance()->finishPickingAdjustNumSig(1);
				}
			}
			//else if ((mMeshStaticData::_pickFilter == PickBiasNum))
			{
				//if (_tempMeshRend->PickOnLineNum(1, event->pos()))//�����
				{
					//mGlobalSignals::getInstance()->finishPickingBiasNumSig(1);
				}
			}
			//else if (mMeshStaticData::_pickSoloOrMutiply == SoloPick)
			{
				float depth;
				makeCurrent();
				glReadPixels(event->pos().x(), SCR_HEIGHT - event->pos().y(), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
				//if (mMeshStaticData::_pickObject == Mesh)
				{
					//_meshRend->SoloPickOnModel(event->pos(), depth);
				}
				//else if (mMeshStaticData::_pickObject == Geometry)
				{
					//_geoRend->SoloPickOnModel(event->pos());
				}
			}
			//else if (mMeshStaticData::_mutiplyPickStatus)
			{
				//if (mMeshStaticData::_pickSoloOrMutiply == MultiplyPick)
				{
					int Width = fabs(nowX - Posx_Firstmouse);
					int Height = fabs(nowY - Posy_Firstmouse);
					int PosX = 0.5*(nowX + Posx_Firstmouse);
					int PosY = 0.5*(nowY + Posy_Firstmouse);
				//	if (mMeshStaticData::_pickObject == Mesh)
				//	{
				//		_meshRend->MultiplyPickOnModel(PosX, PosY, Width, Height, _quadRend->pickQuad);
				//	}
				//	else if (mMeshStaticData::_pickObject == Geometry)
				//	{
				//		_geoRend->MultiplyPickOnModel(PosX, PosY, Width, Height, _quadRend->pickQuad);
				//	}
				//}
				//else if (mMeshStaticData::_pickSoloOrMutiply == RoundPick)
				//{
				//	if (mMeshStaticData::_pickObject == Mesh)
				//	{
				//		_meshRend->RoundPickOnModel((nowX + Posx_Firstmouse) / 2.0, (nowY + Posy_Firstmouse) / 2.0, nowX, nowY);
				//	}
				//	else if (mMeshStaticData::_pickObject == Geometry)
				//	{
				//		_geoRend->RoundPickOnModel((nowX + Posx_Firstmouse) / 2.0, (nowY + Posy_Firstmouse) / 2.0, nowX, nowY);
				//	}
				}
			}
			isFirstMouse = GL_TRUE;
			isRightMousePress = false;
			//mRenderData::GetInstance()->setRotateCenterShowState("RotateCenter", false);//����
		}
		left_up = QVector2D(0, 0);
		left_down = QVector2D(0, 0);
		right_down = QVector2D(0, 0);
		right_up = QVector2D(0, 0);
		//roundCenter = QVector2D(0, 0);
		//roundPoint = QVector2D(0, 0);
		update();
	}

	void mBaseRend::mouseMoveEvent(QMouseEvent *event)
	{
		isMouseMove = true;
		nowX = event->pos().x();
		nowY = event->pos().y();

		//��ѡ�Ŵ󣨴��ݾ��ο������
		if (isLeftMousePress && ifZoomAtFrameCenter == true)
		{
			left_down = QVector2D(Posx_Firstmouse, nowY);
			right_down = QVector2D(nowX, nowY);
			right_up = QVector2D(nowX, Posy_Firstmouse);
		}

		if (isLeftMousePress && ifZoomAtFrameCenter == false/* && mMeshStaticData::_pickSoloOrMutiply == NoPick*/)
		{
			if (isFirstMouse)
			{
				lastX = nowX;
				lastY = nowY;
				isFirstMouse = GL_FALSE;
			}

			GLint xoffset = lastX - nowX;//����X����ƫ����(�ƶ�����)
			GLint yoffset = nowY - lastY;// ����Y����ƫ����
			lastX = nowX;
			lastY = nowY;

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
			else if (ifTranslateXY == true && ifGetRotateCenter == false)
			{
				_modelView->Translate(xoffset, yoffset);
			}
			else if (ifZoomByMouseMove == true && ifGetRotateCenter == false)
			{
				_modelView->ZoomAtViewCenter_ByMove(Posx_Firstmouse, Posy_Firstmouse, nowX, nowY);
			}

			//������ת�����Ƿ���ʾ
			//if (ifTranslateXY == true || ifZoomByMouseMove == true)
			//{
			//	mRenderData::GetInstance()->setRotateCenterShowState("RotateCenter", false);//����
			//}
			//if (ifRotateAtXY == true || ifRotateAtZ == true)
			//{
			//	mRenderData::GetInstance()->setRotateCenterShowState("RotateCenter", true);//��ʾ
			//}
		}
		//else if ((isRightMousePress || isLeftMousePress)/* && mMeshStaticData::_pickSoloOrMutiply == MultiplyPick*/)
		//{
			//if (mMeshStaticData::_mutiplyPickStatus)
			//{
				//left_down = QVector2D(Posx_Firstmouse, nowY);
				//right_down = QVector2D(nowX, nowY);
				//right_up = QVector2D(nowX, Posy_Firstmouse);
			//}
		//}
		//else if ((isRightMousePress || isLeftMousePress)/* && mMeshStaticData::_pickSoloOrMutiply == RoundPick*/)
		//{
			//if (mMeshStaticData::_mutiplyPickStatus)
			//{
				//left_down = QVector2D(Posx_Firstmouse, nowY);
				//right_down = QVector2D(nowX, nowY);
				//right_up = QVector2D(nowX, Posy_Firstmouse);
				//roundCenter = QVector2D((nowX + Posx_Firstmouse) / 2.0, (nowY + Posy_Firstmouse) / 2.0);
				//roundPoint = QVector2D(nowX, nowY);
			//}
		//}
		//�Ҽ�ƽ��
		else if (isRightMousePress/* && mMeshStaticData::_pickSoloOrMutiply == NoPick*/)
		{
			if (isFirstMouse)
			{
				lastX = nowX;
				lastY = nowY;
				isFirstMouse = GL_FALSE;
			}

			GLint xoffset = lastX - nowX;//����X����ƫ����(�ƶ�����)
			GLint yoffset = nowY - lastY;// ����Y����ƫ����
			lastX = nowX;
			lastY = nowY;
			//������ת���Ĳ���ʾ
			//mRenderData::GetInstance()->setRotateCenterShowState("RotateCenter", false);//����

			_modelView->Translate(xoffset, yoffset);

		}
		//else
		{
			// if (mMeshStaticData::_pickFilter != PickAdjustNum&& mMeshStaticData::_pickFilter != PickBiasNum)
			// {
			// 	_meshRend->MouseMoveOnModel(ShaderCommon, event->pos());
			// }
		}
		update();

	}
	void mBaseRend::wheelEvent(QWheelEvent *event)
	{
		QPoint numDegrees = event->angleDelta();//��ȡ���ֵ�ת��
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
	void mBaseRend::GetPointDepthAtMouse()
	{
		glReadPixels(nowX, SCR_HEIGHT - nowY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &BufDepth);//OpenGL��������ԭ�����½� BufDepth��Χ��0,1��
	}
}