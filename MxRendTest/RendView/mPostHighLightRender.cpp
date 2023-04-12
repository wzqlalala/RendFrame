#include "mPostHighLightRender.h"

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

//MDataPost
#include "mPostOneFrameRendData.h"
#include "mPostMeshPickData.h"
#include "mDataPost1.h"
#include "mOneFrameData1.h"
#include "mPostMeshPartData1.h"
#include "mPostMeshData1.h"
#include "mPostMeshFaceData1.h"
#include "mPostMeshLineData1.h"
#include "mPostMeshNodeData1.h"

using namespace mxr;
using namespace std;
namespace MPostRend
{
	mPostHighLightRender::mPostHighLightRender(shared_ptr<mPostRendStatus> rendStatus, mPostMeshPickData *meshPickData):_rendStatus(rendStatus), _meshPickData(meshPickData)
	{
		_geode = MakeAsset<Geode>();
		_viewer = nullptr;
		_facelineRender = nullptr;
		_lineRender = nullptr;
		_pointRender = nullptr;

		this->initial();
	}
	mPostHighLightRender::~mPostHighLightRender()
	{
		
	}

	void mPostHighLightRender::updateHighLightRender(mOneFrameData1 *oneFrameData, mPostOneFrameRendData* oneFrameRendData)
	{
		_oneFrameData = oneFrameData;
		_oneFrameRendData = oneFrameRendData;

		_facelineRender = make_shared<mPostBaseHighLightRender1>(_geode);
		_lineRender = make_shared<mPostBaseHighLightRender>(_geode);
		_pointRender = make_shared<mPostBaseHighLightRender>(_geode);

		_facelineRender->setStateSet(_facelineStateSet);
		_lineRender->setStateSet(_lineStateSet);
		_pointRender->setStateSet(_pointStateSet);

		if (oneFrameData == nullptr || oneFrameRendData == nullptr)
		{
			return;
		}

		const QHash<int, QVector3D> &dis = oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = oneFrameRendData->getDeformationScale();

		QVector3D vertex0, vertex1, vertex2, vertex3, vertex4, vertex5, vertex6, vertex7;
		set<int> nodeids = _meshPickData->getPickNodeIDs();
		for (auto iter = nodeids.begin(); iter != nodeids.end(); ++iter)
		{
			mPostMeshNodeData1 *nodeData = oneFrameData->getNodeDataByID(*iter);
			if (nodeData == nullptr)
			{
				continue;
			}
			_pointRender->_vertex0->append(nodeData->getNodeVertex() + deformationScale * dis.value(*iter));
		}

		QVector<int> nodeorderids = _meshPickData->getPickNodeIDsOrder();
		for (int nodeId : nodeorderids)
		{
			mPostMeshNodeData1 *nodeData = oneFrameData->getNodeDataByID(nodeId);
			if (nodeData == nullptr)
			{
				continue;
			}
			_pointRender->_vertex0->append(nodeData->getNodeVertex() + deformationScale * dis.value(nodeId));
		}

		set<int> meshIDs = _meshPickData->getPickMeshIDs();
		for (int meshID : meshIDs)
		{
			mPostMeshData1 *meshData = oneFrameData->getMeshDataByID(meshID);
			if (meshData == nullptr)
			{
				continue;
			}
			QVector<int> index = meshData->getNodeIndex();
			switch (meshData->getMeshType())
			{
			case MeshPoint:
			{
				vertex0 = oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
				_pointRender->_vertex0->append(vertex0);
				break;
			}
			case MeshBeam:
			{
				vertex0 = oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
				vertex1 = oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
				_lineRender->_vertex0->append(vertex0);
				_lineRender->_vertex0->append(vertex1);
				break;
			}
			case MeshTri:
			{
				vertex0 = oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
				vertex1 = oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
				vertex2 = oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex0->append(vertex1);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex1->append(QVector<float>(3,1));

				break;
			}
			case MeshQuad:
			{
				vertex0 = oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
				vertex1 = oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
				vertex2 = oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
				vertex3 = oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex0->append(vertex1);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex0->append(vertex3);
				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex1->append(QVector<float>(6, 0));
				break;
			}
			case MeshTet:
			{
				vertex0 = oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
				vertex1 = oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
				vertex2 = oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
				vertex3 = oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex0->append(vertex1);

				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex0->append(vertex1);
				_facelineRender->_vertex0->append(vertex3);

				_facelineRender->_vertex0->append(vertex1);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex0->append(vertex3);

				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex0->append(vertex3);
				_facelineRender->_vertex0->append(vertex2);

				_facelineRender->_vertex1->append(QVector<float>(12, 1));
				break;
			}
			case MeshPyramid:
			{
				vertex0 = oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
				vertex1 = oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
				vertex2 = oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
				vertex3 = oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
				vertex4 = oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));

				_facelineRender->_vertex0->append(vertex1);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex0->append(vertex4);

				_facelineRender->_vertex0->append(vertex4);
				_facelineRender->_vertex0->append(vertex3);
				_facelineRender->_vertex0->append(vertex2);

				_facelineRender->_vertex0->append(vertex4);
				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex0->append(vertex3);

				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex0->append(vertex1);
				_facelineRender->_vertex0->append(vertex4);

				_facelineRender->_vertex1->append(QVector<float>(12, 1));

				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex0->append(vertex1);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex0->append(vertex3);
				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex1->append(QVector<float>(6, 0));
				break;
			}
			case MeshWedge:
			{
				vertex0 = oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
				vertex1 = oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
				vertex2 = oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
				vertex3 = oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
				vertex4 = oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));
				vertex5 = oneFrameData->getNodeDataByID(index.at(5))->getNodeVertex() + deformationScale * dis.value(index.at(5));

				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex0->append(vertex1);

