#include "mPostOneFrameRender.h"
#include "mPostModelRender.h"
#include "mPostCuttingPlaneRender.h"

#include "mPostRendStatus.h"
#include <renderpch.h>
#include "Space.h"
#include "texture.h"

#include "mShaderManage.h"
#include "mTextureManage.h"

//视图类
#include "mModelView.h"
#include "mCommonView.h"

#include <QMatrix3x3>
#include <QFileDialog>
#include <QFileInfo>
#include <QApplication>

#include "mPostOneFrameRendData.h"

using namespace mxr;
using namespace std;
namespace MPostRend
{
	mPostOneFrameRender::mPostOneFrameRender(std::shared_ptr<mxr::Group> parent, shared_ptr<mPostRendStatus> rendStatus, mOneFrameData1 *oneFrameData, mPostOneFrameRendData *oneFrameRendData) :
		_parent(parent), _rendStatus(rendStatus), _oneFrameData(oneFrameData), _oneFrameRendData(oneFrameRendData)
	{
		_cuttingPlaneStateSet = nullptr;

		_geode = MakeAsset<Geode>();
		_parent->addChild(_geode);

		_modelRender = make_shared<mPostModelRender>(_geode, _rendStatus, oneFrameData, oneFrameRendData);
	}
	mPostOneFrameRender::~mPostOneFrameRender()
	{
		_parent->removeChild(_geode);
		_modelRender.reset();
	}

	void mPostOneFrameRender::updateUniform(shared_ptr<mModelView> modelView, shared_ptr<mCommonView> commonView)
	{
		if (_modelRender)
		{
			//std::vector<QVector4D> cutplanes;
			//for (int i = 0; i < _cuttingPlaneRenders.size(); i++)
			//{
			//	cutplanes.push_back(_cuttingPlaneRenders[i]->getCuttingPlane());
			//}

			//模型
			_modelRender->setDeformationScale(_oneFrameRendData->getDeformationScale());
			_modelRender->setTextureArgument(_rendStatus->_isEquivariance, _oneFrameRendData->getCurrentMinData(), _oneFrameRendData->getCurrentMaxData(), _oneFrameRendData->getTextureCoordScale());
			if (_cuttingPlaneStateSet)
			{
				_cuttingPlaneStateSet->getUniform("isEquivariance")->SetData(_rendStatus->_isEquivariance);
				_cuttingPlaneStateSet->getUniform("minValue")->SetData(_oneFrameRendData->getCurrentMinData());
				_cuttingPlaneStateSet->getUniform("maxValue")->SetData(_oneFrameRendData->getCurrentMaxData());
				_cuttingPlaneStateSet->getUniform("textureCoordRatio")->SetData(_oneFrameRendData->getTextureCoordScale());
			}
			//_modelRender->setTexture(_texture);
			//_modelRender->setDistancePlane(cutplanes);
		}
	}

	void mPostOneFrameRender::updateOneModelOperate(QPair<MBasicFunction::PostModelOperateEnum, std::set<QString>> postModelOperates)
	{
		if (_modelRender)
		{
			_modelRender->updateOneModelOperate(postModelOperates);
		}
	}

	void mPostOneFrameRender::updateAllModelOperate(MBasicFunction::PostModelOperateEnum postModelOperate)
	{
		if (_modelRender)
		{
			_modelRender->updateAllModelOperate(postModelOperate);
		}
	}

	void mPostOneFrameRender::setFaceStateSet(std::shared_ptr<mxr::StateSet> faceStateSet)
	{
		_faceStateSet = faceStateSet;
		_modelRender->setFaceStateSet(_faceStateSet);
	}

	void mPostOneFrameRender::setFaceTransparentStateSet(std::shared_ptr<mxr::StateSet> faceTransparentStateSet)
	{
		_faceTransparentStateSet = faceTransparentStateSet;
		_modelRender->setFaceTransparentStateSet(_faceTransparentStateSet);
	}

	void mPostOneFrameRender::setFaceTransparentNoDeformationStateSet(std::shared_ptr<mxr::StateSet> faceTransparentNodeformationStateSet)
	{
		_faceTransparentNodeformationStateSet = faceTransparentNodeformationStateSet;
		_modelRender->setFaceTransparentNoDeformationStateSet(_faceTransparentNodeformationStateSet);
	}

