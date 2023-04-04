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
#include "mOneFrameData1.h"

using namespace mxr;
using namespace std;
namespace MPostRend
{
	mPostOneFrameRender::mPostOneFrameRender(shared_ptr<mPostRendStatus> rendStatus, mOneFrameData1 *oneFrameData, mPostOneFrameRendData *oneFrameRendData) : _rendStatus(rendStatus), _oneFrameData(oneFrameData), _oneFrameRendData(oneFrameRendData)
	{
		_cuttingPlaneStateSet = nullptr;

		_geode = MakeAsset<Geode>();
		_viewer = nullptr;

		this->initial();

		_modelRender = make_shared<mPostModelRender>(_geode, _rendStatus, oneFrameData, oneFrameRendData);
	}
	mPostOneFrameRender::~mPostOneFrameRender()
	{
		_modelRender.reset();
		delete _oneFrameRendData;
	}

	void mPostOneFrameRender::bufferThisFrame(QOpenGLContext *context)
	{
		context->makeCurrent(context->surface());
		_viewer->compile();
	}

	void mPostOneFrameRender::deleteThieFrame()
	{
		if (_viewer)
		{
			_viewer->deleteAllData();
		}
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
		_viewer->noClearRun();
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

	bool mPostOneFrameRender::deleteCuttingPlane(int num)
	{
		if (num >= _cuttingPlaneRenders.size())
		{
			return false;
		}
		for (int i = num; i < _cuttingPlaneRenders.size(); i++)
		{
			_cuttingPlaneRenders[i]->updateCuttingPlaneIndex(i - 1);
		}
		_cuttingPlaneRenders[num].reset();
		_cuttingPlaneRenders[num] = nullptr;
		_cuttingPlaneRenders.removeAt(num);
		return true;
	}

	bool mPostOneFrameRender::reverseCuttingPlaneNormal(int num)
	{
		if (num >= _cuttingPlaneRenders.size())
		{
			return false;
		}
		_cuttingPlaneRenders[num]->reverseCuttingPlaneNormal();
		return true;
	}

	void mPostOneFrameRender::setOnlyShowCuttingPlane(bool isOnlyShowCuttingPlane)
	{
		if (isOnlyShowCuttingPlane)
		{
			this->updateAllModelOperate(HideAllPartOperate);
		}
		else
		{
			this->updateOneModelOperate({ShowOnePartOperate ,_oneFrameData->getAllPartNames() });
		}
		//_modelRender
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
		std::shared_ptr<mPostCuttingPlaneRender> planerender = make_shared<mPostCuttingPlaneRender>(_geode, cuttingPlaneStateSet, transparentPlaneStateSet);
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
		_rendStatus->_isShowTransparentPlane = isShow;
		for (auto cuttingPlane : _cuttingPlaneRenders)
		{
			cuttingPlane->setIsShowCuttingPlane(isShow);
		}
		//_isShowPlane = isShow;
	}
	void mPostOneFrameRender::initial()
	{
		if (!_viewer)
		{
			_viewer = MakeAsset<mxr::Viewer>();
			_viewer->setSceneData(_geode);
		}
	}

}