				_facelineRender->_vertex0->append(vertex3);
				_facelineRender->_vertex0->append(vertex4);
				_facelineRender->_vertex0->append(vertex5);

				_facelineRender->_vertex1->append(QVector<float>(6, 1));

				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex0->append(vertex1);
				_facelineRender->_vertex0->append(vertex4);
				_facelineRender->_vertex0->append(vertex4);
				_facelineRender->_vertex0->append(vertex3);
				_facelineRender->_vertex0->append(vertex0);

				_facelineRender->_vertex0->append(vertex1);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex0->append(vertex5);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex0->append(vertex5);
				_facelineRender->_vertex0->append(vertex1);

				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex0->append(vertex3);
				_facelineRender->_vertex0->append(vertex3);
				_facelineRender->_vertex0->append(vertex5);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex1->append(QVector<float>(18, 0));
				break;
			}
			case MeshHex:
			{
				vertex0 = oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
				vertex1 = oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
				vertex2 = oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
				vertex3 = oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
				vertex4 = oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));
				vertex5 = oneFrameData->getNodeDataByID(index.at(5))->getNodeVertex() + deformationScale * dis.value(index.at(5));
				vertex6 = oneFrameData->getNodeDataByID(index.at(6))->getNodeVertex() + deformationScale * dis.value(index.at(6));
				vertex7 = oneFrameData->getNodeDataByID(index.at(7))->getNodeVertex() + deformationScale * dis.value(index.at(7));

				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex0->append(vertex3);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex0->append(vertex1);
				_facelineRender->_vertex0->append(vertex0);

				_facelineRender->_vertex0->append(vertex4);
				_facelineRender->_vertex0->append(vertex5);
				_facelineRender->_vertex0->append(vertex6);
				_facelineRender->_vertex0->append(vertex6);
				_facelineRender->_vertex0->append(vertex7);
				_facelineRender->_vertex0->append(vertex4);

				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex0->append(vertex1);
				_facelineRender->_vertex0->append(vertex5);
				_facelineRender->_vertex0->append(vertex5);
				_facelineRender->_vertex0->append(vertex4);
				_facelineRender->_vertex0->append(vertex0);

				_facelineRender->_vertex0->append(vertex1);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex0->append(vertex6);
				_facelineRender->_vertex0->append(vertex6);
				_facelineRender->_vertex0->append(vertex5);
				_facelineRender->_vertex0->append(vertex1);

				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex0->append(vertex3);
				_facelineRender->_vertex0->append(vertex7);
				_facelineRender->_vertex0->append(vertex7);
				_facelineRender->_vertex0->append(vertex6);
				_facelineRender->_vertex0->append(vertex2);

				_facelineRender->_vertex0->append(vertex3);
				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex0->append(vertex4);
				_facelineRender->_vertex0->append(vertex4);
				_facelineRender->_vertex0->append(vertex7);
				_facelineRender->_vertex0->append(vertex3);

				_facelineRender->_vertex1->append(QVector<float>(36, 0));
				break;
			}
			default:break;
			}
		}

		set<int> meshfaceids = _meshPickData->getPickMeshFaceIDs();
		for (auto iter = meshfaceids.begin(); iter != meshfaceids.end(); ++iter)
		{
			mPostMeshFaceData1 *meshFaceData = oneFrameData->getMeshFaceDataByID(*iter);
			if (meshFaceData == nullptr)
			{
				continue;
			}
			QVector<int> index = meshFaceData->getNodeIndex();
			if (index.size() == 3)
			{
				vertex0 = oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
				vertex1 = oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
				vertex2 = oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex0->append(vertex1);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex1->append(QVector<float>(3, 1));
			}
			else
			{
				vertex0 = oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
				vertex1 = oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
				vertex2 = oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
				vertex3 = oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex0->append(vertex1);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex0->append(vertex2);
				_facelineRender->_vertex0->append(vertex3);
				_facelineRender->_vertex0->append(vertex0);
				_facelineRender->_vertex1->append(QVector<float>(6, 0));
			}
		}

		set<QString> partNames = _meshPickData->getPickMeshPartNames();
		for (QString partName : partNames)
		{
			mPostMeshPartData1 *partData = oneFrameData->getMeshPartDataByPartName(partName);
			if (partData == nullptr)
			{
				continue;
			}
			
			//边界线
			set<int> meshLineIDs = partData->getMeshLineIDs();
			for (int meshLineID : meshLineIDs)
			{
				mPostMeshLineData1 *meshLineData = oneFrameData->getMeshLineDataByID(meshLineID);
				if (meshLineData == nullptr)
				{
					continue;
				}
				mPostMeshData1 *meshData = oneFrameData->getMeshDataByID(meshLineData->getMeshID());
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				_lineRender->_vertex0->append(oneFrameData->getNodeDataByID(meshLineData->getMeshLineNodeIndex1())->getNodeVertex() + deformationScale * dis.value(meshLineData->getMeshLineNodeIndex1()));
				_lineRender->_vertex0->append(oneFrameData->getNodeDataByID(meshLineData->getMeshLineNodeIndex2())->getNodeVertex() + deformationScale * dis.value(meshLineData->getMeshLineNodeIndex2()));
			}

			//一维网格
			meshIDs = partData->getMeshIDs1();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				if (meshData->getMeshType() == MeshBeam)
				{
					QVector<int> index = meshData->getNodeIndex();
					for (int j = 0; j < 2; ++j)
					{
						vertex0 = oneFrameData->getNodeDataByID(index.at(j))->getNodeVertex() + deformationScale * dis.value(index.at(j));
						_lineRender->_vertex0->append(vertex0);
					}
				}
			}

			//0维网格
			set<int> massIDs = partData->getMeshIDs0();
			for (int meshID : massIDs)
			{
				mPostMeshData1 *meshData = oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				if (meshData->getMeshType() == MeshPoint)
				{
					vertex0 = oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
					_pointRender->_vertex0->append(vertex0);
				}
			}
		}
	}

	void mPostHighLightRender::updateUniform(shared_ptr<mModelView> modelView, shared_ptr<mCommonView> commonView)
	{
		//_modelRender->setDeformationScale(_oneFrameRendData->getDeformationScale());
		_facelineStateSet->getUniform("projection")->SetData(modelView->_projection);
		_facelineStateSet->getUniform("view")->SetData(modelView->_view);
		_facelineStateSet->getUniform("model")->SetData(modelView->_model);
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


	void mPostHighLightRender::setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> edgelineStateSet)
	{
		_lineStateSet = edgelineStateSet;
		//_modelRender->setEdgeLineStateSet(_lineStateSet);
	}

	void mPostHighLightRender::setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet)
	{
		_facelineStateSet = facelineStateSet;
		//_modelRender->setFaceLineStateSet(_facelineStateSet);
	}

	void mPostHighLightRender::setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet)
	{
		_lineStateSet = lineStateSet;
		//_modelRender->setLineStateSet(_lineStateSet);
	}

	void mPostHighLightRender::setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet)
	{
		_pointStateSet = pointStateSet;
		//_modelRender->setPointStateSet(_pointStateSet);
	}

	void mPostHighLightRender::initial()
	{
		if (!_viewer)
		{
			_viewer = MakeAsset<mxr::Viewer>();
			_viewer->setSceneData(_geode);
		}

		_facelineStateSet = MakeAsset<StateSet>();
		shared_ptr<Shader> facelineshader = mShaderManage::GetInstance()->GetShader("PostHighLightFaceLine");
		_facelineStateSet->setShader(facelineshader);
		_facelineStateSet->setDrawMode(GL_TRIANGLES);
		_facelineStateSet->setAttributeAndModes(MakeAsset<Depth>(), 0);
		_facelineStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(PolygonMode::FRONT_AND_BACK, PolygonMode::LINE), 1);
		_facelineStateSet->setAttributeAndModes(MakeAsset<PolygonOffsetLine>(-1, -1), 1);

		_facelineStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_facelineStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_facelineStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));

		//edgeline
		_lineStateSet = MakeAsset<StateSet>();
		shared_ptr<Shader> meshlineshader = mShaderManage::GetInstance()->GetShader("PostHighLightLine");
		_lineStateSet->setShader(meshlineshader);
		_lineStateSet->setDrawMode(GL_LINES);
		_lineStateSet->setAttributeAndModes(MakeAsset<Depth>(), 0);
		_lineStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(mxr::PolygonMode::FRONT_AND_BACK, mxr::PolygonMode::FILL), 1);

		_lineStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_lineStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_lineStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));

		_pointStateSet = MakeAsset<StateSet>();
		shared_ptr<Shader> pointshader = mShaderManage::GetInstance()->GetShader("PostHighLightPoint");
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
	}

	mPostBaseHighLightRender::mPostBaseHighLightRender(std::shared_ptr<mxr::Group> parent)
	{
		_drawable = MakeAsset<Drawable>();
		_vertex0 = MakeAsset<Vec3Array>();
		_drawable->setVertexAttribArray(0, _vertex0);

		_parent = parent;
		_parent->addChild(_drawable);
	}

	mPostBaseHighLightRender::~mPostBaseHighLightRender()
	{
		_parent->removeChild(_drawable);
	}

	void mPostBaseHighLightRender::setStateSet(std::shared_ptr<mxr::StateSet> state)
	{
		_drawable->setStateSet(state);
	}


	mPostBaseHighLightRender1::mPostBaseHighLightRender1(std::shared_ptr<mxr::Group> parent)
	{
		_drawable = MakeAsset<Drawable>();
		_vertex0 = MakeAsset<Vec3Array>();
		_vertex1 = MakeAsset<FloatArray>();
		_drawable->setVertexAttribArray(0, _vertex0);
		_drawable->setVertexAttribArray(1, _vertex1);

		_parent = parent;
		_parent->addChild(_drawable);
	}

	mPostBaseHighLightRender1::~mPostBaseHighLightRender1()
	{
		_parent->removeChild(_drawable);
	}

	void mPostBaseHighLightRender1::setStateSet(std::shared_ptr<mxr::StateSet> state)
	{
		_drawable->setStateSet(state);
	}

}