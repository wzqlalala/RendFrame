#include "mPostRender.h"
#include "mPostRend.h"
#include "mPostRendStatus.h"
#include "mPostOneFrameRender.h"
#include "mPostModelRender.h"
#include "mPostAnimationRender.h"

#include <renderpch.h>
#include "texture.h"
#include "Space.h"

#include "mShaderManage.h"
#include "mTextureManage.h"

//视图类
#include "mModelView.h"
#include "mCommonView.h"

#include <QMatrix3x3>
#include <QFileDialog>
#include <QFileInfo>
#include <QApplication>
#include <QtConcurrent/QtConcurrent>
#include <QThread>

//MDataPost
#include "mDataPost1.h"
#include "mOneFrameData1.h"
#include "mPostOneFrameRendData.h"
#include "mPostAnimationRendData.h"
#include "mPostColorTableData.h"
#include "mPostMeshPickThread.h"
#include "mPostMeshPickData.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace mxr;
using namespace std;
namespace MPostRend
{
	mPostRender::mPostRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mPostRend *postRend):mBaseRender(app, parent, postRend)
	{
		this->makeCurrent();

		//QOpenGLContext *context = QOpenGLContext::currentContext();
		//qDebug() << "mPostRender" << QString::number(long long int(context), 16);
		_dataPost = nullptr;
		_texture = nullptr;
		_oneFrameRender = nullptr;
		_oneFrameAnimationRender = nullptr;
		//_animationId = 0;
		//_animationRender = nullptr;

		_rendStatus = make_shared<mPostRendStatus>();

		/**********************************************************模型**********************************************************/
		//face
		_faceStateSet = MakeAsset<StateSet>();
		shared_ptr<Shader> faceshader = mShaderManage::GetInstance()->GetShader("PostMeshFaceWithDeformation");
		_faceStateSet->setShader(faceshader);
		_faceStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_faceStateSet->setAttributeAndModes(MakeAsset<PolygonOffsetFill>(0, 0), 0);
		_faceStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_faceStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);

		_faceStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_faceStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_faceStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_faceStateSet->setUniform(MakeAsset<Uniform>("planes[0]", QVector4D()), false);
		_faceStateSet->setUniform(MakeAsset<Uniform>("planes[1]", QVector4D()), false);
		_faceStateSet->setUniform(MakeAsset<Uniform>("planes[2]", QVector4D()), false);
		_faceStateSet->setUniform(MakeAsset<Uniform>("planes[3]", QVector4D()), false);
		_faceStateSet->setUniform(MakeAsset<Uniform>("planes[4]", QVector4D()), false);
		_faceStateSet->setUniform(MakeAsset<Uniform>("planes[5]", QVector4D()), false);
		_faceStateSet->setUniform(MakeAsset<Uniform>("planes[6]", QVector4D()), false);
		_faceStateSet->setUniform(MakeAsset<Uniform>("planes[7]", QVector4D()), false);
		_faceStateSet->setUniform(MakeAsset<Uniform>("lightIsOn", int(1)));
		_faceStateSet->setUniform(MakeAsset<Uniform>("viewPos", QVector3D()));
		_faceStateSet->setUniform(MakeAsset<Uniform>("light.position", _rendStatus->_postLight.lightPosition));
		_faceStateSet->setUniform(MakeAsset<Uniform>("light.ambient", _rendStatus->_postLight.ambient));
		_faceStateSet->setUniform(MakeAsset<Uniform>("light.diffuse", _rendStatus->_postLight.diffuse));
		_faceStateSet->setUniform(MakeAsset<Uniform>("light.specular", _rendStatus->_postLight.specular));
		_faceStateSet->setUniform(MakeAsset<Uniform>("light.shiness", _rendStatus->_postLight.shiness));
		_faceStateSet->setUniform(MakeAsset<Uniform>("deformationScale", QVector3D()));
		_faceStateSet->setUniform(MakeAsset<Uniform>("minValue", float(0)));
		_faceStateSet->setUniform(MakeAsset<Uniform>("maxValue", float(0)));
		_faceStateSet->setUniform(MakeAsset<Uniform>("isEquivariance", int(0)));
		_faceStateSet->setUniform(MakeAsset<Uniform>("textureCoordRatio", float(0)));

		//face透明
		_faceTransparentStateSet = MakeAsset<StateSet>();
		shared_ptr<Shader> facetransparentshader = mShaderManage::GetInstance()->GetShader("PostMeshFaceTransparentWithDeformation");
		_faceTransparentStateSet->setShader(facetransparentshader);
		_faceTransparentStateSet->setAttributeAndModes(MakeAsset<Depth>(), 0);
		_faceTransparentStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_faceTransparentStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), 1);

		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("planes[0]", QVector4D()), false);
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("planes[1]", QVector4D()), false);
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("planes[2]", QVector4D()), false);
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("planes[3]", QVector4D()), false);
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("planes[4]", QVector4D()), false);
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("planes[5]", QVector4D()), false);
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("planes[6]", QVector4D()), false);
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("planes[7]", QVector4D()), false);
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("lightIsOn", int(1)));
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("viewPos", QVector3D()));
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("light.position", _rendStatus->_postLight.lightPosition));
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("light.ambient", _rendStatus->_postLight.ambient));
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("light.diffuse", _rendStatus->_postLight.diffuse));
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("light.specular", _rendStatus->_postLight.specular));
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("light.shiness", _rendStatus->_postLight.shiness));
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("deformationScale", QVector3D()));

		//face透明无变形
		_faceTransparentNodeformationStateSet = MakeAsset<StateSet>();
		shared_ptr<Shader> facetransparentNodeformationshader = mShaderManage::GetInstance()->GetShader("PostMeshFaceTransparentWithOutDeformation");
		_faceTransparentNodeformationStateSet->setShader(facetransparentNodeformationshader);
		_faceTransparentNodeformationStateSet->setAttributeAndModes(MakeAsset<Depth>(), 0);
		_faceTransparentNodeformationStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_faceTransparentNodeformationStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), 1);

		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("planes[0]", QVector4D()), false);
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("planes[1]", QVector4D()), false);
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("planes[2]", QVector4D()), false);
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("planes[3]", QVector4D()), false);
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("planes[4]", QVector4D()), false);
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("planes[5]", QVector4D()), false);
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("planes[6]", QVector4D()), false);
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("planes[7]", QVector4D()), false);
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("lightIsOn", int(1)));
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("viewPos", QVector3D()));
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("light.position", _rendStatus->_postLight.lightPosition));
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("light.ambient", _rendStatus->_postLight.ambient));
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("light.diffuse", _rendStatus->_postLight.diffuse));
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("light.specular", _rendStatus->_postLight.specular));
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("light.shiness", _rendStatus->_postLight.shiness));

		//edgeline
		_edgelineStateSet = MakeAsset<StateSet>();
		shared_ptr<Shader> meshlineshader = mShaderManage::GetInstance()->GetShader("PostEdgeLineWithDeformation");
		_edgelineStateSet->setShader(meshlineshader);
		_edgelineStateSet->setDrawMode(GL_LINES);
		_edgelineStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_edgelineStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(mxr::PolygonMode::FRONT_AND_BACK, mxr::PolygonMode::FILL), 1);
		_edgelineStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), 1);

		_edgelineStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("planes[0]", QVector4D()), false);
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("planes[1]", QVector4D()), false);
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("planes[2]", QVector4D()), false);
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("planes[3]", QVector4D()), false);
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("planes[4]", QVector4D()), false);
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("planes[5]", QVector4D()), false);
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("planes[6]", QVector4D()), false);
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("planes[7]", QVector4D()), false);
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("deformationScale", QVector3D()));
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("showColor", _rendStatus->_faceLineColor));
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("rightToLeft", 0));
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("lineWidth", 1.0f));

		//faceline
		_facelineStateSet = MakeAsset<StateSet>();
		shared_ptr<Shader> facelineshader = mShaderManage::GetInstance()->GetShader("PostMeshFaceLineWithDeformation");
		_facelineStateSet->setShader(facelineshader);
		_facelineStateSet->setDrawMode(GL_TRIANGLES);
		_facelineStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_facelineStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(PolygonMode::FRONT_AND_BACK, PolygonMode::LINE), 1);
		_facelineStateSet->setAttributeAndModes(MakeAsset<PolygonOffsetLine>(-1, -1), 1);
		_facelineStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), 1);

		_facelineStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_facelineStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_facelineStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_facelineStateSet->setUniform(MakeAsset<Uniform>("planes[0]", QVector4D()), false);
		_facelineStateSet->setUniform(MakeAsset<Uniform>("planes[1]", QVector4D()), false);
		_facelineStateSet->setUniform(MakeAsset<Uniform>("planes[2]", QVector4D()), false);
		_facelineStateSet->setUniform(MakeAsset<Uniform>("planes[3]", QVector4D()), false);
		_facelineStateSet->setUniform(MakeAsset<Uniform>("planes[4]", QVector4D()), false);
		_facelineStateSet->setUniform(MakeAsset<Uniform>("planes[5]", QVector4D()), false);
		_facelineStateSet->setUniform(MakeAsset<Uniform>("planes[6]", QVector4D()), false);
		_facelineStateSet->setUniform(MakeAsset<Uniform>("planes[7]", QVector4D()), false);
		_facelineStateSet->setUniform(MakeAsset<Uniform>("deformationScale", QVector3D()));
		_facelineStateSet->setUniform(MakeAsset<Uniform>("showColor", _rendStatus->_faceLineColor));
		_facelineStateSet->setUniform(MakeAsset<Uniform>("rightToLeft", 0));
		//glLineWidth(10.0);

		//line
		_lineStateSet = MakeAsset<StateSet>();
		shared_ptr<Shader> lineshader = mShaderManage::GetInstance()->GetShader("PostMeshLineWithDeformation");
		_lineStateSet->setShader(lineshader);
		_lineStateSet->setDrawMode(GL_LINES);
		_lineStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_lineStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_lineStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);

		_lineStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_lineStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_lineStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_lineStateSet->setUniform(MakeAsset<Uniform>("planes[0]", QVector4D()), false);
		_lineStateSet->setUniform(MakeAsset<Uniform>("planes[1]", QVector4D()), false);
		_lineStateSet->setUniform(MakeAsset<Uniform>("planes[2]", QVector4D()), false);
		_lineStateSet->setUniform(MakeAsset<Uniform>("planes[3]", QVector4D()), false);
		_lineStateSet->setUniform(MakeAsset<Uniform>("planes[4]", QVector4D()), false);
		_lineStateSet->setUniform(MakeAsset<Uniform>("planes[5]", QVector4D()), false);
		_lineStateSet->setUniform(MakeAsset<Uniform>("planes[6]", QVector4D()), false);
		_lineStateSet->setUniform(MakeAsset<Uniform>("planes[7]", QVector4D()), false);
		_lineStateSet->setUniform(MakeAsset<Uniform>("deformationScale", QVector3D()));
		_lineStateSet->setUniform(MakeAsset<Uniform>("minValue", float(0)));
		_lineStateSet->setUniform(MakeAsset<Uniform>("maxValue", float(0)));
		_lineStateSet->setUniform(MakeAsset<Uniform>("isEquivariance", int(0)));
		_lineStateSet->setUniform(MakeAsset<Uniform>("textureCoordRatio", float(0)));

		//point
		_pointStateSet = MakeAsset<StateSet>();
		shared_ptr<Shader> pointshader = mShaderManage::GetInstance()->GetShader("PostMeshPointWithDeformation");
		_pointStateSet->setShader(pointshader);
		_pointStateSet->setDrawMode(GL_POINTS);
		_pointStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_pointStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_pointStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);

		_pointStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_pointStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_pointStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_pointStateSet->setUniform(MakeAsset<Uniform>("planes[0]", QVector4D()), false);
		_pointStateSet->setUniform(MakeAsset<Uniform>("planes[1]", QVector4D()), false);
		_pointStateSet->setUniform(MakeAsset<Uniform>("planes[2]", QVector4D()), false);
		_pointStateSet->setUniform(MakeAsset<Uniform>("planes[3]", QVector4D()), false);
		_pointStateSet->setUniform(MakeAsset<Uniform>("planes[4]", QVector4D()), false);
		_pointStateSet->setUniform(MakeAsset<Uniform>("planes[5]", QVector4D()), false);
		_pointStateSet->setUniform(MakeAsset<Uniform>("planes[6]", QVector4D()), false);
		_pointStateSet->setUniform(MakeAsset<Uniform>("planes[7]", QVector4D()), false);
		_pointStateSet->setUniform(MakeAsset<Uniform>("lightIsOn", int(1)));
		_pointStateSet->setUniform(MakeAsset<Uniform>("viewPos", QVector3D()));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.position", _rendStatus->_postLight.lightPosition));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.ambient", _rendStatus->_postLight.ambient));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.diffuse", _rendStatus->_postLight.diffuse));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.specular", _rendStatus->_postLight.specular));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.shiness", _rendStatus->_postLight.shiness));
		_pointStateSet->setUniform(MakeAsset<Uniform>("deformationScale", QVector3D()));
		_pointStateSet->setUniform(MakeAsset<Uniform>("minValue", float(0)));
		_pointStateSet->setUniform(MakeAsset<Uniform>("maxValue", float(0)));
		_pointStateSet->setUniform(MakeAsset<Uniform>("isEquivariance", int(0)));
		_pointStateSet->setUniform(MakeAsset<Uniform>("textureCoordRatio", float(0)));
		_pointStateSet->setUniform(MakeAsset<Uniform>("PointSize", _rendStatus->_pointSize));

		/**********************************************************切面**********************************************************/
		//cuttingplane
		_cuttingPlaneStateSet = MakeAsset<StateSet>();
		shared_ptr<Shader> cuttingplaneshader = mShaderManage::GetInstance()->GetShader("PostCuttingPlaneWithOutDeformation");
		_cuttingPlaneStateSet->setShader(cuttingplaneshader);
		_cuttingPlaneStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_cuttingPlaneStateSet->setAttributeAndModes(MakeAsset<PolygonOffsetFill>(1, 1), 1);
		_cuttingPlaneStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_cuttingPlaneStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);

		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("planes[0]", QVector4D()), false);
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("planes[1]", QVector4D()), false);
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("planes[2]", QVector4D()), false);
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("planes[3]", QVector4D()), false);
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("planes[4]", QVector4D()), false);
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("planes[5]", QVector4D()), false);
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("planes[6]", QVector4D()), false);
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("planes[7]", QVector4D()), false);
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("lightIsOn", int(1)));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("viewPos", QVector3D()));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("light.position", _rendStatus->_postLight.lightPosition));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("light.ambient", _rendStatus->_postLight.ambient));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("light.diffuse", _rendStatus->_postLight.diffuse));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("light.specular", _rendStatus->_postLight.specular));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("light.shiness", _rendStatus->_postLight.shiness));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("minValue", float(0)));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("maxValue", float(0)));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("isEquivariance", int(0)));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("textureCoordRatio", float(0)));

		//transparentPlane
		_transparentPlaneStateSet = MakeAsset<StateSet>();
		shared_ptr<Shader> transparentplaneshader = mShaderManage::GetInstance()->GetShader("PostTransparentPlaneWithOutDeformation");
		_transparentPlaneStateSet->setShader(transparentplaneshader);
		_transparentPlaneStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_transparentPlaneStateSet->setAttributeAndModes(MakeAsset<PolygonOffsetFill>(0, 0), 0);
		_transparentPlaneStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(mxr::PolygonMode::FRONT_AND_BACK, mxr::PolygonMode::FILL), 1);
		_transparentPlaneStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), 1);
		_transparentPlaneStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_transparentPlaneStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_transparentPlaneStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));

		//初始化计时器
		_aniTimer = new QTimer;
		_aniTimer->setInterval(0);
		connect(_aniTimer, SIGNAL(timeout()), this, SLOT(slot_aniTimer()));

		this->doneCurrent();
	}
	bool mPostRender::getIsDragSomething(QVector2D pos)
	{
		//判断是否有物体被拖拽
		return false;
	}
	void mPostRender::dragSomething(QVector2D pos)
	{
		//拖拽物体并更新物体
	}
	void mPostRender::startPick(QVector<QVector2D> poses)
	{
		makeCurrent();
		//开始拾取操作
		if (!_oneFrameRender)
		{
			return;
		}
		_thread->setCurrentFrameRend(_oneFrameRender->getOneFrameData(), _oneFrameRender->getOneFrameRendData());
		_thread->setMatrix(_baseRend->getCamera()->getPVMValue());
		_thread->setWidget(_baseRend->getCamera()->SCR_WIDTH, _baseRend->getCamera()->SCR_HEIGHT);
		_thread->setPickMode(*_baseRend->getCurrentPickMode(), *_baseRend->getMultiplyPickMode());
		if (*_baseRend->getCurrentPickMode() == PickMode::SoloPick)
		{		
			//QOpenGLContext::currentContext()->functions()->glFlush();
			//GLenum error = QOpenGLContext::currentContext()->functions()->glGetError();
			//if (error != 0)
			//{
			//	qDebug() << error;
			//}
			//qDebug() << "startPick" << QString::number(long long int(QOpenGLContext::currentContext()), 16);
			float depth;
			QOpenGLContext::currentContext()->functions()->glReadPixels(poses.first().x(), _baseRend->getCamera()->SCR_HEIGHT - poses.first().y(), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
			_thread->setLocation(poses.first().toPoint(), depth);
			//error = QOpenGLContext::currentContext()->functions()->glGetError();
			//if (error != 0)
			//{
			//	qDebug() << error;
			//}
		}

		QFuture<void> future; 
		future = QtConcurrent::run(_thread, &mPostMeshPickThread::startPick);
		QObject::connect(&w, &QFutureWatcher<void>::finished, [this] {
			//this->
			set<int> ids = _pickData->getPickNodeIDs();
			qDebug() << "拾取完成";
			QObject::disconnect(&w, 0, 0, 0);//断开信号
			
		});
		w.setFuture(future);

	}
	void mPostRender::updateOneModelOperate(QPair<MBasicFunction::PostModelOperateEnum, std::set<QString>> postModelOperates)
	{
		this->makeCurrent();
		if (_oneFrameRender)
		{
			_oneFrameRender->updateOneModelOperate(postModelOperates);
		}
		if (_oneFrameAnimationRender)
		{
			_oneFrameAnimationRender->updateOneModelOperate(postModelOperates);
		}
		for (auto rend : _animationRender)
		{
			rend->updateOneModelOperate(postModelOperates);
		}
	}
	void mPostRender::updateAllModelOperate(MBasicFunction::PostModelOperateEnum postModelOperate)
	{
		this->makeCurrent();
		if (_oneFrameRender)
		{
			_oneFrameRender->updateAllModelOperate(postModelOperate);
		}
		if (_oneFrameAnimationRender)
		{
			_oneFrameAnimationRender->updateAllModelOperate(postModelOperate);
		}
		for (auto rend : _animationRender)
		{
			rend->updateAllModelOperate(postModelOperate);
		}
	}
	void mPostRender::setPostMode(PostMode postMode)
	{
		_rendStatus->_postMode = postMode;
	}
	void mPostRender::clearRender()
	{
		this->makeCurrent();
		_dataPost.reset();
		//QOpenGLContext *context = QOpenGLContext::currentContext();
		//qDebug() << "deletecontext" << QString::number(long long int(context), 16);
		if (_oneFrameRender)
		{
			_oneFrameRender->deleteThieFrame();
			_oneFrameRender.reset();
		}
		deleteAnimation();
	}
	void mPostRender::setRendCurrentFrameData(mPostOneFrameRendData* postOneFrameRendData)
	{
		this->makeCurrent();
		if (!_dataPost || nullptr == postOneFrameRendData)
		{
			return;
		}
		int id = postOneFrameRendData->getRendID();
		mOneFrameData1 *oneFrameData = _dataPost->getOneFrameData(id);
		if (_oneFrameRender)
		{
			_oneFrameRender.reset();
		}
		_oneFrameRender = make_shared<mPostOneFrameRender>(_rendStatus, oneFrameData, postOneFrameRendData);
		if (!_texture)
		{
			mPostColorTableData *table = postOneFrameRendData->getRendColorTable();
			_texture = MakeAsset<Texture>(GL_TEXTURE_1D, table->getPostColorTableNum(), 0, 0, GL_RGB8, 1);
			_texture->SetData(0, 0, table->getPostColorTableNum(), table->getColorTable());
			_faceStateSet->setTexture("texture", _texture);
			_lineStateSet->setTexture("texture", _texture);
			_pointStateSet->setTexture("texture", _texture);
			_cuttingPlaneStateSet->setTexture("texture", _texture);
		}
		_oneFrameRender->setFaceStateSet(_faceStateSet);
		_oneFrameRender->setFaceTransparentNoDeformationStateSet(_faceTransparentNodeformationStateSet);
		_oneFrameRender->setFaceTransparentStateSet(_faceTransparentStateSet);
		_oneFrameRender->setEdgeLineStateSet(_edgelineStateSet);
		_oneFrameRender->setFaceLineStateSet(_facelineStateSet);
		_oneFrameRender->setLineStateSet(_lineStateSet);
		_oneFrameRender->setPointStateSet(_pointStateSet);
		//_oneFrameRender->setTexture(_texture);
		_oneFrameRender->updateAllModelOperate(ImportOperate);
		this->setDispersed(true);
		this->initialPickThreads();
	}

	void mPostRender::setShowFuntion(ShowFuntion showFuntion)
	{
		this->makeCurrent();
		_rendStatus->_showFunction = showFuntion;
		if (_oneFrameRender)
		{
			_oneFrameRender->getModelRender()->setShowFuntion(showFuntion);
		}
		if (_oneFrameAnimationRender)
		{
			_oneFrameAnimationRender->getModelRender()->setShowFuntion(showFuntion);
		}
		for (auto rend : _animationRender)
		{
			rend->getModelRender()->setShowFuntion(showFuntion);
		}
	}

	void mPostRender::setDispersed(bool isdispersed)
	{
		this->makeCurrent();
		_texture->Bind(_texture->ID());
		if (isdispersed)
		{
			_texture->setTexParameteri(GL_NEAREST, GL_NEAREST);
		}
		else
		{
			_texture->setTexParameteri(GL_LINEAR, GL_LINEAR);
		}
	}

	void mPostRender::setDeformationScale(QVector3D deformationScale)
	{
		this->makeCurrent();
		_rendStatus->_deformFactor = deformationScale;
		if (_oneFrameRender)
		{
			_oneFrameRender->setDeformationScale(deformationScale);
		}
		for (auto rend : _animationRender)
		{
			rend->setDeformationScale(deformationScale);
		}
	}

	void mPostRender::setIsShowInitialShape(bool isShowInitialShape)
	{
		this->makeCurrent();
		_rendStatus->_isShowInitialShape = isShowInitialShape;
		if (_oneFrameRender)
		{
			_oneFrameRender->getModelRender()->setIsShowInitialShape(isShowInitialShape);
		}
		if (_oneFrameAnimationRender)
		{
			_oneFrameAnimationRender->getModelRender()->setIsShowInitialShape(isShowInitialShape);
		}
		for (auto rend : _animationRender)
		{
			rend->getModelRender()->setIsShowInitialShape(isShowInitialShape);
		}
	}

	void mPostRender::setTextureCoordScale(float textureCoordScale)
	{
		this->makeCurrent();
		if (_oneFrameRender)
		{
			_oneFrameRender->setTextureCoordScale(textureCoordScale);
		}
		for (auto rend : _animationRender)
		{
			rend->setTextureCoordScale(textureCoordScale);
		}
	}

	void mPostRender::setMinMaxData(float maxValue, float minValue)
	{
		this->makeCurrent();
		if (_oneFrameRender)
		{
			_oneFrameRender->setMinMaxData(maxValue, minValue);
		}
	}

	void mPostRender::setRangeType(int index)
	{
		this->makeCurrent();
		_rendStatus->_rangeType = index;
		if (_oneFrameRender)
		{
			_oneFrameRender->updateAllModelOperate(UpdateMinMax);
		}
	}

	void mPostRender::setDispersIsEquivariance(bool isEquivariance)
	{
		_rendStatus->_isEquivariance = isEquivariance;
	}

	void mPostRender::setPointSize(int size)
	{
		_rendStatus->_pointSize = size;
		_pointStateSet->getUniform("PointSize")->SetData(_rendStatus->_pointSize);
	}

	void mPostRender::setEdgeLineWidth(float size)
	{
		_rendStatus->_lineWidth = size;
		_edgelineStateSet->getUniform("lineWidth")->SetData(_rendStatus->_lineWidth);
	}

	void mPostRender::setFaceLineColor(QVector4D color)
	{
		_rendStatus->_faceLineColor = color;
		_facelineStateSet->getUniform("showColor")->SetData(_rendStatus->_faceLineColor);
	}

	void mPostRender::deleteCuttingPlane(int num)
	{
		this->makeCurrent();
		bool hasDeleteCuttingPlane = false;
		if (_oneFrameRender)
		{
			hasDeleteCuttingPlane = _oneFrameRender->deleteCuttingPlane(num);
		}
		if (_oneFrameAnimationRender)
		{
			hasDeleteCuttingPlane = _oneFrameAnimationRender->deleteCuttingPlane(num);
		}
		for (auto rend : _animationRender)
		{
			hasDeleteCuttingPlane = rend->deleteCuttingPlane(num);
		}
		if (!hasDeleteCuttingPlane)
		{
			return;
		}
		_rendStatus->_cuttingPlanes.removeAt(num);
		this->updateCuttingPlaneUniform();
	}

	void mPostRender::reverseCuttingPlaneNormal(int num)
	{
		this->makeCurrent();
		bool hasReverseCuttingPlane = false;
		if (_oneFrameRender)
		{
			hasReverseCuttingPlane = _oneFrameRender->reverseCuttingPlaneNormal(num);
		}
		if (_oneFrameAnimationRender)
		{
			hasReverseCuttingPlane = _oneFrameAnimationRender->reverseCuttingPlaneNormal(num);
		}
		for (auto rend : _animationRender)
		{
			hasReverseCuttingPlane = rend->reverseCuttingPlaneNormal(num);
		}
		if (!hasReverseCuttingPlane)
		{
			return;
		}
		_rendStatus->_cuttingPlanes[num] = -_rendStatus->_cuttingPlanes[num];
		updateCuttingPlaneUniform();
	}

	void mPostRender::setOnlyShowCuttingPlane(bool isOnlyShowCuttingPlane)
	{
		_rendStatus->_isOnlyShowCuttingPlane = isOnlyShowCuttingPlane;
		this->makeCurrent();

		if (_oneFrameRender)
		{
			_oneFrameRender->setOnlyShowCuttingPlane(isOnlyShowCuttingPlane);
		}
		if (_oneFrameAnimationRender)
		{
			_oneFrameAnimationRender->setOnlyShowCuttingPlane(isOnlyShowCuttingPlane);
		}
		for (auto rend : _animationRender)
		{
			rend->setOnlyShowCuttingPlane(isOnlyShowCuttingPlane);
		}
	}

	void mPostRender::setIsShowCuttingPlane(int num, bool isShow)
	{
		this->makeCurrent();
		if (_oneFrameRender)
		{
			_oneFrameRender->setIsShowCuttingPlane(num, isShow);
		}
		if (_oneFrameAnimationRender)
		{
			_oneFrameAnimationRender->setIsShowCuttingPlane(num, isShow);
		}
		for (auto rend : _animationRender)
		{
			rend->setIsShowCuttingPlane(num, isShow);
		}
	}

	void mPostRender::createCuttingPlane(int num, QVector3D normal, QVector3D vertex, bool hasVector)
	{
		this->makeCurrent();
		bool hasCreateCuttingPlane = false;
		if (_oneFrameRender)
		{
			hasCreateCuttingPlane = _oneFrameRender->createCuttingPlane(_cuttingPlaneStateSet, _transparentPlaneStateSet, num, normal, vertex, hasVector);
		}
		if (_oneFrameAnimationRender)
		{
			hasCreateCuttingPlane = _oneFrameAnimationRender->createCuttingPlane(_cuttingPlaneStateSet, _transparentPlaneStateSet, num, normal, vertex, hasVector);
		}
		for (auto rend : _animationRender)
		{
			hasCreateCuttingPlane = rend->createCuttingPlane(_cuttingPlaneStateSet, _transparentPlaneStateSet, num, normal, vertex, hasVector);
		}
		if (!hasCreateCuttingPlane)
		{
			return;
		}
		QVector4D plane = QVector4D(normal, -QVector3D::dotProduct(normal, vertex));
		if (num < _rendStatus->_cuttingPlanes.size())
		{
			_rendStatus->_cuttingPlanes.replace(num, plane);
		}
		else if (num == _rendStatus->_cuttingPlanes.size())
		{
			_rendStatus->_cuttingPlanes.append(plane);
		}
		this->updateCuttingPlaneUniform();
	}

	void mPostRender::setPlaneData(int num, QVector3D normal, QVector3D centervertex, float maxR)
	{
		this->makeCurrent();
		if (_oneFrameRender)
		{
			_oneFrameRender->setPlaneData(num, normal, centervertex, maxR);
		}
		if (_oneFrameAnimationRender)
		{
			_oneFrameAnimationRender->setPlaneData(num, normal, centervertex, maxR);
		}
		for (auto rend : _animationRender)
		{
			rend->setPlaneData(num, normal, centervertex, maxR);
		}
	}

	void mPostRender::setIsShowPlane(bool isShow)
	{
		this->makeCurrent();
		if (_oneFrameRender)
		{
			_oneFrameRender->setIsShowPlane(isShow);
		}
		if (_oneFrameAnimationRender)
		{
			_oneFrameAnimationRender->setIsShowPlane(isShow);
		}
		for (auto rend : _animationRender)
		{
			rend->setIsShowPlane(isShow);
		}
	}

	void mPostRender::setRendAnimationFrame(mPostAnimationRendData *allFrameRendData)
	{
		this->makeCurrent();
		if (!_dataPost || !allFrameRendData)
		{
			return;
		}
		deleteAnimation();

	}

	void mPostRender::createLinearAnimation(PostMode postMode)
	{
		this->makeCurrent();
		if (!_dataPost || nullptr == _oneFrameRender)
		{
			return;
		}
		deleteAnimation();
		mPostOneFrameRendData* postOneFrameRendData = _oneFrameRender->getOneFrameRendData();
		int id = postOneFrameRendData->getRendID();
		QVector3D deformationScale = postOneFrameRendData->getDeformationScale();
		mOneFrameData1 *oneFrameData = _dataPost->getOneFrameData(id);

		mPostOneFrameRendData *newFrameRendData = new mPostOneFrameRendData(*postOneFrameRendData);
		_oneFrameAnimationRender = make_shared<mPostOneFrameRender>(_rendStatus, oneFrameData, newFrameRendData);
		_oneFrameAnimationRender->setFaceStateSet(_faceStateSet);
		_oneFrameAnimationRender->setFaceTransparentNoDeformationStateSet(_faceTransparentNodeformationStateSet);
		_oneFrameAnimationRender->setFaceTransparentStateSet(_faceTransparentStateSet);
		_oneFrameAnimationRender->setEdgeLineStateSet(_edgelineStateSet);
		_oneFrameAnimationRender->setFaceLineStateSet(_facelineStateSet);
		_oneFrameAnimationRender->setLineStateSet(_lineStateSet);
		_oneFrameAnimationRender->setPointStateSet(_pointStateSet);
		_oneFrameAnimationRender->setTextureCoordScale(0.0);
		_oneFrameAnimationRender->setDeformationScale(QVector3D(0,0,0));
		_oneFrameAnimationRender->updateAllModelOperate(ImportOperate);
		_oneFrameAnimationRender->bufferThisFrame(_app->GLContext());
		_rendStatus->_aniCurrentFrame = 1;
		_rendStatus->_postMode = postMode;

		/*
		int ids = 8;
		QVector<QFuture<void>> futures;
		for (int i = 0; i < ids; i++)
		{
			mPostOneFrameRendData *newFrameRendData = new mPostOneFrameRendData(*postOneFrameRendData);
			std::shared_ptr<mPostOneFrameRender> oneFrameRender = make_shared<mPostOneFrameRender>(_rendStatus, oneFrameData, newFrameRendData);
			float scale = postMode == OneFrameLinearAnimation ? i / float(ids - 1) : sin(2 * M_PI * i / float(ids - 1));

			oneFrameRender->setFaceStateSet(_faceStateSet);
			oneFrameRender->setFaceTransparentNoDeformationStateSet(_faceTransparentNodeformationStateSet);
			oneFrameRender->setFaceTransparentStateSet(_faceTransparentStateSet);
			oneFrameRender->setEdgeLineStateSet(_edgelineStateSet);
			oneFrameRender->setFaceLineStateSet(_facelineStateSet);
			oneFrameRender->setLineStateSet(_lineStateSet);
			oneFrameRender->setPointStateSet(_pointStateSet);
			oneFrameRender->setTextureCoordScale(scale);
			oneFrameRender->setDeformationScale(deformationScale*scale);
			_animationRender.insert(i + 1, oneFrameRender);
			futures.append(QtConcurrent::run([oneFrameRender]
			{
				oneFrameRender->updateAllModelOperate(ImportOperate);
			}));

		}
		while (!futures.empty())
		{
			futures.back().waitForFinished();
			//futures.back().result()->bufferThisFrame(_app->GLContext());
			futures.takeLast();
		}
		for (auto rend : _animationRender)
		{
			//QtConcurrent::run(rend.get(), &mPostOneFrameRender::bufferThisFrame,_app->GLContext());
			rend->bufferThisFrame(_app->GLContext());
		}
		//for (auto rend : _animationRender)
		//{
		//	rend->getOneFrameRendData()->deleteValueAndDisplacementData();
		//}
		*/

	}

	void mPostRender::deleteAnimation()
	{
		this->makeCurrent();
		setTimerOn(false);
		if (_oneFrameAnimationRender)
		{
			_oneFrameAnimationRender->deleteThieFrame();
			_oneFrameAnimationRender.reset();
		}
		for (auto render : _animationRender)
		{
			render->deleteThieFrame();
			render.reset();
		}
		_animationRender.clear();
	}

	void mPostRender::setTimerOn(bool ison)
	{
		mxr::time->start();
		if (ison)
		{
			int interval = 1000 / 30;
			_aniTimer->start(interval);
			_rendStatus->_aniIsStart = true;
		}
		else
		{
			_rendStatus->_aniIsStart = false;
			_aniTimer->stop();
		}
	}
	
	void mPostRender::slot_aniTimer()
	{
		//qDebug() << __LINE__ << mxr::time->elapsed();
		//mxr::time->start();
		int interval = 1000 / _rendStatus->_aniFrameRate;
		_aniTimer->setInterval(interval);

		//判断是否到达最后一帧
		if (_rendStatus->_aniCurrentFrame >= _rendStatus->_aniEndFrame)
		{
			//判断是否循环播放
			if (_rendStatus->_aniLoopPlay)
			{
				//把当前帧设置为起始帧
				_rendStatus->_aniCurrentFrame = _rendStatus->_aniStartFrame;
			}
			else
			{
				//停止动画
				_rendStatus->_aniIsStart = false;
				_aniTimer->stop();
			}
		}
		else
		{
			//根据帧间隔运行
			_rendStatus->_aniCurrentFrame += _rendStatus->_aniFrameInterval;
		}
		
		if (_rendStatus->_postMode == OneFrameLinearAnimation || _rendStatus->_postMode == OneFrameSinAnimation)
		{
			//切换帧
			int nf = _rendStatus->_aniLinearCount;
			float scale = _rendStatus->_postMode == OneFrameLinearAnimation ? _rendStatus->_aniCurrentFrame / float(nf - 1) : sin(2 * M_PI * _rendStatus->_aniCurrentFrame / float(nf - 1));
			_oneFrameAnimationRender->setDeformationScale(scale * _rendStatus->_deformFactor);
			_oneFrameAnimationRender->setTextureCoordScale(scale);
		}
		else
		{
			//this->setAnimationFrameID(_frameGUIData->_aniCurrentFrame);
		}
		emit update();
	}

	mPostRender::~mPostRender()
	{
		this->makeCurrent();
		_oneFrameRender.reset();
		_oneFrameAnimationRender.reset();
		//_animationRender.reset();
		
	}

	void mPostRender::updateCuttingPlaneUniform()
	{
		int cuttingPlaneSize = _rendStatus->_cuttingPlanes.size();
		for (int i = 0; i < 7; i++)
		{
			string str = QString("planes[%1]").arg(i).toStdString();
			if (i < cuttingPlaneSize)
			{
				_cuttingPlaneStateSet->getUniform(str)->SetData(_rendStatus->_cuttingPlanes[i]);
				_cuttingPlaneStateSet->getUniform(str)->SetEnable(true);
				_faceStateSet->getUniform(str)->SetData(_rendStatus->_cuttingPlanes[i]);
				_faceStateSet->getUniform(str)->SetEnable(true);
				_faceTransparentStateSet->getUniform(str)->SetData(_rendStatus->_cuttingPlanes[i]);
				_faceTransparentStateSet->getUniform(str)->SetEnable(true);
				_faceTransparentNodeformationStateSet->getUniform(str)->SetData(_rendStatus->_cuttingPlanes[i]);
				_faceTransparentNodeformationStateSet->getUniform(str)->SetEnable(true);
				_edgelineStateSet->getUniform(str)->SetData(_rendStatus->_cuttingPlanes[i]);
				_edgelineStateSet->getUniform(str)->SetEnable(true);
				_facelineStateSet->getUniform(str)->SetData(_rendStatus->_cuttingPlanes[i]);
				_facelineStateSet->getUniform(str)->SetEnable(true);
				_lineStateSet->getUniform(str)->SetData(_rendStatus->_cuttingPlanes[i]);
				_lineStateSet->getUniform(str)->SetEnable(true);
				_pointStateSet->getUniform(str)->SetData(_rendStatus->_cuttingPlanes[i]);
				_pointStateSet->getUniform(str)->SetEnable(true);
				_cuttingPlaneStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 1);
				_faceStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 1);
				_faceTransparentStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 1);
				_faceTransparentNodeformationStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 1);
				_edgelineStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 1);
				_facelineStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 1);
				_lineStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 1);
				_pointStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 1);
			}
			else
			{
				_cuttingPlaneStateSet->getUniform(str)->SetEnable(false);
				_cuttingPlaneStateSet->getUniform(str)->SetData(QVector4D());
				_faceStateSet->getUniform(str)->SetEnable(false);
				_faceStateSet->getUniform(str)->SetData(QVector4D());
				_faceTransparentStateSet->getUniform(str)->SetEnable(false);
				_faceTransparentStateSet->getUniform(str)->SetData(QVector4D());
				_faceTransparentNodeformationStateSet->getUniform(str)->SetEnable(false);
				_faceTransparentNodeformationStateSet->getUniform(str)->SetData(QVector4D());
				_edgelineStateSet->getUniform(str)->SetEnable(false);
				_edgelineStateSet->getUniform(str)->SetData(QVector4D());
				_facelineStateSet->getUniform(str)->SetEnable(false);
				_facelineStateSet->getUniform(str)->SetData(QVector4D());
				_lineStateSet->getUniform(str)->SetEnable(false);
				_lineStateSet->getUniform(str)->SetData(QVector4D());
				_pointStateSet->getUniform(str)->SetEnable(false);
				_pointStateSet->getUniform(str)->SetData(QVector4D());
				_cuttingPlaneStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 0);
				_faceStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 0);
				_faceTransparentStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 0);
				_faceTransparentNodeformationStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 0);
				_edgelineStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 0);
				_facelineStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 0);
				_lineStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 0);
				_pointStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 0);
			}
		}
	}

	void mPostRender::initialPickThreads()
	{
		if (!_dataPost)
		{
			return;
		}
		_pickData = new mPostMeshPickData;

		//初始化部件拾取多线程
		set<QString> partNames = _dataPost->getAllPostPartNames();
		_thread = new mPostMeshPickThread(_pickData);
		_thread->setPickFilter(_baseRend->getPickFilter());
		for (QString partName : partNames)
		{
			_thread->appendPartSpaceTree(partName, _oneFrameRender->getModelRender()->getPartSpaceTree(partName));
		}	
	}

	void mPostRender::updateUniform(shared_ptr<mModelView> modelView, shared_ptr<mCommonView> commonView)
	{
		if (_faceStateSet)
		{
	/*		_faceStateSet->getUniform("light.position")->SetData(light.lightPosition);
			_faceStateSet->getUniform("light.ambient")->SetData(light.ambient);
			_faceStateSet->getUniform("light.diffuse")->SetData(light.diffuse);
			_faceStateSet->getUniform("light.specular")->SetData(light.specular);
			_faceStateSet->getUniform("light.shiness")->SetData(light.shiness);
			_faceStateSet->getUniform("viewPos")->SetData(viewpos);
			_faceStateSet->getUniform("lightIsOn")->SetData(lightIsOn);*/

			_faceStateSet->getUniform("projection")->SetData(modelView->_projection);
			_faceStateSet->getUniform("view")->SetData(modelView->_view);
			_faceStateSet->getUniform("model")->SetData(modelView->_model);
			_faceTransparentStateSet->getUniform("projection")->SetData(modelView->_projection);
			_faceTransparentStateSet->getUniform("view")->SetData(modelView->_view);
			_faceTransparentStateSet->getUniform("model")->SetData(modelView->_model);
			_faceTransparentNodeformationStateSet->getUniform("projection")->SetData(modelView->_projection);
			_faceTransparentNodeformationStateSet->getUniform("view")->SetData(modelView->_view);
			_faceTransparentNodeformationStateSet->getUniform("model")->SetData(modelView->_model);
			_edgelineStateSet->getUniform("projection")->SetData(modelView->_projection);
			_edgelineStateSet->getUniform("view")->SetData(modelView->_view);
			_edgelineStateSet->getUniform("model")->SetData(modelView->_model);
			_facelineStateSet->getUniform("projection")->SetData(modelView->_projection);
			_facelineStateSet->getUniform("view")->SetData(modelView->_view);
			_facelineStateSet->getUniform("model")->SetData(modelView->_model);
			_lineStateSet->getUniform("projection")->SetData(modelView->_projection);
			_lineStateSet->getUniform("view")->SetData(modelView->_view);
			_lineStateSet->getUniform("model")->SetData(modelView->_model);
			_pointStateSet->getUniform("projection")->SetData(modelView->_projection);
			_pointStateSet->getUniform("view")->SetData(modelView->_view);
			_pointStateSet->getUniform("model")->SetData(modelView->_model);
			_cuttingPlaneStateSet->getUniform("projection")->SetData(modelView->_projection);
			_cuttingPlaneStateSet->getUniform("view")->SetData(modelView->_view);
			_cuttingPlaneStateSet->getUniform("model")->SetData(modelView->_model);
			_transparentPlaneStateSet->getUniform("projection")->SetData(modelView->_projection);
			_transparentPlaneStateSet->getUniform("view")->SetData(modelView->_view);
			_transparentPlaneStateSet->getUniform("model")->SetData(modelView->_model);

			_faceStateSet->getUniform("viewPos")->SetData(modelView->_Eye);
			_faceTransparentStateSet->getUniform("viewPos")->SetData(modelView->_Eye);
			_pointStateSet->getUniform("viewPos")->SetData(modelView->_Eye);
			_cuttingPlaneStateSet->getUniform("viewPos")->SetData(modelView->_Eye);
			//_faceStateSet->getUniform("light.ambient")->SetData(_rendStatus->_postLight.ambient);
			//_faceStateSet->getUniform("light.diffuse")->SetData(_rendStatus->_postLight.diffuse);
			//_faceStateSet->getUniform("light.specular")->SetData(_rendStatus->_postLight.specular);
			//_faceStateSet->getUniform("light.shiness")->SetData(_rendStatus->_postLight.shiness);
			if (_rendStatus->_lightIsDependOnCamera)
			{
				_faceStateSet->getUniform("light.position")->SetData(2 * modelView->_Eye - modelView->_Center);
				_faceTransparentStateSet->getUniform("light.position")->SetData(2 * modelView->_Eye - modelView->_Center);
				_faceTransparentNodeformationStateSet->getUniform("light.position")->SetData(2 * modelView->_Eye - modelView->_Center);
				_pointStateSet->getUniform("light.position")->SetData(2 * modelView->_Eye - modelView->_Center);
				_cuttingPlaneStateSet->getUniform("light.position")->SetData(2 * modelView->_Eye - modelView->_Center);
			}
			else
			{
				_faceStateSet->getUniform("light.position")->SetData(_rendStatus->_postLight.lightPosition);
				_faceTransparentStateSet->getUniform("light.position")->SetData(_rendStatus->_postLight.lightPosition);
				_faceTransparentNodeformationStateSet->getUniform("light.position")->SetData(_rendStatus->_postLight.lightPosition);
				_pointStateSet->getUniform("light.position")->SetData(_rendStatus->_postLight.lightPosition);
				_cuttingPlaneStateSet->getUniform("light.position")->SetData(_rendStatus->_postLight.lightPosition);
			}

			_facelineStateSet->getUniform("rightToLeft")->SetData(float(modelView->_Right - modelView->_Left));
			_edgelineStateSet->getUniform("rightToLeft")->SetData(float(modelView->_Right - modelView->_Left));
		}
		if (_rendStatus->_postMode == OneFrame)
		{
			if (_oneFrameRender)
			{
				_oneFrameRender->updateUniform(modelView, commonView);
			}
		}
		else if (_rendStatus->_postMode == OneFrameLinearAnimation || _rendStatus->_postMode == OneFrameSinAnimation)
		{
			if (_oneFrameAnimationRender)
			{
				_oneFrameAnimationRender->updateUniform(modelView, commonView);
			}
		}
		else if (!_animationRender.empty())
		{
			_animationRender.value(_rendStatus->_aniCurrentFrame)->updateUniform(modelView, commonView);
		}	
	}
}