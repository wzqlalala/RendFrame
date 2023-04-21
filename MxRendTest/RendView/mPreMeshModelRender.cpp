#include "mPreGeoModelRender.h"

#include "mGeoModelData1.h"
#include "mGeoPartData1.h"
#include "mGeoSolidData1.h"
#include "mGeoFaceData1.h"
#include "mGeoLineData1.h"
#include "mGeoPointData1.h"

#include <renderpch.h>
#include "Space.h"

#include "mShaderManage.h"
#include "mTextureManage.h"

#include "mPreRendStatus.h"

// ”Õº¿‡
#include "mModelView.h"
#include "mViewBase.h"

#include <QMatrix3x3>
#include <QFileDialog>
#include <QFileInfo>
#include <QApplication>

using namespace mxr;
using namespace std;
using namespace MBasicFunction;
namespace MPreRend
{
	mPreGeoModelRender::mPreGeoModelRender(std::shared_ptr<mxr::Group> parent, shared_ptr<mPreRendStatus> rendStatus, mGeoModelData1 *geoModelData):
		_parent(parent), _rendStatus(rendStatus), _geoModelData(geoModelData)
	{
		_geode = MakeAsset<Geode>();
		_parent->addChild(_geode);	

		//updateAllModelOperate(ImportOperate);
	}

