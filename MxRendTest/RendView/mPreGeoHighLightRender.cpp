#include "mPreGeoHighLightRender.h"

#include "mPreRendStatus.h"
#include <renderpch.h>
#include "Space.h"
#include "texture.h"

#include "mShaderManage.h"
#include "mTextureManage.h"

//视图类
#include "mModelView.h"
#include "mViewBase.h"

#include <QMatrix3x3>
#include <QFileDialog>
#include <QFileInfo>
#include <QApplication>

//MDataPost
#include "mGeoModelData1.h"
#include "mGeoPartData1.h"
#include "mGeoSolidData1.h"
#include "mGeoFaceData1.h"
#include "mGeoLineData1.h"
#include "mGeoPointData1.h"
#include "mGeoPickData1.h"

using namespace mxr;
using namespace std;
namespace MPreRend
{
	mPreGeoHighLightRender::mPreGeoHighLightRender(shared_ptr<mPreRendStatus> rendStatus, mGeoPickData1 *geoPickData, mGeoModelData1 *geoModelData):_rendStatus(rendStatus), _geoPickData(geoPickData),_geoModelData(geoModelData)
	{
		_geode = MakeAsset<Geode>();
		_viewer = nullptr;
		_faceRender = nullptr;
		_lineRender = nullptr;
		_pointRender = nullptr;

		this->initial();
	}
	mPreGeoHighLightRender::~mPreGeoHighLightRender()
	{
		
	}