	void mPostOneFrameRender::setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> edgelineStateSet)
	{
		_edgelineStateSet = edgelineStateSet;
		_modelRender->setEdgeLineStateSet(_edgelineStateSet);
	}

	void mPostOneFrameRender::setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet)
	{
		_facelineStateSet = facelineStateSet;
		_modelRender->setFaceLineStateSet(_facelineStateSet);
	}

	void mPostOneFrameRender::setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet)
	{
		_lineStateSet = lineStateSet;
		_modelRender->setLineStateSet(_lineStateSet);
	}

	void mPostOneFrameRender::setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet)
	{
		_pointStateSet = pointStateSet;
		_modelRender->setPointStateSet(_pointStateSet);
	}

	void mPostOneFrameRender::setTexture(std::shared_ptr<mxr::Texture> texture)
	{
		_texture = texture;
	}

	void mPostOneFrameRender::setDeformationScale(QVector3D deformationScale)
	{
		if (_oneFrameRendData)
		{
			_oneFrameRendData->setDeformationScale(deformationScale);
		}
		if (_modelRender)
		{
			_modelRender->updateAllModelOperate(UpdateNormal);
		}
	}

	void mPostOneFrameRender::setTextureCoordScale(float textureCoordScale)
	{
		if (_oneFrameRendData)
		{
			_oneFrameRendData->setTextureCoordScale(textureCoordScale);
		}
	}

	void mPostOneFrameRender::setMinMaxData(float maxValue, float minValue)
	{
		if (_oneFrameRendData)
		{
			_oneFrameRendData->setMinMaxData(maxValue, minValue);
			this->updateAllModelOperate(UpdateMinMax);

		}
	}

	void mPostOneFrameRender::deleteCuttingPlane(int num)
	{
		if (num >= _cuttingPlaneRenders.size())
		{
			return;
		}
		for (int i = num; i < _cuttingPlaneRenders.size(); i++)
		{
			_cuttingPlaneRenders[i]->updateCuttingPlaneIndex(i - 1);
		}
		_cuttingPlaneRenders[num].reset();
		_cuttingPlaneRenders[num] = nullptr;
		_cuttingPlaneRenders.removeAt(num);
		_rendStatus->_cuttingPlanes.removeAt(num);
		this->updateCuttingPlaneUniform();
	}

	void mPostOneFrameRender::reverseCuttingPlaneNormal(int num)
	{
		if (num >= _cuttingPlaneRenders.size())
		{
			return;
		}
		_cuttingPlaneRenders[num]->reverseCuttingPlaneNormal();
	}

	void mPostOneFrameRender::setOnlyShowCuttingPlane(bool isOnlyShowCuttingPlane)
	{
		
	}


	void mPostOneFrameRender::setIsShowCuttingPlane(int num, bool isShow)
	{
		if (num >= _cuttingPlaneRenders.size())
		{
			return;
		}
		_cuttingPlaneRenders[num]->setIsShowCuttingPlane(isShow);
	}

	bool mPostOneFrameRender::createCuttingPlane(std::shared_ptr<mxr::StateSet> cuttingPlaneStateSet, std::shared_ptr<mxr::StateSet> transparentPlaneStateSet, int num, QVector3D normal, QVector3D vertex, bool hasVector)
	{
		_cuttingPlaneStateSet = cuttingPlaneStateSet;
		_transparentPlaneStateSet = transparentPlaneStateSet;
		if (num > 7 || num > _cuttingPlaneRenders.size())
		{
			return false;
		}
		std::shared_ptr<mPostCuttingPlaneRender> planerender = make_shared<mPostCuttingPlaneRender>(_parent, cuttingPlaneStateSet, transparentPlaneStateSet);
		planerender->createCuttingPlane(_oneFrameData, _oneFrameRendData, num, normal, vertex, hasVector);
		if (num < _cuttingPlaneRenders.size())
		{
			_cuttingPlaneRenders[num].reset();
			_cuttingPlaneRenders.replace(num, planerender);
		}
		else if (num == _cuttingPlaneRenders.size())
		{
			_cuttingPlaneRenders.append(planerender);
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

		return true;
	}

	void mPostOneFrameRender::setPlaneData(int num, QVector3D normal, QVector3D centervertex, float maxR)
	{
		if (num > 7 || num > _cuttingPlaneRenders.size())
		{
			return;
		}
		if (num < _cuttingPlaneRenders.size())
		{
			auto postCuttingPlane = _cuttingPlaneRenders[num];
			postCuttingPlane->setPlaneData(normal, centervertex, maxR);
		}
	}

	void mPostOneFrameRender::setIsShowPlane(bool isShow)
	{
		//_isShowPlane = isShow;
	}
	void mPostOneFrameRender::updateCuttingPlaneUniform()
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
}