	void mPreGeoModelRender::setFaceStateSet(std::shared_ptr<mxr::StateSet> faceStateSet)
	{
		_faceStateSet = faceStateSet;
		//_facerend->setStateSet(_faceStateSet);
	}
	void mPreGeoModelRender::setFaceTransparentStateSet(std::shared_ptr<mxr::StateSet> faceTransparentStateSet)
	{
		_faceTransparentStateSet = faceTransparentStateSet;
	}
	void mPreGeoModelRender::setFaceTransparentNoDeformationStateSet(std::shared_ptr<mxr::StateSet> faceTransparentNoDeformationStateSet)
	{
		_faceTransparentNodeformationStateSet = faceTransparentNoDeformationStateSet;
	}
	void mPreGeoModelRender::setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> edgelineStateSet)
	{
		_edgelineStateSet = edgelineStateSet;
		//_meshlinerend->setStateSet(_edgelineStateSet);
	}
	void mPreGeoModelRender::setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet)
	{
		_facelineStateSet = facelineStateSet;
		//_facelinerend->setStateSet(_facelineStateSet);
	}
	void mPreGeoModelRender::setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet)
	{
		_lineStateSet = lineStateSet;
	}
	void mPreGeoModelRender::setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet)
	{
		_pointStateSet = pointStateSet;
	}
	mPreGeoModelRender::~mPreGeoModelRender()
	{
		_parent->removeChild(_geode);
		_partRenders.clear();
	}
	void mPreGeoModelRender::UpdateModelBuffer()
	{
		std::set<QString> hidePartNames;
		//auto iter = _oneFrameData->getMeshPartIterator();
		_partOrder = _geoModelData->getAllPartName();
		for (auto partName : _partOrder)
		{
			mGeoPartData1 *partData = _geoModelData->getGeoPartDataByPartName(partName);
			if (partData == nullptr)
			{
				continue;
			}
			if (!partData->getPartVisual())
			{
				hidePartNames.insert(partName);
			}
			asset_ref<mPreGeoPartRender> part = MakeAsset<mPreGeoPartRender>(_geode, partData);
			part->setFaceStateSet(_faceStateSet);
			part->setFaceTransparentNodeformationStateSet(_faceTransparentNodeformationStateSet);
			part->setEdgeLineStateSet(_edgelineStateSet);
			part->setFaceLineStateSet(_facelineStateSet);
			part->setLineStateSet(_lineStateSet);
			part->setPointStateSet(_pointStateSet);
			part->ImportPart();
			part->calculateSpaceTreeThread();

			_partRenders.insert(partName, part);
		}
		updateOneModelOperate(QPair<MBasicFunction::ModelOperateEnum, std::set<QString>>(HideOnePart, hidePartNames));
	}
	void mPreGeoModelRender::updateOneModelOperate(QPair<MBasicFunction::ModelOperateEnum, std::set<QString>> postModelOperates)
	{
		ModelOperateEnum postModelOperate = postModelOperates.first;
		set<QString> partNames = postModelOperates.second;
		if (postModelOperate == HideOnePart)
		{
			for (QString partName : partNames)
			{
				_geoModelData->setGeoPartVisual(partName, false);
				_partRenders[partName]->getGeode()->setNodeMask(1);
			}
		}
		else if (postModelOperate == ShowOnePart)
		{
			for (QString partName : partNames)
			{
				_geoModelData->setGeoPartVisual(partName, true);
				_partRenders[partName]->setShowFuntion(_rendStatus->_showFunction);
			}
		}
		else if (postModelOperate == ColorOnePart)
		{
			for (QString partName : partNames)
			{
				QVector3D color = _geoModelData->getGeoPartColor(partName);
				Array *array = _partRenders[partName]->getFaceDrawable()->getVertexAttribArray(1);
				array->updata(QVector<QVector3D>(array->size(), color).data());
				array = _partRenders[partName]->getLineDrawable()->getVertexAttribArray(1);
				array->updata(QVector<QVector3D>(array->size(), color).data());
				array = _partRenders[partName]->getPointDrawable()->getVertexAttribArray(1);
				array->updata(QVector<QVector3D>(array->size(), color).data());
			}
		}
	}
	void mPreGeoModelRender::updateAllModelOperate(MBasicFunction::ModelOperateEnum postModelOperate)
	{
		std::set<QString> partnames{ _partOrder.begin(),_partOrder.end()};
		if (postModelOperate == ImportPart)
		{
			UpdateModelBuffer();
			setShowFuntion(_rendStatus->_showFunction);
			//UpdateRigidWallBuffer();
			//updateAllModelOperate(UpdateMinMax);
		}
		else if (postModelOperate == HideAllPart)
		{
			updateOneModelOperate(QPair<MBasicFunction::ModelOperateEnum, std::set<QString>>(HideOnePart, partnames));
		}
		else if (postModelOperate == ShowAllPart)
		{
			updateOneModelOperate(QPair<MBasicFunction::ModelOperateEnum, std::set<QString>>(ShowOnePart, partnames));
		}
		else if (postModelOperate == ColorAllPart)
		{
			updateOneModelOperate(QPair<MBasicFunction::ModelOperateEnum, std::set<QString>>(ColorOnePart, partnames));
		}
	}

	void mPreGeoModelRender::setShowFuntion(ShowFuntion showFuntion)
	{
		if (_faceStateSet)
		{
			for (auto iter : _partRenders)
			{
				iter->setShowFuntion(showFuntion);
			}
		}
	}

	Space::AABB mPreGeoModelRender::getModelAABB()
	{
		Space::AABB aabb;
		for(auto partrend : _partRenders)
		{
			if (partrend->getPartSpaceTree())
			{
				aabb.push(partrend->getPartSpaceTree()->space);
			}
		}
		return aabb;
	}
	Space::SpaceTree *mPreGeoModelRender::getPartSpaceTree(QString partName)
	{
		return _partRenders.value(partName)->getPartSpaceTree();
	}
	mPreGeoPartRender::mPreGeoPartRender(std::shared_ptr<mxr::Group> parent, MDataGeo::mGeoPartData1 * part)
	{
		_facerend = MakeAsset<mGroupRender5<Vec3Array, Vec3Array, FloatArray, FloatArray, Vec3Array>>(_geode);
		_facetransparentnodeformationrend = MakeAsset<mGroupRender1<Vec3Array>>(_geode);
		_edgelinerend = MakeAsset<mGroupRender2<Vec3Array, Vec3Array>>(_geode);
		_facelinerend = MakeAsset<mGroupRender3<Vec3Array, Vec3Array, FloatArray>>(_geode);
		_linerend = MakeAsset<mGroupRender5<Vec3Array, Vec3Array, FloatArray, FloatArray, Vec3Array>>(_geode);
		_pointrend = MakeAsset<mGroupRender5<Vec3Array, Vec3Array, FloatArray, FloatArray, Vec3Array>>(_geode);
	}
	mPreGeoPartRender::~mPreGeoPartRender()
	{
		_parent->removeChild(_geode);
		Space::destoryTree(_spaceTree);
	}
	void mPreGeoPartRender::setFaceStateSet(std::shared_ptr<mxr::StateSet> faceStateSet)
	{
		_facerend->setStateSet(faceStateSet);
	}
	void mPreGeoPartRender::setFaceTransparentNodeformationStateSet(std::shared_ptr<mxr::StateSet> faceTransparentNoDeformationStateSet)
	{
		_facetransparentnodeformationrend->setStateSet(faceTransparentNoDeformationStateSet);
	}
	void mPreGeoPartRender::setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> edgelineStateSet)
	{
		_edgelinerend->setStateSet(edgelineStateSet);
	}

	void mPreGeoPartRender::setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet)
	{
		_facelinerend->setStateSet(facelineStateSet);
	}
	void mPreGeoPartRender::setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet)
	{
		_linerend->setStateSet(lineStateSet);
	}
	void mPreGeoPartRender::setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet)
	{
		_pointrend->setStateSet(pointStateSet);
	}
	void mPreGeoPartRender::calculateSpaceTreeThread()
	{

	}
	Space::SpaceTree * mPreGeoPartRender::getPartSpaceTree()
	{
		return nullptr;
	}
	void mPreGeoPartRender::ImportPart()
	{

	}
	void mPreGeoPartRender::setShowFuntion(ShowFuntion showFuntion)
	{

	}
}