	void mPreGeoHighLightRender::updateHighLightRender()
	{

		_faceRender = make_shared<mGroupRender1<Vec3Array>>(_geode);
		_lineRender = make_shared<mGroupRender1<Vec3Array>>(_geode);
		_pointRender = make_shared<mGroupRender1<Vec3Array>>(_geode);

		_faceRender->setStateSet(_faceStateSet);
		_lineRender->setStateSet(_lineStateSet);
		_pointRender->setStateSet(_pointStateSet);

		QVector<QVector3D> pointVertex;
		QVector<QVector3D> lineVertex;
		QVector<QVector3D> faceVertex;

		set<QString> partNames = _geoPickData->getPickGeoPartNames();
		for (QString partName : partNames)
		{
			MDataGeo::mGeoPartData1 *partData = _geoModelData->getGeoPartDataByPartName(partName);
			if (partData == nullptr)
			{
				continue;
			}
			if (!partData->getPartVisual())
			{
				continue;
			}
			int shapeType = partData->getGeoShapeType();
			//混合即可
			if (shapeType == 0)
			{
				set<int> faceIds = partData->getGeoFaceIDs();
				for (int faceId : faceIds)
				{
					MDataGeo::mGeoFaceData1* geoFaceData = _geoModelData->getGeoFaceDataByID(faceId);
					if (geoFaceData == nullptr)
					{
						continue;
					}
					faceVertex.append(geoFaceData->getGeoFaceVertex());
				}
				if (faceIds.size() != 0)
				{
					continue;
				}
				set<int> lineIds = partData->getGeoLineIDs();
				for (int lineID : lineIds)
				{
					MDataGeo::mGeoLineData1* geoLineData = _geoModelData->getGeoLineDataByID(lineID);
					if (geoLineData == nullptr)
					{
						continue;
					}
					lineVertex.append(geoLineData->getGeoLineVertex());
				}
				if (lineIds.size() != 0)
				{
					continue;
				}
				set<int> pointIds = partData->getGeoPointIDs();
				for (int pointId : pointIds)
				{
					MDataGeo::mGeoPointData1* geoPointData = _geoModelData->getGeoPointDataByID(pointId);
					if (geoPointData == nullptr)
					{
						continue;
					}
					pointVertex.append(geoPointData->getGeoPointVertex());
				}
			}
			else if (shapeType == 1 || shapeType == 2 || shapeType == 4)
			{
				set<int> faceIds = partData->getGeoFaceIDs();
				for (int faceId : faceIds)
				{
					MDataGeo::mGeoFaceData1* geoFaceData = _geoModelData->getGeoFaceDataByID(faceId);
					if (geoFaceData == nullptr)
					{
						continue;
					}
					faceVertex.append(geoFaceData->getGeoFaceVertex());
				}
			}
			//高亮线即可
			else if (shapeType == 6)
			{
				set<int> lineIds = partData->getGeoLineIDs();
				for (int lineID : lineIds)
				{
					MDataGeo::mGeoLineData1* geoLineData = _geoModelData->getGeoLineDataByID(lineID);
					if (geoLineData == nullptr)
					{
						continue;
					}
					lineVertex.append(geoLineData->getGeoLineVertex());
				}
			}
			//高亮点即可
			else if (shapeType == 7)
			{
				set<int> pointIds = partData->getGeoPointIDs();
				for (int pointId : pointIds)
				{
					MDataGeo::mGeoPointData1* geoPointData = _geoModelData->getGeoPointDataByID(pointId);
					if (geoPointData == nullptr)
					{
						continue;
					}
					pointVertex.append(geoPointData->getGeoPointVertex());
				}
			}
		}

		set<int> pointids = _geoPickData->getPickPointIDs();
		for (auto iter = pointids.begin(); iter != pointids.end(); ++iter)
		{
			MDataGeo::mGeoPointData1* geoPointData = _geoModelData->getGeoPointDataByID(*iter);
			if (geoPointData == nullptr)
			{
				continue;
			}
			pointVertex.append(geoPointData->getGeoPointVertex());
		}

		set<int> lineids = _geoPickData->getPickLineIDs();
		for (auto iter = lineids.begin(); iter != lineids.end(); ++iter)
		{
			MDataGeo::mGeoLineData1* geoLineData = _geoModelData->getGeoLineDataByID(*iter);
			if (geoLineData == nullptr)
			{
				continue;
			}
			lineVertex += geoLineData->getGeoLineVertex();
		}

		set<int> solidids = _geoPickData->getPickSolidIDs();
		for (auto iter = solidids.begin(); iter != solidids.end(); ++iter)
		{
			MDataGeo::mGeoSolidData1* geoSolidData = _geoModelData->getGeoSolidDataByID(*iter);
			if (geoSolidData == nullptr)
			{
				continue;
			}
			set<int> faceIds = geoSolidData->getGeoSolidFaceIDs();
			for (int faceId : faceIds)
			{
				MDataGeo::mGeoFaceData1* geoFaceData = _geoModelData->getGeoFaceDataByID(faceId);
				if (geoFaceData == nullptr)
				{
					continue;
				}
				faceVertex += geoFaceData->getGeoFaceVertex();
			}
		}

		set<int> faceids = _geoPickData->getPickFaceIDs();
		for (auto iter = faceids.begin(); iter != faceids.end(); ++iter)
		{
			MDataGeo::mGeoFaceData1* geoFaceData = _geoModelData->getGeoFaceDataByID(*iter);
			if (geoFaceData == nullptr)
			{
				continue;
			}
			faceVertex += geoFaceData->getGeoFaceVertex();
		}

		QHashIterator<int, QVector<QVector3D>> iter(_geoPickData->getPickVertexOnGeoLineDatas());
		while (iter.hasNext())
		{
			iter.next();
			pointVertex.append(iter.value());
		}

		QHashIterator<int, QVector<QVector3D>> iter1(_geoPickData->getPickVertexOnGeoFaceDatas());
		while (iter1.hasNext())
		{
			iter1.next();
			pointVertex.append(iter1.value());
		}

		pointVertex.append(_geoPickData->getPickVertexOnScreenDatas());
		_pointRender->_vertex0->append(pointVertex);
		_lineRender->_vertex0->append(lineVertex);
		_faceRender->_vertex0->append(faceVertex);
	}

