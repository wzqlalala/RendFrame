#include "mPreRender.h"
#include "mPreRend.h"
#include "mPreRendStatus.h"
#include "mPreGeoModelRender.h"
//#include "mPostHighLightRender.h"
#include "mFontRender.h"
#include "mArrowRender.h"

//MDataGeo
#include "mGeoPickData1.h"
#include "mPreGeoPickThread.h"

#include <renderpch.h>
#include "texture.h"
#include "Space.h"

#include "mShaderManage.h"
#include "mTextureManage.h"

#include "mGeoModelData1.h"

//视图类
#include "mModelView.h"
#include "mViewBase.h"

#include <QMatrix3x3>
#include <QFileDialog>
#include <QFileInfo>
#include <QApplication>
#include <QtConcurrent/QtConcurrent>
#include <QThread>



#define _USE_MATH_DEFINES
#include <math.h>

using namespace mxr;
using namespace std;
namespace MPreRend
{
	mPreRender::mPreRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mPreRend *preRend):mBaseRender(app, parent, preRend)
	{
		this->makeCurrent();

		//QOpenGLContext *context = QOpenGLContext::currentContext();
		//qDebug() << "mPreRender" << QString::number(long long int(context), 16);

		_rendStatus = make_shared<mPreRendStatus>();
		_pointTexture = mTextureManage::GetInstance()->GetTexture("GeoPoint.png", 3);
		_geoModelData = new mGeoModelData1();
		_geoModelRender = MakeAsset<mPreGeoModelRender>(parent, _rendStatus, _geoModelData);


		/**********************************************************模型**********************************************************/
		//face
		_faceStateSet = MakeAsset<StateSet>();
		shared_ptr<Shader> shader = mShaderManage::GetInstance()->GetShader("PreGeoMeshFace");
		_faceStateSet->setShader(shader);
		_faceStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_faceStateSet->setAttributeAndModes(MakeAsset<PolygonOffsetFill>(0, 0), 0);
		_faceStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_faceStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);

		_faceStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_faceStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_faceStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_faceStateSet->setUniform(MakeAsset<Uniform>("lightIsOn", int(1)));
		_faceStateSet->setUniform(MakeAsset<Uniform>("viewPos", QVector3D()));
		_faceStateSet->setUniform(MakeAsset<Uniform>("light.position", _rendStatus->_postLight.lightPosition));
		_faceStateSet->setUniform(MakeAsset<Uniform>("light.ambient", _rendStatus->_postLight.ambient));
		_faceStateSet->setUniform(MakeAsset<Uniform>("light.diffuse", _rendStatus->_postLight.diffuse));
		_faceStateSet->setUniform(MakeAsset<Uniform>("light.specular", _rendStatus->_postLight.specular));
		_faceStateSet->setUniform(MakeAsset<Uniform>("light.shiness", _rendStatus->_postLight.shiness));

		//edgeline
		_edgelineStateSet = MakeAsset<StateSet>();
		shader = mShaderManage::GetInstance()->GetShader("PreGeoMeshEdgeLine");
		_edgelineStateSet->setShader(shader);
		_edgelineStateSet->setDrawMode(GL_LINES);
		_edgelineStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_edgelineStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(mxr::PolygonMode::FRONT_AND_BACK, mxr::PolygonMode::FILL), 1);
		_edgelineStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), 1);

		_edgelineStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("showColor", _rendStatus->_faceLineColor));
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("rightToLeft", 0));
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("lineWidth", 1.0f));

		//faceline
		//_facelineStateSet = MakeAsset<StateSet>();
		//shader = mShaderManage::GetInstance()->GetShader("PostMeshFaceLineWithDeformation");
		//_facelineStateSet->setShader(shader);
		//_facelineStateSet->setDrawMode(GL_TRIANGLES);
		//_facelineStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		//_facelineStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(PolygonMode::FRONT_AND_BACK, PolygonMode::LINE), 1);
		//_facelineStateSet->setAttributeAndModes(MakeAsset<PolygonOffsetLine>(-1, -1), 1);
		//_facelineStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), 1);

		//_facelineStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		//_facelineStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		//_facelineStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		//_facelineStateSet->setUniform(MakeAsset<Uniform>("showColor", _rendStatus->_faceLineColor));
		//_facelineStateSet->setUniform(MakeAsset<Uniform>("rightToLeft", 0));

		//line
		_independentlineStateSet = MakeAsset<StateSet>();
		shader = mShaderManage::GetInstance()->GetShader("PreGeoMeshLine");
		_independentlineStateSet->setShader(shader);
		_independentlineStateSet->setDrawMode(GL_LINES);
		_independentlineStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_independentlineStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_independentlineStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);

		_independentlineStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_independentlineStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_independentlineStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));

		//dotline
		_dotlineStateSet = MakeAsset<StateSet>();
		shader = mShaderManage::GetInstance()->GetShader("PreGeoMeshDotLine");
		_dotlineStateSet->setShader(shader);
		_dotlineStateSet->setDrawMode(GL_LINES);
		_dotlineStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_dotlineStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_dotlineStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);

		_dotlineStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_dotlineStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_dotlineStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));

		//point
		_pointStateSet = MakeAsset<StateSet>();
		shader = mShaderManage::GetInstance()->GetShader("PreGeoPoint");
		_pointStateSet->setShader(shader);
		_pointStateSet->setDrawMode(GL_POINTS);
		_pointStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_pointStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_pointStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);

		_pointStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_pointStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_pointStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_pointStateSet->setUniform(MakeAsset<Uniform>("lightIsOn", int(1)));
		_pointStateSet->setUniform(MakeAsset<Uniform>("viewPos", QVector3D()));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.position", _rendStatus->_postLight.lightPosition));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.ambient", _rendStatus->_postLight.ambient));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.diffuse", _rendStatus->_postLight.diffuse));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.specular", _rendStatus->_postLight.specular));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.shiness", _rendStatus->_postLight.shiness));
		_pointStateSet->setUniform(MakeAsset<Uniform>("PointSize", 10));
		_pointStateSet->setTexture("sprite_texture",_pointTexture);

		_geoModelRender->setFaceStateSet(_faceStateSet);
		_geoModelRender->setIndependentLineStateSet(_independentlineStateSet);
		_geoModelRender->setDotLineStateSet(_dotlineStateSet);
		_geoModelRender->setEdgeLineStateSet(_edgelineStateSet);
		_geoModelRender->setPointStateSet(_pointStateSet);

		//初始化高亮渲染
		_pickData = new mGeoPickData1();
		_geoPickThread = new mPreGeoPickThread(_geoModelData, _pickData);
		_geoPickThread->setPickFilter(_baseRend->getPickFilter());
		//_highLightRender = make_shared<mPostHighLightRender>(_rendStatus, _pickData);

		//this->doneCurrent();
	}
	bool mPreRender::getIsDragSomething(QVector2D pos)
	{
		//判断是否有物体被拖拽
		return false;
	}
	void mPreRender::dragSomething(QVector2D pos)
	{
		//拖拽物体并更新物体
	}
	void mPreRender::startPick(QVector<QVector2D> poses)
	{
		makeCurrent();
		//开始拾取操作
		if (!_geoModelRender)
		{
			return;
		}
		_pickData->setGeoPickFunction(int(_baseRend->getPickFuntion()));
		//_thread->setCurrentFrameRend(_oneFrameRender->getOneFrameData(), _oneFrameRender->getOneFrameRendData());
		_geoPickThread->setMatrix(_baseRend->getCamera()->getPVMValue());
		_geoPickThread->setWidget(_baseRend->getCamera()->SCR_WIDTH, _baseRend->getCamera()->SCR_HEIGHT);
		_geoPickThread->setPickMode(*_baseRend->getCurrentPickMode(), *_baseRend->getMultiplyPickMode());
		if (*_baseRend->getCurrentPickMode() == PickMode::SoloPick)
		{		
			float depth;
			QOpenGLContext::currentContext()->functions()->glReadPixels(poses.first().x() - 1, _baseRend->getCamera()->SCR_HEIGHT - poses.first().y() - 1, 2, 2, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
			_geoPickThread->setLocation(poses.first().toPoint(), depth);
		}
		else
		{
			_geoPickThread->setLocation(poses, (_baseRend->getCamera()->_Center - _baseRend->getCamera()->_Eye).normalized());
		}
		QFuture<void> future; 
		future = QtConcurrent::run(_geoPickThread, &mPreGeoPickThread::startPick);
		QObject::connect(&w, &QFutureWatcher<void>::finished, [this] {
			//_highLightRender->updateHighLightRender(_oneFrameRender->getOneFrameData(), _oneFrameRender->getOneFrameRendData());
			//this->
			set<int> ids = _pickData->getPickPointIDs();
			qDebug() << "拾取完成";
			QObject::disconnect(&w, 0, 0, 0);//断开信号
			emit update();
		});
		w.setFuture(future);

	}

	bool mPreRender::getModelAABB(Space::AABB &aabb)
	{
		bool hasModel = false;
		if (_geoModelData->isShowModel())
		{
			aabb.push(_geoModelData->getModelSize());
			hasModel = true;
		}
		if (true)
		{

		}
		return hasModel;
	}

	void mPreRender::updateHighLightRender()
	{
		this->makeCurrent();
		//if (_highLightRender)
		//	_highLightRender->updateHighLightRender(_oneFrameRender->getOneFrameData(), _oneFrameRender->getOneFrameRendData());
	}
	void mPreRender::updateModelOperate(QPair<MBasicFunction::ModelOperateEnum, std::set<QString>> modelOperates)
	{
		this->makeCurrent();
		if (_geoModelRender)
		{
			_geoModelRender->updateModelOperate(modelOperates);
		}
	}
	void mPreRender::clearRender()
	{
		this->makeCurrent();
		//_dataPost.reset();
		//QOpenGLContext *context = QOpenGLContext::currentContext();
		//qDebug() << "deletecontext" << QString::number(long long int(context), 16);
		//if (_oneFrameRender)
		//{
		//	_oneFrameRender->deleteThieFrame();
		//	_oneFrameRender.reset();
		//}
		//deleteAnimation();
		_baseRend->getFontRender()->clearAllRender();
		_baseRend->getArrowRender()->clearAllRender();
	}
	
	void mPreRender::setShowFuntion(ShowFuntion showFuntion)
	{
		this->makeCurrent();
		_rendStatus->_showFunction = showFuntion;
		if (_geoModelRender)
		{
			_geoModelRender->setShowFuntion(showFuntion);
		}
	}

	void mPreRender::setPointSize(int size)
	{
		_rendStatus->_pointSize = size;
		_pointStateSet->getUniform("PointSize")->SetData(_rendStatus->_pointSize);
	}

	void mPreRender::setEdgeLineWidth(float size)
	{
		_rendStatus->_lineWidth = size;
		_edgelineStateSet->getUniform("lineWidth")->SetData(_rendStatus->_lineWidth);
	}

	void mPreRender::setFaceLineColor(QVector4D color)
	{
		_rendStatus->_faceLineColor = color;
		_facelineStateSet->getUniform("showColor")->SetData(_rendStatus->_faceLineColor);
	}

	mPreRender::~mPreRender()
	{
		this->makeCurrent();
		_geoModelRender.reset();
		
	}

	void mPreRender::initialPickThreads()
	{
		//if (!_dataPost)
		//{
		//	return;
		//}

		////初始化部件拾取多线程
		//set<QString> partNames = _dataPost->getAllPostPartNames();
		//_thread = new mPostMeshPickThread(_pickData);
		//_thread->setPickFilter(_baseRend->getPickFilter());
		//for (QString partName : partNames)
		//{
		//	_thread->appendPartSpaceTree(partName, _oneFrameRender->getModelRender()->getPartSpaceTree(partName));
		//}	
	}

	void mPreRender::updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView)
	{
		if (_faceStateSet)
		{
			_faceStateSet->getUniform("projection")->SetData(modelView->_projection);
			_faceStateSet->getUniform("view")->SetData(modelView->_view);
			_faceStateSet->getUniform("model")->SetData(modelView->_model);
			_edgelineStateSet->getUniform("projection")->SetData(modelView->_projection);
			_edgelineStateSet->getUniform("view")->SetData(modelView->_view);
			_edgelineStateSet->getUniform("model")->SetData(modelView->_model);
			//_facelineStateSet->getUniform("projection")->SetData(modelView->_projection);
			//_facelineStateSet->getUniform("view")->SetData(modelView->_view);
			//_facelineStateSet->getUniform("model")->SetData(modelView->_model);
			_independentlineStateSet->getUniform("projection")->SetData(modelView->_projection);
			_independentlineStateSet->getUniform("view")->SetData(modelView->_view);
			_independentlineStateSet->getUniform("model")->SetData(modelView->_model);
			_dotlineStateSet->getUniform("projection")->SetData(modelView->_projection);
			_dotlineStateSet->getUniform("view")->SetData(modelView->_view);
			_dotlineStateSet->getUniform("model")->SetData(modelView->_model);
			_pointStateSet->getUniform("projection")->SetData(modelView->_projection);
			_pointStateSet->getUniform("view")->SetData(modelView->_view);
			_pointStateSet->getUniform("model")->SetData(modelView->_model);			

			_faceStateSet->getUniform("viewPos")->SetData(modelView->_Eye);
			_pointStateSet->getUniform("viewPos")->SetData(modelView->_Eye);
			if (_rendStatus->_lightIsDependOnCamera)
			{
				_faceStateSet->getUniform("light.position")->SetData(2 * modelView->_Eye - modelView->_Center);
				_pointStateSet->getUniform("light.position")->SetData(2 * modelView->_Eye - modelView->_Center);
			}
			else
			{
				_faceStateSet->getUniform("light.position")->SetData(_rendStatus->_postLight.lightPosition);
				_pointStateSet->getUniform("light.position")->SetData(_rendStatus->_postLight.lightPosition);
			}

			//_facelineStateSet->getUniform("rightToLeft")->SetData(float(modelView->_Right - modelView->_Left));
			_edgelineStateSet->getUniform("rightToLeft")->SetData(float(modelView->_Right - modelView->_Left));
		}
		//检测模型数据更新模型渲染
		bool isUpdateCamera{ false };
		if (_geoModelRender)
		{
			isUpdateCamera = isUpdateCamera | _geoModelRender->updateRender();
		}
		if (isUpdateCamera)
		{
			_baseRend->slotUpdateOrthoAndCamera();
		}

		//_highLightRender->updateUniform(modelView, commonView);
	}
}