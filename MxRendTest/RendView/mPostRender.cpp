#include "mPostRender.h"
#include "mPostRendStatus.h"
#include "mPostOneFrameRender.h"
#include "mPostModelRender.h"
#include "mPostAnimationRender.h"

#include <renderpch.h>
#include "texture.h"
#include "Space.h"

#include "mShaderManage.h"
#include "mTextureManage.h"

//��ͼ��
#include "mModelView.h"
#include "mCommonView.h"

#include <QMatrix3x3>
#include <QFileDialog>
#include <QFileInfo>
#include <QApplication>

//MDataPost
#include "mDataPost1.h"
#include "mOneFrameData1.h"
#include "mPostOneFrameRendData.h"
#include "mPostAnimationRendData.h"
#include "mPostColorTableData.h"

using namespace mxr;
using namespace std;
namespace MPostRend
{
	mPostRender::mPostRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent):mBaseRender(app, parent)
	{
		_dataPost = nullptr;
		_texture = nullptr;
		_oneFrameRender = nullptr;
		_animationRender = nullptr;

		_rendStatus = make_shared<mPostRendStatus>();

		/**********************************************************ģ��**********************************************************/
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

		//face͸��
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

		//face͸���ޱ���
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

		/**********************************************************����**********************************************************/
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
		_transparentPlaneStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(mxr::PolygonMode::FRONT_AND_BACK, mxr::PolygonMode::FILL), 1);
		_transparentPlaneStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), 1);
		_transparentPlaneStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_transparentPlaneStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_transparentPlaneStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
	}
	void mPostRender::clearRender()
	{
		if (_oneFrameRender)
		{
			_oneFrameRender.reset();
		}
		if (_animationRender)
		{
			_animationRender.reset();
		}

	}
	void mPostRender::setRendCurrentFrameData(mPostOneFrameRendData* postOneFrameRendData)
	{
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
		_oneFrameRender = make_shared<mPostOneFrameRender>(_parent, _rendStatus, oneFrameData, postOneFrameRendData);
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
	}
	void mPostRender::setRendAnimationFrame(mPostAnimationRendData *allFrameRendData)
	{
		if (!_dataPost || !allFrameRendData)
		{
			return;
		}

	}

	void mPostRender::setShowFuntion(ShowFuntion showFuntion)
	{
		_rendStatus->_showFunction = showFuntion;
		if (_oneFrameRender)
		{
			_oneFrameRender->getModelRender()->setShowFuntion(showFuntion);
		}
	}

	void mPostRender::setDispersed(bool isdispersed)
	{
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
		if (_oneFrameRender)
		{
			_oneFrameRender->setDeformationScale(deformationScale);
		}
	}

	void mPostRender::setIsShowInitialShape(bool isShowInitialShape)
	{
		_rendStatus->_isShowInitialShape = isShowInitialShape;
		if (_oneFrameRender)
		{
			_oneFrameRender->getModelRender()->setIsShowInitialShape(isShowInitialShape);
		}
	}

	void mPostRender::setTextureCoordScale(float textureCoordScale)
	{
		if (_oneFrameRender)
		{
			_oneFrameRender->setTextureCoordScale(textureCoordScale);
		}
	}

	void mPostRender::setMinMaxData(float maxValue, float minValue)
	{
		if (_oneFrameRender)
		{
			_oneFrameRender->setMinMaxData(maxValue, minValue);
		}
	}

	void mPostRender::setRangeType(int index)
	{
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
		_facelineStateSet->getUniform("PointSize")->SetData(_rendStatus->_pointSize);
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
		if (_oneFrameRender)
		{
			_oneFrameRender->deleteCuttingPlane(num);
		}
	}

	void mPostRender::reverseCuttingPlaneNormal(int num)
	{
		if (_oneFrameRender)
		{
			_oneFrameRender->reverseCuttingPlaneNormal(num);
		}
	}

	void mPostRender::setOnlyShowCuttingPlane(bool isOnlyShowCuttingPlane)
	{
		if (_oneFrameRender)
		{
			_oneFrameRender->setOnlyShowCuttingPlane(isOnlyShowCuttingPlane);
		}
	}

	void mPostRender::setIsShowCuttingPlane(int num, bool isShow)
	{
		if (_oneFrameRender)
		{
			_oneFrameRender->setIsShowCuttingPlane(num, isShow);
		}
	}

	void mPostRender::createCuttingPlane(int num, QVector3D normal, QVector3D vertex, bool hasVector)
	{
		bool hasCreateCuttingPlane = false;
		if (_oneFrameRender)
		{
			hasCreateCuttingPlane = _oneFrameRender->createCuttingPlane(_cuttingPlaneStateSet, _transparentPlaneStateSet, num, normal, vertex, hasVector);
		}
	}

	void mPostRender::setPlaneData(int num, QVector3D normal, QVector3D centervertex, float maxR)
	{
		if (_oneFrameRender)
		{
			_oneFrameRender->setPlaneData(num, normal, centervertex, maxR);
		}
	}

	void mPostRender::setIsShowPlane(bool isShow)
	{
		if (_oneFrameRender)
		{
			_oneFrameRender->setIsShowPlane(isShow);
		}
	}

	mPostRender::~mPostRender()
	{
		_oneFrameRender.reset();
		_animationRender.reset();
		
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


		if (_oneFrameRender)
		{
			_oneFrameRender->updateUniform(modelView, commonView);
		}
	}
}