	void mPreGeoHighLightRender::updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView)
	{
		//_modelRender->setDeformationScale(_oneFrameRendData->getDeformationScale());
		_faceStateSet->getUniform("projection")->SetData(modelView->_projection);
		_faceStateSet->getUniform("view")->SetData(modelView->_view);
		_faceStateSet->getUniform("model")->SetData(modelView->_model);
		_lineStateSet->getUniform("projection")->SetData(modelView->_projection);
		_lineStateSet->getUniform("view")->SetData(modelView->_view);
		_lineStateSet->getUniform("model")->SetData(modelView->_model);
		_pointStateSet->getUniform("projection")->SetData(modelView->_projection);
		_pointStateSet->getUniform("view")->SetData(modelView->_view);
		_pointStateSet->getUniform("model")->SetData(modelView->_model);

		_pointStateSet->getUniform("viewPos")->SetData(modelView->_Eye);

		if (_rendStatus->_lightIsDependOnCamera)
		{
			_pointStateSet->getUniform("light.position")->SetData(2 * modelView->_Eye - modelView->_Center);
		}
		else
		{
			_pointStateSet->getUniform("light.position")->SetData(_rendStatus->_postLight.lightPosition);
		}

		_viewer->noClearRun();
	}


	void mPreGeoHighLightRender::setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> edgelineStateSet)
	{
		_lineStateSet = edgelineStateSet;
		//_modelRender->setEdgeLineStateSet(_lineStateSet);
	}

	void mPreGeoHighLightRender::setFaceStateSet(std::shared_ptr<mxr::StateSet> faceStateSet)
	{
		_faceStateSet = faceStateSet;
		//_modelRender->setFaceLineStateSet(_faceStateSet);
	}

	void mPreGeoHighLightRender::setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet)
	{
		_lineStateSet = lineStateSet;
		//_modelRender->setLineStateSet(_lineStateSet);
	}

	void mPreGeoHighLightRender::setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet)
	{
		_pointStateSet = pointStateSet;
		//_modelRender->setPointStateSet(_pointStateSet);
	}

	void mPreGeoHighLightRender::initial()
	{
		if (!_viewer)
		{
			_viewer = MakeAsset<mxr::Viewer>();
			_viewer->setSceneData(_geode);
		}

		_faceStateSet = MakeAsset<StateSet>();
		shared_ptr<Shader> facelineshader = mShaderManage::GetInstance()->GetShader("PreHighLightGeoMeshFace");
		_faceStateSet->setShader(facelineshader);
		_faceStateSet->setDrawMode(GL_TRIANGLES);
		_faceStateSet->setAttributeAndModes(MakeAsset<Depth>(), 0);
		_faceStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(PolygonMode::FRONT_AND_BACK, PolygonMode::FILL), 1);
		_faceStateSet->setAttributeAndModes(MakeAsset<PolygonOffsetLine>(-1, -1), 1);

		_faceStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_faceStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_faceStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_faceStateSet->setUniform(MakeAsset<Uniform>("showColor", QVector3D(1,1,1)));

		//edgeline
		_lineStateSet = MakeAsset<StateSet>();
		shared_ptr<Shader> meshlineshader = mShaderManage::GetInstance()->GetShader("PreHighLightGeoMeshLine");
		_lineStateSet->setShader(meshlineshader);
		_lineStateSet->setDrawMode(GL_LINES);
		_lineStateSet->setAttributeAndModes(MakeAsset<Depth>(), 0);
		_lineStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(mxr::PolygonMode::FRONT_AND_BACK, mxr::PolygonMode::FILL), 1);

		_lineStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_lineStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_lineStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_lineStateSet->setUniform(MakeAsset<Uniform>("showColor", QVector3D(1, 1, 1)));

		_pointStateSet = MakeAsset<StateSet>();
		shared_ptr<Shader> pointshader = mShaderManage::GetInstance()->GetShader("PreHighLightGeoPoint");
		_pointStateSet->setShader(pointshader);
		_pointStateSet->setDrawMode(GL_POINTS);
		_pointStateSet->setAttributeAndModes(MakeAsset<Depth>(), 0);
		_pointStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(mxr::PolygonMode::FRONT_AND_BACK, mxr::PolygonMode::FILL), 1);

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
		_pointStateSet->setUniform(MakeAsset<Uniform>("showColor", QVector3D(1, 1, 1)));
		_pointStateSet->setTexture("sprite_texture", mTextureManage::GetInstance()->GetTexture("GeoPoint.png", 3));
	}

}