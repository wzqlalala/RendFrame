#include "mPreGeoModelRender.h"

//MDataGeo
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

//视图类
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

	void mPreGeoModelRender::setFaceStateSet(std::shared_ptr<mxr::StateSet> stateset)
	{
		_faceStateSet = stateset;
		//_facerend->setStateSet(_faceStateSet);
	}

	void mPreGeoModelRender::setIndependentLineStateSet(std::shared_ptr<mxr::StateSet> stateset)
	{
		_indepentdentlineStateSet = stateset;
	}

	void mPreGeoModelRender::setDotLineStateSet(std::shared_ptr<mxr::StateSet> stateset)
	{
		_dotlineStateSet = stateset;
	}

	void mPreGeoModelRender::setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> stateset)
	{
		_edgelineStateSet = stateset;
		//_meshlinerend->setStateSet(_edgelineStateSet);
	}

	void mPreGeoModelRender::setPointStateSet(std::shared_ptr<mxr::StateSet> stateset)
	{
		_pointStateSet = stateset;
	}
	mPreGeoModelRender::~mPreGeoModelRender()
	{
		_parent->removeChild(_geode);
		_partRenders.clear();
	}
	bool mPreGeoModelRender::updateRender()
	{
		if (!_geoModelData->_modelOperate.empty())
		{
			bool isUpdateCamera{false};
			while (!_geoModelData->_modelOperate.empty())
			{
				isUpdateCamera = isUpdateCamera | updateModelOperate(_geoModelData->_modelOperate.takeFirst());
			}
			return isUpdateCamera;
		}
		return false;
	}
	void mPreGeoModelRender::UpdateModelBuffer()
	{
		std::set<QString> hidePartNames;
		//auto iter = _oneFrameData->getMeshPartIterator();
		QList<QString> partOrder = _geoModelData->getAllPartName();
		for (auto partName : partOrder)
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
			asset_ref<mPreGeoPartRender> part = MakeAsset<mPreGeoPartRender>(_geode, partData, _geoModelData);
			part->setFaceStateSet(_faceStateSet);
			part->setEdgeLineStateSet(_edgelineStateSet);
			part->setIndependentLineStateSet(_indepentdentlineStateSet);
			part->setDotLineStateSet(_dotlineStateSet);
			part->setPointStateSet(_pointStateSet);

			_partRenders.insert(partName, part);
		}
		updateModelOperate(QPair<MBasicFunction::ModelOperateEnum, std::set<QString>>(HideOnePart, hidePartNames));
	}
	bool mPreGeoModelRender::updateModelOperate(QPair<MBasicFunction::ModelOperateEnum, std::set<QString>> postModelOperates)
	{
		ModelOperateEnum postModelOperate = postModelOperates.first;
		set<QString> partNames = postModelOperates.second;
		if (postModelOperate == AppendOnePart || postModelOperate == ReplaceOnePart)
		{
			for (QString partName : partNames)
			{
				mGeoPartData1 *partData = _geoModelData->getGeoPartDataByPartName(partName);
				if (partData)
				{
					auto part = MakeAsset<mPreGeoPartRender>(_geode, partData, _geoModelData);
					part->setFaceStateSet(_faceStateSet);
					part->setEdgeLineStateSet(_edgelineStateSet);
					part->setIndependentLineStateSet(_indepentdentlineStateSet);
					part->setDotLineStateSet(_dotlineStateSet);
					part->setPointStateSet(_pointStateSet);
					_partRenders[partName] = part;
				}
			}
			return true;
		}
		else if (postModelOperate == DeleteOnePart)
		{
			for (QString partName : partNames)
			{
				_partRenders.remove(partName);
			}
			return true;
		}
		else if (postModelOperate == HideOnePart)
		{
			for (QString partName : partNames)
			{
				_partRenders[partName]->getGeode()->setNodeMask(1);
			}
			return true;
		}
		else if (postModelOperate == ShowOnePart)
		{
			for (QString partName : partNames)
			{
				_partRenders[partName]->getGeode()->setNodeMask(0);
			}
			return true;
		}
		else if (postModelOperate == ColorOnePart)
		{
			for (QString partName : partNames)
			{
				QVector3D color = _geoModelData->getGeoPartColor(partName);
				Array *array = _partRenders[partName]->getFaceDrawable()->getVertexAttribArray(1);
				array->updata(QVector<QVector3D>(array->size(), color).data());
				array = _partRenders[partName]->getIndependentLineDrawable()->getVertexAttribArray(1);
				array->updata(QVector<QVector3D>(array->size(), color).data());
				array = _partRenders[partName]->getPointDrawable()->getVertexAttribArray(1);
				array->updata(QVector<QVector3D>(array->size(), color).data());
			}
		}
		else
		{
			QList<QString> partNames = _partRenders.keys();
			std::set<QString> partnames{ partNames.begin(),partNames.end() };
			if (postModelOperate == ImportPart)
			{
				UpdateModelBuffer();
				setShowFuntion(_rendStatus->_showFunction);
				//UpdateRigidWallBuffer();
				//updateAllModelOperate(UpdateMinMax);
			}
			else if (postModelOperate == HideAllPart)
			{
				updateModelOperate(QPair<MBasicFunction::ModelOperateEnum, std::set<QString>>(HideOnePart, partnames));
			}
			else if (postModelOperate == ShowAllPart)
			{
				updateModelOperate(QPair<MBasicFunction::ModelOperateEnum, std::set<QString>>(ShowOnePart, partnames));
			}
			else if (postModelOperate == ColorAllPart)
			{
				updateModelOperate(QPair<MBasicFunction::ModelOperateEnum, std::set<QString>>(ColorOnePart, partnames));
			}
			else if (postModelOperate == DeleteAllPart)
			{
				updateModelOperate(QPair<MBasicFunction::ModelOperateEnum, std::set<QString>>(DeleteOnePart, partnames));
			}
		}
		return false;
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
			aabb.push(partrend->getPartAABB());			
		}
		return aabb;
	}
	Space::AABB mPreGeoModelRender::getPartAABB(QString partName)
	{
		return _partRenders.value(partName)->getPartAABB();
	}
	mPreGeoPartRender::mPreGeoPartRender(std::shared_ptr<mxr::Group> parent, MDataGeo::mGeoPartData1 * part, mGeoModelData1 *geoModelData):
		_parent(parent),_partData(part)
	{
		_geode = MakeAsset<Geode>();
		_parent->addChild(_geode);

		_facerend = MakeAsset<mGroupRender2<Vec3Array, Vec3Array>>(_geode);
		_edgelinerend = MakeAsset<mGroupRender1<Vec3Array>>(_geode);
		_independentlinerend = MakeAsset<mGroupRender2<Vec3Array, Vec3Array>>(_geode);
		_dotlinerend = MakeAsset<mGroupRender2<Vec3Array, Vec3Array>>(_geode);
		_pointrend = MakeAsset<mGroupRender2<Vec3Array, Vec3Array>>(_geode);

		appendPart(geoModelData);
	}
	mPreGeoPartRender::~mPreGeoPartRender()
	{
		_parent->removeChild(_geode);
	}
	void mPreGeoPartRender::setFaceStateSet(std::shared_ptr<mxr::StateSet> stateset)
	{
		_facerend->setStateSet(stateset);
	}
	void mPreGeoPartRender::setIndependentLineStateSet(std::shared_ptr<mxr::StateSet> stateset)
	{
		_independentlinerend->setStateSet(stateset);
	}
	void mPreGeoPartRender::setDotLineStateSet(std::shared_ptr<mxr::StateSet> stateset)
	{
		_dotlinerend->setStateSet(stateset);
	}
	
	void mPreGeoPartRender::setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> stateset)
	{
		_edgelinerend->setStateSet(stateset);
	}

	void mPreGeoPartRender::setPointStateSet(std::shared_ptr<mxr::StateSet> stateset)
	{
		_pointrend->setStateSet(stateset);
	}
	Space::AABB mPreGeoPartRender::getPartAABB()
	{
		return _partData->getGeoPartAABB();
	}

	void mPreGeoPartRender::appendPart(mGeoModelData1 *geoModelData)
	{
		if (_partData == nullptr)
		{
			return;
		}

		//几何点
		set<int> pointIds = _partData->getGeoPointIDs();
		for (int pointId : pointIds)
		{
			MDataGeo::mGeoPointData1* geoPointData = geoModelData->getGeoPointDataByID(pointId);
			if (geoPointData == nullptr)
			{
				continue;
			}
			if (geoPointData->getGeoPointProperty() == MDataGeo::IndependentPoint)//单独的点，渲染
			{
				_pointrend->_vertex0->append(geoPointData->getGeoPointVertex());
				_pointrend->_vertex1->append(_partData->getPartColor());
			}
		}

		if (pointIds.size() != 0)
		{

		}
		//几何线
		set<int> lineIds = _partData->getGeoLineIDs();
		for (int lineID : lineIds)
		{
			MDataGeo::mGeoLineData1* geoLineData = geoModelData->getGeoLineDataByID(lineID);
			if (geoLineData == nullptr)
			{
				continue;
			}
			switch (geoLineData->getGeoLineProperty())
			{
			case GeoLineProperty::EdgeOnFace://普通边界，黑色
				for (QVector3D vertex : geoLineData->getGeoLineVertex())
				{
					_edgelinerend->_vertex0->append(vertex);
				}
				break;
			case GeoLineProperty::DottedTopology://虚拓扑边，灰色虚线
				for (QVector3D vertex : geoLineData->getGeoLineVertex())
				{
					_dotlinerend->_vertex0->append(vertex);
				}
				break;
			case GeoLineProperty::IndependentEdge://独立边，部件颜色
				for (QVector3D vertex : geoLineData->getGeoLineVertex())
				{
					_independentlinerend->_vertex0->append(vertex);
					_independentlinerend->_vertex1->append(_partData->getPartColor());
				}
				break;
			default:
				break;
			}
		}
		//几何面
		set<int> faceIds = _partData->getGeoFaceIDs();
		for (int faceId : faceIds)
		{
			MDataGeo::mGeoFaceData1* geoFaceData = geoModelData->getGeoFaceDataByID(faceId);
			if (geoFaceData == nullptr)
			{
				continue;
			}
			for (QVector3D vertex : geoFaceData->getGeoFaceVertex())
			{
				_facerend->_vertex0->append(vertex);
				_facerend->_vertex1->append(_partData->getPartColor());
			}
		}

		if (_partData->getPartVisual())
		{
			return;
		}
		return;
	}
	void mPreGeoPartRender::setShowFuntion(ShowFuntion showFuntion)
	{

	}
}