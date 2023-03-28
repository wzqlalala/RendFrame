#include "mPostPartRender.h"
#include <renderpch.h>
#include "Space.h"

#include "mShaderManage.h"
#include "mTextureManage.h"

#include "mDataPost1.h"
#include "mOneFrameData1.h"
#include "mPostMeshPartData1.h"
#include "mPostMeshData1.h"
#include "mPostMeshFaceData1.h"
#include "mPostMeshLineData1.h"
#include "mPostMeshNodeData1.h"

//视图类
#include "mModelView.h"
#include "mCommonView.h"

#include <QMatrix3x3>
#include <QFileDialog>
#include <QFileInfo>
#include <QApplication>

#include "mBasicEnum.h"

using namespace mxr;
using namespace std;
using namespace MDataPost;
using namespace MBasicFunction;
namespace MPostRend
{
	mPostPartRender::mPostPartRender(std::shared_ptr<mxr::Group> parent, MDataPost::mOneFrameData1 * oneFrameData, MDataPost::mPostMeshPartData1 * part)
	{
		_oneFrameData = oneFrameData;
		_partData = part;
		_parent = parent;
		_spaceTree = nullptr;

		_geode = MakeAsset<Geode>();
		_parent->addChild(_geode);

		_facerend = MakeAsset<mPostPartFaceRender>(_geode);
		_facetransparentnodeformationrend = MakeAsset<mPostPartFaceTransparentNodeformationRender>(_geode);
		_edgelinerend = MakeAsset<mPostPartEdgeLineRender>(_geode);
		_facelinerend = MakeAsset<mPostPartFaceLineRender>(_geode);
		_linerend = MakeAsset<mPostPartLineRender>(_geode);
		_pointrend = MakeAsset<mPostPartPointRender>(_geode);
	}

	mPostPartRender::~mPostPartRender()
	{
		 _parent->removeChild(_geode);
		 Space::destoryTree(_spaceTree);
	}

	void mPostPartRender::setFaceStateSet(std::shared_ptr<mxr::StateSet> faceStateSet)
	{
		//_faceStateSet = faceStateSet;
		_facerend->setStateSet(faceStateSet);
	}
	void mPostPartRender::setFaceTransparentNodeformationStateSet(std::shared_ptr<mxr::StateSet> faceTransparentNoDeformationStateSet)
	{
		_facetransparentnodeformationrend->setStateSet(faceTransparentNoDeformationStateSet);
	}
	//void mPostPartRender::setFaceTransparentStateSet(std::shared_ptr<mxr::StateSet> faceTransparentStateSet)
	//{
	//	//_faceTransparentStateSet = faceTransparentStateSet;
	//	_facerend->setStateSet(faceTransparentStateSet);
	//}
	void mPostPartRender::setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> edgelineStateSet)
	{
		//_edgelineStateSet = meshlineStateSet;
		_edgelinerend->setStateSet(edgelineStateSet);
	}
	void mPostPartRender::setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet)
	{
		//_facelineStateSet = facelineStateSet;
		_facelinerend->setStateSet(facelineStateSet);
	}
	void mPostPartRender::setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet)
	{
		_linerend->setStateSet(lineStateSet);
	}
	void mPostPartRender::setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet)
	{
		_pointrend->setStateSet(pointStateSet);
	}
	void mPostPartRender::ImportPart(QVector3D deformationScale, QHash<int, float>& values, QHash<int, QVector3D>& dis, MViewBasic::NodeOrElement type)
	{
		this->deformationScale = deformationScale;
		QVector<mPostMeshData1*> meshdata0 = _partData->getMeshDatas0();
		for (int i = 0; i < meshdata0.size(); i++)
		{
			AppendMesh0(meshdata0[i], values, dis, type);
		}

		qDebug() << "0维网格" << meshdata0.size();
		
		QVector<mPostMeshData1*> meshdata1 = _partData->getMeshDatas1();
		for (int i = 0; i < meshdata1.size(); i++)
		{
			AppendMesh1(meshdata1[i], values, dis, type);
		}

		qDebug() << "1维网格" << meshdata1.size();
		
		QVector<mPostMeshData1*> meshdata2 = _partData->getMeshDatas2();
		for (int i = 0; i < meshdata2.size(); i++)
		{
			AppendMesh2(meshdata2[i], values, dis, type);
		}

		qDebug() << "2维网格" << meshdata2.size();

		QVector<mPostMeshFaceData1*> meshFaces = _partData->getMeshFaceData();
		for (int i = 0; i < meshFaces.size(); i++)
		{
			AppendMesh3(meshFaces[i], values, dis, type);
		}

		qDebug() << "3维网格表面" << meshFaces.size();

		set<int> meshLineIDs = _partData->getMeshLineIDs();
		for (int meshLineID : meshLineIDs)
		{
			mPostMeshLineData1 *meshLineData = _oneFrameData->getMeshLineDataByID(meshLineID);
			AppendMeshLine(meshLineData, dis, type);
		}

		qDebug() << "边界线" << meshLineIDs.size();

		_facetransparentnodeformationrend->getDrawable()->setVertexAttribArray(0, _facerend->_vertex0);
		_facetransparentnodeformationrend->getDrawable()->setVertexAttribArray(1, _facerend->_vertex6);

		_facelinerend->getDrawable()->setVertexAttribArray(0, _facerend->_vertex0);
		_facelinerend->getDrawable()->setVertexAttribArray(1, _facerend->_vertex4);

		qDebug() <<  _facelinerend->_vertex0->size();

		//calculateSpaceTreeThread(dis);
	}
	void mPostPartRender::UpdatePartVariableValue(QHash<int, float>& values, MViewBasic::NodeOrElement type)
	{
		/*
		QVector<float> pointValue;
		QVector<float> pointHasValue;
		QVector<mPostMeshData1*> meshdata0 = _partData->getMeshDatas0();
		for (int i = 0; i < meshdata0.size(); i++)
		{
			GetVariableValue_Mesh0(pointValue, pointHasValue, meshdata0[i], values, type);
		}
		_pointrend->getDrawable()->getVertexAttribArray(3)->updata(0, pointValue.size() * sizeof(float), pointValue.data());
		_pointrend->getDrawable()->getVertexAttribArray(6)->updata(0, pointHasValue.size() * sizeof(float), pointHasValue.data());

		QVector<float> lineValue;
		QVector<float> lineHasValue;
		QVector<mPostMeshData1*> meshdata1 = _partData->getMeshDatas1();
		for (int i = 0; i < meshdata1.size(); i++)
		{
			GetVariableValue_Mesh1(lineValue, lineHasValue, meshdata1[i], values, type);
		}
		line_rend->getDrawable()->getVertexAttribArray(3)->updata(0, lineValue.size() * sizeof(float), lineValue.data());
		line_rend->getDrawable()->getVertexAttribArray(6)->updata(0, lineHasValue.size() * sizeof(float), lineHasValue.data());
		*/
		QVector<float> faceValue;
		QVector<float> faceHasValue;
		QVector<mPostMeshData1*> meshdata2 = _partData->getMeshDatas2();
		for (int i = 0; i < meshdata2.size(); i++)
		{
			GetVariableValue_Mesh2(faceValue, faceHasValue, meshdata2[i], values, type);
		}

		QVector<mPostMeshFaceData1*> meshFaces = _partData->getMeshFaceData();
		for (int i = 0; i < meshFaces.size(); i++)
		{
			GetVariableValue_Mesh3(faceValue, faceHasValue, meshFaces[i], values, type);
		}
		_facerend->getDrawable()->getVertexAttribArray(3)->updata(0, faceValue.size() * sizeof(float), faceValue.data());
		_facerend->getDrawable()->getVertexAttribArray(6)->updata(0, faceHasValue.size() * sizeof(float), faceHasValue.data());
	}

	void mPostPartRender::setShowFuntion(ShowFuntion showFuntion)
	{
		_linerend->getDrawable()->setNodeMask(0);
		if (showFuntion == ElementFace)
		{
			_facerend->getDrawable()->setNodeMask(0);
			_facelinerend->getDrawable()->setNodeMask(1);
			_edgelinerend->getDrawable()->setNodeMask(1);
		}
		else if (showFuntion == SmoothShaded)
		{
			_facerend->getDrawable()->setNodeMask(0);
			_facelinerend->getDrawable()->setNodeMask(0);
			_edgelinerend->getDrawable()->setNodeMask(0);
		}
		else if (showFuntion == WireEdge)
		{
			_facerend->getDrawable()->setNodeMask(1);
			_facelinerend->getDrawable()->setNodeMask(1);
			_edgelinerend->getDrawable()->setNodeMask(0);
		}
	}

	void mPostPartRender::setIsShowInitialShape(bool isShowInitialShape)
	{
		if (isShowInitialShape)
		{
			_facetransparentnodeformationrend->getDrawable()->setNodeMask(0);
		}
		else
		{
			_facetransparentnodeformationrend->getDrawable()->setNodeMask(1);
		}
	}

	void mPostPartRender::AppendMesh0(MDataPost::mPostMeshData1 * mesh, QHash<int, float> &values, QHash<int, QVector3D> &dis, MViewBasic::NodeOrElement type)
	{
		
		if (mesh == nullptr)
		{
			return;
		}
		if (!mesh->getMeshVisual())
		{
			return;
		}
		int meshID = mesh->getMeshID();
		float value;
		if (mesh->getMeshType() == MeshPoint)
		{
			for (int j = 0; j < 1; ++j)
			{
				int nodeID = mesh->getNodeIndex().at(j);
				QVector3D vertex0 = _oneFrameData->getNodeDataByID(nodeID)->getNodeVertex();
				_pointrend->_vertex0->push_back(vertex0);
				_pointrend->_vertex1->push_back(_partData->getPartColor());
				_pointrend->_vertex2->push_back(1);
				if (type == MViewBasic::PostElement)
				{
					//单元
					value = values.value(meshID);
					if (values.contains(meshID))
					{
						_pointrend->_vertex5->append(QVector<float>{1});
						_minValue = min(_minValue, value);
						_maxValue = max(_maxValue, value);

					}
					else
					{
						_pointrend->_vertex5->append(QVector<float>{0});
					}
				}
				else if (type == MViewBasic::PostNode)
				{
					//节点
					value = values.value(nodeID);
					if (values.contains(nodeID))
					{
						_pointrend->_vertex5->append(QVector<float>{1});
						_minValue = min(_minValue, value);
						_maxValue = max(_maxValue, value);

					}
					else
					{
						_pointrend->_vertex5->append(QVector<float>{0});
					}
				}
				_pointrend->_vertex3->push_back(value);
				_pointrend->_vertex4->push_back(dis.value(nodeID));
			}


			
		}
		
	}

	void mPostPartRender::AppendMesh1(MDataPost::mPostMeshData1 * mesh, QHash<int, float> &values, QHash<int, QVector3D> &dis, MViewBasic::NodeOrElement type)
	{
		
		mPostMeshData1 *meshData = mesh;
		if (meshData == nullptr)
		{
			return;
		}
		if (!meshData->getMeshVisual())
		{
			return;
		}
		int meshID = meshData->getMeshID();
		float value;
		if (meshData->getMeshType() == MeshBeam)
		{
			QVector<int> index = meshData->getNodeIndex();
			bool hasValue{ true };
			for (int j = 0; j < 2; ++j)
			{
				int nodeID = index.at(j);
				QVector3D vertex0 = _oneFrameData->getNodeDataByID(nodeID)->getNodeVertex();
				_linerend->_vertex0->push_back(vertex0);
				_linerend->_vertex1->push_back(_partData->getPartColor());
				_linerend->_vertex2->push_back(1);
				if (type == MViewBasic::PostElement)
				{
					//单元
					value = values.value(meshID);
				}
				else if (type == MViewBasic::PostNode)
				{
					//节点
					value = values.value(nodeID);
				}
				_linerend->_vertex3->push_back(value);
				_linerend->_vertex4->push_back(dis.value(nodeID));
				if (type == MViewBasic::PostNode)
				{
					if (hasValue && values.contains(nodeID))
					{
						_minValue = min(_minValue, value);
						_maxValue = max(_maxValue, value);
					}
					else
					{
						hasValue = false;
					}
				}
			}
			if (type == MViewBasic::PostElement)
			{
				if (values.contains(meshID))
				{
					_linerend->_vertex5->append(QVector<float>{1, 1});
					_minValue = min(_minValue, value);
					_maxValue = max(_maxValue, value);
				}
				else
				{
					_linerend->_vertex5->append(QVector<float>{0, 0});
				}
			}
			else if (type == MViewBasic::PostNode)
			{
				if (hasValue)
				{
					_linerend->_vertex5->append(QVector<float>{ 1, 1 });
				}
				else
				{
					_linerend->_vertex5->append(QVector<float>{ 0, 0 });
				}
			}
		}
		
	}

	QVector<int> quadToTriIndex{ 0, 1, 2, 2, 3, 0 };

	void mPostPartRender::AppendMesh2(MDataPost::mPostMeshData1 * mesh, QHash<int, float> &values, QHash<int, QVector3D> &dis, MViewBasic::NodeOrElement type)
	{
		mPostMeshData1 *meshData = mesh;
		if (meshData == nullptr)
		{
			return;
		}
		if (!meshData->getMeshVisual())
		{
			return;
		}
		int meshID = meshData->getMeshID();
		float value;
		QVector3D vertex0, vertex1, vertex2, vertex3;
		bool hasValue{ true };
		if (meshData->getMeshType() == MeshTri)
		{
			QVector<int> index = meshData->getNodeIndex();

			for (int j = 0; j < 3; ++j)
			{
				int nodeID = index.at(j);
				vertex0 = _oneFrameData->getNodeDataByID(nodeID)->getNodeVertex();
				_facerend->_vertex0->push_back(vertex0);
				_facerend->_vertex1->push_back(_partData->getPartColor());
				_facerend->_vertex2->push_back(1);
				if (type == MViewBasic::PostElement)
				{
					//单元
					value = values.value(meshID);
				}
				else if (type == MViewBasic::PostNode)
				{
					//节点
					value = values.value(nodeID);
				}
				_facerend->_vertex3->push_back(value);
				_facerend->_vertex4->push_back(dis.value(nodeID));
				if (type == MViewBasic::PostNode)
				{
					if (hasValue && values.contains(nodeID))
					{
						_minValue = min(_minValue, value);
						_maxValue = max(_maxValue, value);
					}
					else
					{
						hasValue = false;
					}
				}
			}
			if (type == MViewBasic::PostElement)
			{
				if (values.contains(meshID))
				{
					_facerend->_vertex5->append(QVector<float>{1, 1, 1});
					_minValue = min(_minValue, value);
					_maxValue = max(_maxValue, value);
				}
				else
				{
					_facerend->_vertex5->append(QVector<float>{0, 0, 0});
				}
			}
			else if (type == MViewBasic::PostNode)
			{
				if (hasValue)
				{
					_facerend->_vertex5->append(QVector<float>{ 1, 1, 1 });
				}
				else
				{
					_facerend->_vertex5->append(QVector<float>{ 0, 0, 0 });
				}
			}
			vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + dis.value(index.at(0)) * deformationScale;
			vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + dis.value(index.at(1)) * deformationScale;
			vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + dis.value(index.at(2)) * deformationScale;
			QVector3D normal = QVector3D::crossProduct((vertex1 - vertex0).normalized(), (vertex2 - vertex1).normalized()).normalized();
			_facerend->_vertex6->append(QVector<QVector3D>(3, normal));
			//_facerend->_vertex8->append(QVector<float>(3, (float)meshID));
			_facelinerend->_vertex2->append(QVector<float>(3, 1.0f));
			//_facelinerend->_vertex3->append(QVector<float>(3, (vertex0 - vertex1).length()));
		}
		else if (meshData->getMeshType() == MeshQuad)
		{
			QVector<int> index = meshData->getNodeIndex();

			for (int i : quadToTriIndex)
			{
				int nodeID = index.at(i);
				vertex0 = _oneFrameData->getNodeDataByID(nodeID)->getNodeVertex();
				_facerend->_vertex0->push_back(vertex0);
				_facerend->_vertex1->push_back(_partData->getPartColor());
				_facerend->_vertex2->push_back(1);
				if (type == MViewBasic::PostElement)
				{
					//单元
					value = values.value(meshID);
				}
				else if (type == MViewBasic::PostNode)
				{
					//节点
					value = values.value(nodeID);
				}
				_facerend->_vertex3->push_back(value);
				_facerend->_vertex4->push_back(dis.value(nodeID));
				if (type == MViewBasic::PostNode)
				{
					if (hasValue && values.contains(nodeID))
					{
						_minValue = min(_minValue, value);
						_maxValue = max(_maxValue, value);
					}
					else
					{
						hasValue = false;
					}
				}
			}
			if (type == MViewBasic::PostElement)
			{
				if (values.contains(meshID))
				{
					_facerend->_vertex5->append(QVector<float>(6, 1));
					_minValue = min(_minValue, value);
					_maxValue = max(_maxValue, value);
				}
				else
				{
					_facerend->_vertex5->append(QVector<float>(6, 0));
				}
			}
			else if (type == MViewBasic::PostNode)
			{
				if (hasValue)
				{
					_facerend->_vertex5->append(QVector<float>(6, 1));
				}
				else
				{
					_facerend->_vertex5->append(QVector<float>(6, 0));
				}
			}

			vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + dis.value(index.at(0)) * deformationScale;
			vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + dis.value(index.at(1)) * deformationScale;
			vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + dis.value(index.at(2)) * deformationScale;
			QVector3D normal = QVector3D::crossProduct((vertex1 - vertex0).normalized(), (vertex2 - vertex1).normalized()).normalized();
			_facerend->_vertex6->append(QVector<QVector3D>(6, normal));
			//_facerend->_vertex8->append(QVector<float>(6, (float)meshID));
			_facelinerend->_vertex2->append(QVector<float>(6, 0.0f));
			//_facelinerend->_vertex3->append(QVector<float>(6, (vertex0 - vertex1).length()));
		}

	}

	void mPostPartRender::AppendMesh3(MDataPost::mPostMeshFaceData1 * mesh, QHash<int, float> &values, QHash<int, QVector3D> &dis, MViewBasic::NodeOrElement type)
	{
		//三维网格表面
		mPostMeshFaceData1 * meshFaceData = mesh;
		if (meshFaceData == nullptr)
		{
			return;
		}
		meshFaceData->setVisual(true);
		QVector<int> index = meshFaceData->getNodeIndex();
		int num = index.size();
		int meshID = meshFaceData->getMeshID1();
		float value;
		QVector3D vertex0, vertex1, vertex2, vertex3;
		bool hasValue{ true };
		if (num == 3)
		{
			for (int j = 0; j < 3; ++j)
			{
				int nodeID = index.at(j);
				vertex0 = _oneFrameData->getNodeDataByID(nodeID)->getNodeVertex();
				_facerend->_vertex0->push_back(vertex0);
				_facerend->_vertex1->push_back(_partData->getPartColor());
				_facerend->_vertex2->push_back(1);
				if (type == MViewBasic::PostElement)
				{
					value = values.value(meshID);
				}
				else if (type == MViewBasic::PostNode)
				{
					value = values.value(nodeID);
				}
				_facerend->_vertex3->push_back(value);
				_facerend->_vertex4->push_back(dis.value(nodeID));
				if (type == MViewBasic::PostNode)
				{
					if (hasValue && values.contains(nodeID))
					{
						_minValue = min(_minValue, value);
						_maxValue = max(_maxValue, value);
					}
					else
					{
						hasValue = false;
					}
				}
			}
			if (type == MViewBasic::PostElement)
			{
				if (values.contains(meshID))
				{
					_facerend->_vertex5->append(QVector<float>{1, 1, 1});
					_minValue = min(_minValue, value);
					_maxValue = max(_maxValue, value);
				}
				else
				{
					_facerend->_vertex5->append(QVector<float>{0, 0, 0});
				}
			}
			else if (type == MViewBasic::PostNode)
			{
				if (hasValue)
				{
					_facerend->_vertex5->append(QVector<float>{ 1, 1, 1 });
				}
				else
				{
					_facerend->_vertex5->append(QVector<float>{ 0, 0, 0 });
				}
			}
			vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + dis.value(index.at(0)) * deformationScale;
			vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + dis.value(index.at(1)) * deformationScale;
			vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + dis.value(index.at(2)) * deformationScale;
			QVector3D normal = QVector3D::crossProduct((vertex1 - vertex0).normalized(), (vertex2 - vertex1).normalized()).normalized();
			_facerend->_vertex6->append(QVector<QVector3D>(3, normal));

			//if (!_oneFrameData->getMeshDataByID(meshID)->getMeshVisual())
			//{
			//	meshID = meshFaceData->getMeshID2();
			//}
			//_facerend->_vertex8->append(QVector<float>(3, (float)meshID));
			_facelinerend->_vertex2->append(QVector<float>(3, 1.0f));
			//_facelinerend->_vertex3->append(QVector<float>(3, (vertex0 - vertex1).length()));
		}
		else if (num == 4)
		{
			for (int i : quadToTriIndex)
			{
				int nodeID = index.at(i);
				vertex0 = _oneFrameData->getNodeDataByID(nodeID)->getNodeVertex();
				_facerend->_vertex0->push_back(vertex0);
				_facerend->_vertex1->push_back(_partData->getPartColor());
				_facerend->_vertex2->push_back(1);
				if (type == MViewBasic::PostElement)
				{
					//单元
					value = values.value(meshID);
				}
				else if (type == MViewBasic::PostNode)
				{
					//节点
					value = values.value(nodeID);
				}
				_facerend->_vertex3->push_back(value);
				_facerend->_vertex4->push_back(dis.value(nodeID));
				if (type == MViewBasic::PostNode)
				{
					if (hasValue && values.contains(nodeID))
					{
						_minValue = min(_minValue, value);
						_maxValue = max(_maxValue, value);
					}
					else
					{
						hasValue = false;
					}
				}
			}
			if (type == MViewBasic::PostElement)
			{
				if (values.contains(meshID))
				{
					_facerend->_vertex5->append(QVector<float>(6, 1));
					_minValue = min(_minValue, value);
					_maxValue = max(_maxValue, value);
				}
				else
				{
					_facerend->_vertex5->append(QVector<float>(6, 0));
				}
			}
			else if (type == MViewBasic::PostNode)
			{
				if (hasValue)
				{
					_facerend->_vertex5->append(QVector<float>(6, 1));
				}
				else
				{
					_facerend->_vertex5->append(QVector<float>(6, 0));
				}
			}

			vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + dis.value(index.at(0)) * deformationScale;
			vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + dis.value(index.at(1)) * deformationScale;
			vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + dis.value(index.at(2)) * deformationScale;
			QVector3D normal = QVector3D::crossProduct((vertex1 - vertex0).normalized(), (vertex2 - vertex1).normalized()).normalized();
			_facerend->_vertex6->append(QVector<QVector3D>(6, normal));
			//_facerend->_vertex8->append(QVector<float>(6, (float)meshID));
			_facelinerend->_vertex2->append(QVector<float>(6, 0.0f));
			//_facelinerend->_vertex3->append(QVector<float>(6, (vertex0 - vertex1).length()));
		}

	}

	void mPostPartRender::AppendMeshLine(MDataPost::mPostMeshLineData1 * mesh, QHash<int, QVector3D> &dis, MViewBasic::NodeOrElement type)
	{
		
		if (mesh == nullptr)
		{
			return;
		}
		mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(mesh->getMeshID());
		if (meshData == nullptr)
		{
			return;
		}
		if (!meshData->getMeshVisual())
		{
			return;
		}
		QVector3D vertex0 = _oneFrameData->getNodeDataByID(mesh->getMeshLineNodeIndex1())->getNodeVertex();
		QVector3D vertex1 = _oneFrameData->getNodeDataByID(mesh->getMeshLineNodeIndex2())->getNodeVertex();
		_edgelinerend->_vertex0->push_back(vertex0); _edgelinerend->_vertex0->push_back(vertex1);
		_edgelinerend->_vertex1->push_back(dis.value(mesh->getMeshLineNodeIndex1()));
		_edgelinerend->_vertex1->push_back(dis.value(mesh->getMeshLineNodeIndex2()));
		_edgelinerend->_vertex2->append(QVector<float>(2, vertex0.distanceToPoint(vertex1)));
		
	}

	void mPostPartRender::GetVariableValue_Mesh0(QVector<float> &meshvalues, QVector<float> &meshhasvalues, MDataPost::mPostMeshData1 * mesh, QHash<int, float> &values, MViewBasic::NodeOrElement type)
	{
		if (mesh == nullptr)
		{
			return;
		}
		if (!mesh->getMeshVisual())
		{
			return;
		}
		int meshID = mesh->getMeshID();
		float value;
		if (mesh->getMeshType() == MeshPoint)
		{
			for (int j = 0; j < 1; ++j)
			{
				int nodeID = mesh->getNodeIndex().at(j);
				if (type == MViewBasic::PostElement)
				{
					//单元
					value = values.value(meshID);
				}
				else if (type == MViewBasic::PostNode)
				{
					//节点
					value = values.value(nodeID);
				}
				meshvalues.append(value);
			}
			if (values.contains(meshID))
			{
				meshhasvalues.append(QVector<float>{1});
				_minValue = min(_minValue, value);
				_maxValue = max(_maxValue, value);

			}
			else
			{
				meshhasvalues.append(QVector<float>{1});
			}
		}

	}

	void mPostPartRender::GetVariableValue_Mesh1(QVector<float> &meshvalues, QVector<float> &meshhasvalues, MDataPost::mPostMeshData1 * mesh, QHash<int, float> &values, MViewBasic::NodeOrElement type)
	{
		mPostMeshData1 *meshData = mesh;
		if (meshData == nullptr)
		{
			return;
		}
		if (!meshData->getMeshVisual())
		{
			return;
		}
		int meshID = meshData->getMeshID();
		float value;
		if (meshData->getMeshType() == MeshBeam)
		{
			QVector<int> index = meshData->getNodeIndex();
			bool hasValue{ true };
			for (int j = 0; j < 2; ++j)
			{
				int nodeID = index.at(j);
				if (type == MViewBasic::PostElement)
				{
					//单元
					value = values.value(meshID);
				}
				else if (type == MViewBasic::PostNode)
				{
					//节点
					value = values.value(nodeID);
				}
				meshvalues.append(value);
				if (type == MViewBasic::PostNode)
				{
					if (hasValue && values.contains(nodeID))
					{
						_minValue = min(_minValue, value);
						_maxValue = max(_maxValue, value);
					}
					else
					{
						hasValue = false;
					}
				}
			}
			if (type == MViewBasic::PostElement)
			{
				if (values.contains(meshID))
				{
					meshhasvalues.append(QVector<float>{1, 1});
					_minValue = min(_minValue, value);
					_maxValue = max(_maxValue, value);
				}
				else
				{
					meshhasvalues.append(QVector<float>{0, 0});
				}
			}
			else if (type == MViewBasic::PostNode)
			{
				if (hasValue)
				{
					meshhasvalues.append(QVector<float>{ 1, 1 });
				}
				else
				{
					meshhasvalues.append(QVector<float>{ 0, 0 });
				}
			}
		}

	}


	void mPostPartRender::GetVariableValue_Mesh2(QVector<float> &meshvalues, QVector<float> &meshhasvalues, MDataPost::mPostMeshData1 * mesh, QHash<int, float> &values, MViewBasic::NodeOrElement type)
	{
		mPostMeshData1 *meshData = mesh;
		if (meshData == nullptr)
		{
			return;
		}
		if (!meshData->getMeshVisual())
		{
			return;
		}
		int meshID = meshData->getMeshID();
		float value;
		bool hasValue{ true };
		if (meshData->getMeshType() == MeshTri)
		{
			QVector<int> index = meshData->getNodeIndex();

			for (int j = 0; j < 3; ++j)
			{
				int nodeID = index.at(j);
				if (type == MViewBasic::PostElement)
				{
					//单元
					value = values.value(meshID);
				}
				else if (type == MViewBasic::PostNode)
				{
					//节点
					value = values.value(nodeID);
				}
				meshvalues.append(value);

				if (type == MViewBasic::PostNode)
				{
					if (hasValue && values.contains(nodeID))
					{
						_minValue = min(_minValue, value);
						_maxValue = max(_maxValue, value);
					}
					else
					{
						hasValue = false;
					}
				}
			}
			if (type == MViewBasic::PostElement)
			{
				if (values.contains(meshID))
				{
					meshhasvalues.append(QVector<float>{1, 1, 1});
					_minValue = min(_minValue, value);
					_maxValue = max(_maxValue, value);
				}
				else
				{
					meshhasvalues.append(QVector<float>{0, 0, 0});
				}
			}
			else if (type == MViewBasic::PostNode)
			{
				if (hasValue)
				{
					meshhasvalues.append(QVector<float>{ 1, 1, 1 });
				}
				else
				{
					meshhasvalues.append(QVector<float>{ 0, 0, 0 });
				}
			}

		}
		else if (meshData->getMeshType() == MeshQuad)
		{
			QVector<int> index = meshData->getNodeIndex();

			for (int i : quadToTriIndex)
			{
				int nodeID = index.at(i);
				if (type == MViewBasic::PostElement)
				{
					//单元
					value = values.value(meshID);
				}
				else if (type == MViewBasic::PostNode)
				{
					//节点
					value = values.value(nodeID);
				}
				meshvalues.append(value);

				if (type == MViewBasic::PostNode)
				{
					if (hasValue && values.contains(nodeID))
					{
						_minValue = min(_minValue, value);
						_maxValue = max(_maxValue, value);
					}
					else
					{
						hasValue = false;
					}
				}
			}
			if (type == MViewBasic::PostElement)
			{
				if (values.contains(meshID))
				{
					meshhasvalues.append(QVector<float>(6, 1));
					_minValue = min(_minValue, value);
					_maxValue = max(_maxValue, value);
				}
				else
				{
					meshhasvalues.append(QVector<float>(6, 0));
				}
			}
			else if (type == MViewBasic::PostNode)
			{
				if (hasValue)
				{
					meshhasvalues.append(QVector<float>(6, 1));
				}
				else
				{
					meshhasvalues.append(QVector<float>(6, 0));
				}
			}
		}
	}




	void mPostPartRender::GetVariableValue_Mesh3(QVector<float> &meshvalues, QVector<float> &meshhasvalues, MDataPost::mPostMeshFaceData1 *mesh, QHash<int, float> &values, MViewBasic::NodeOrElement type)
	{
		//三维网格表面
		mPostMeshFaceData1 * meshFaceData = mesh;
		if (meshFaceData == nullptr)
		{
			return;
		}
		meshFaceData->setVisual(true);
		QVector<int> index = meshFaceData->getNodeIndex();
		int num = index.size();
		int meshID = meshFaceData->getMeshID1();
		float value;
		bool hasValue{ true };
		if (num == 3)
		{
			for (int j = 0; j < 3; ++j)
			{
				int nodeID = index.at(j);

				if (type == MViewBasic::PostElement)
				{
					value = values.value(meshID);
				}
				else if (type == MViewBasic::PostNode)
				{
					value = values.value(nodeID);
				}
				meshvalues.push_back(value);

				if (type == MViewBasic::PostNode)
				{
					if (hasValue && values.contains(nodeID))
					{
						_minValue = min(_minValue, value);
						_maxValue = max(_maxValue, value);
					}
					else
					{
						hasValue = false;
					}
				}
			}
			if (type == MViewBasic::PostElement)
			{
				if (values.contains(meshID))
				{
					meshhasvalues.append(QVector<float>{1, 1, 1});
					_minValue = min(_minValue, value);
					_maxValue = max(_maxValue, value);
				}
				else
				{
					meshhasvalues.append(QVector<float>{0, 0, 0});
				}
			}
			else if (type == MViewBasic::PostNode)
			{
				if (hasValue)
				{
					meshhasvalues.append(QVector<float>{ 1, 1, 1 });
				}
				else
				{
					meshhasvalues.append(QVector<float>{ 0, 0, 0 });
				}
			}


		}
		else if (num == 4)
		{
			for (int i : quadToTriIndex)
			{
				int nodeID = index.at(i);
				if (type == MViewBasic::PostElement)
				{
					//单元
					value = values.value(meshID);
				}
				else if (type == MViewBasic::PostNode)
				{
					//节点
					value = values.value(nodeID);
				}
				meshvalues.append(value);

				if (type == MViewBasic::PostNode)
				{
					if (hasValue && values.contains(nodeID))
					{
						_minValue = min(_minValue, value);
						_maxValue = max(_maxValue, value);
					}
					else
					{
						hasValue = false;
					}
				}
			}
			if (type == MViewBasic::PostElement)
			{
				if (values.contains(meshID))
				{
					meshhasvalues.append(QVector<float>(6, 1));
					_minValue = min(_minValue, value);
					_maxValue = max(_maxValue, value);
				}
				else
				{
					meshhasvalues.append(QVector<float>(6, 0));
				}
			}
			else if (type == MViewBasic::PostNode)
			{
				if (hasValue)
				{
					meshhasvalues.append(QVector<float>(6, 1));
				}
				else
				{
					meshhasvalues.append(QVector<float>(6, 0));
				}
			}

		}
	}

	void mPostPartRender::calculateSpaceTreeThread(const QHash<int, QVector3D> &dis)
	{
		if (_oneFrameData == nullptr)
		{
			return;
		}
		if (_partData == nullptr)
		{
			return;
		}
		Space::AABB aabb;
		QVector<mPostMeshFaceData1*> meshFaceIDs = _partData->getMeshFaceData();
		int sss_i = 0;
		for (mPostMeshFaceData1* s : meshFaceIDs)
		{
			QVector<int> nodes = s->getNodeIndex();
			for (int j = 0; j < nodes.size(); j++)
			{
				aabb.push(_oneFrameData->getNodeDataByID(nodes[j])->getNodeVertex() + dis.value(nodes.at(j)) * deformationScale);			
			}
		}
		QVector<mPostMeshData1*> meshDatas = _partData->getMeshDatas2() + _partData->getMeshDatas1() + _partData->getMeshDatas0();
		for (mPostMeshData1* s: meshDatas)
		{
			QVector<int> nodes = s->getNodeIndex();
			for (int j = 0; j < nodes.size(); j++)
			{
				aabb.push(_oneFrameData->getNodeDataByID(nodes[j])->getNodeVertex() + dis.value(nodes.at(j)) * deformationScale);			
			}
		}
		if (_spaceTree != nullptr)
		{
			Space::destoryTree(_spaceTree);
		}
		_spaceTree = new Space::SpaceTree(aabb);
		Space::initSpaceTree(_spaceTree, 2);
		meshDatas += _partData->getMeshDatas3();
		for (auto item : meshDatas)
		{
			//mPostMeshData1 *mesh = _oneFrameData->getMeshDataByID(item);
			QVector<int> nodes = item->getNodeIndex();
			Space::AABB aabb1;
			for (int i = 0; i < nodes.size(); i++)
			{
				aabb1.push(_oneFrameData->getNodeDataByID(nodes[i])->getNodeVertex() + dis.value(nodes.at(i)) * deformationScale);			
			}
			Space::addMeshAABBToSpace(_spaceTree, item, _oneFrameData, aabb1);
			//Space::addMeshToSpace(partData->spaceTree, fd->getMeshDataByID(item), fd);
		}

		//输出信息
		//mGlobalSignals::getInstance()->outputMessageSig(0, QString("%1部件八叉树生成成功！").arg(QString(_partData->getPartName())));

		//qDebug() << endl;
	}

	Space::SpaceTree* mPostPartRender::getPartSpaceTree()
	{
		return _spaceTree;
	}


	mPostPartFaceRender::mPostPartFaceRender(std::shared_ptr<mxr::Group> parent)
	{
		_drawable = MakeAsset<Drawable>();
		_vertex0 = MakeAsset<Vec3Array>();
		_vertex1 = MakeAsset<Vec3Array>();
		_vertex2 = MakeAsset<FloatArray>();
		_vertex3 = MakeAsset<FloatArray>();
		_vertex4 = MakeAsset<Vec3Array >();
		_vertex5 = MakeAsset<FloatArray>();
		_vertex6 = MakeAsset<Vec3Array >();
		//_vertex8 = MakeAsset<FloatArray>();

		_drawable->setVertexAttribArray(0, _vertex0);
		_drawable->setVertexAttribArray(1, _vertex1);
		_drawable->setVertexAttribArray(2, _vertex2);
		_drawable->setVertexAttribArray(3, _vertex3);
		_drawable->setVertexAttribArray(4, _vertex4);
		_drawable->setVertexAttribArray(5, _vertex5);
		_drawable->setVertexAttribArray(6, _vertex6);
		//_drawable->setVertexAttribArray(8, _vertex8);

		_parent = parent;
		_parent->addChild(_drawable);
	}

	mPostPartFaceRender::~mPostPartFaceRender()
	{
		_parent->removeChild(_drawable);
	}

	void mPostPartFaceRender::setStateSet(std::shared_ptr<mxr::StateSet> state)
	{
		_drawable->setStateSet(state);
	}

	mPostPartFaceTransparentNodeformationRender::mPostPartFaceTransparentNodeformationRender(std::shared_ptr<mxr::Group> parent)
	{
		_drawable = MakeAsset<Drawable>();
		_vertex0 = MakeAsset<Vec3Array>();
		_vertex1 = MakeAsset<Vec3Array>();
		//_vertex8 = MakeAsset<FloatArray>();

		_drawable->setVertexAttribArray(0, _vertex0);
		_drawable->setVertexAttribArray(1, _vertex1);
		//_drawable->setVertexAttribArray(8, _vertex8);

		_parent = parent;
		_parent->addChild(_drawable);
	}

	mPostPartFaceTransparentNodeformationRender::~mPostPartFaceTransparentNodeformationRender()
	{
		_parent->removeChild(_drawable);
	}

	void mPostPartFaceTransparentNodeformationRender::setStateSet(std::shared_ptr<mxr::StateSet> state)
	{
		_drawable->setStateSet(state);
	}

	mPostPartFaceLineRender::mPostPartFaceLineRender(std::shared_ptr<mxr::Group> parent)
	{
		_drawable = MakeAsset<Drawable>();
		_vertex0 = MakeAsset<mxr::Vec3Array>();
		_vertex1 = MakeAsset<mxr::Vec3Array>();
		_vertex2 = MakeAsset<mxr::FloatArray>();
		//_vertex3 = MakeAsset<mxr::FloatArray>();

		_drawable->setVertexAttribArray(0, _vertex0);
		_drawable->setVertexAttribArray(1, _vertex1);
		_drawable->setVertexAttribArray(2, _vertex2);
		//_drawable->setVertexAttribArray(3, _vertex3);

		_parent = parent;
		_parent->addChild(_drawable);
	}

	mPostPartFaceLineRender::~mPostPartFaceLineRender()
	{
		_parent->removeChild(_drawable);
	}

	void mPostPartFaceLineRender::setStateSet(std::shared_ptr<mxr::StateSet> state)
	{
		_drawable->setStateSet(state);
	}

	mPostPartLineRender::mPostPartLineRender(std::shared_ptr<mxr::Group> parent)
	{
		_drawable = MakeAsset<Drawable>();
		_vertex0 = MakeAsset<Vec3Array>();
		_vertex1 = MakeAsset<Vec3Array>();
		_vertex2 = MakeAsset<FloatArray>();
		_vertex3 = MakeAsset<FloatArray>();
		_vertex4 = MakeAsset<Vec3Array >();
		_vertex5 = MakeAsset<FloatArray>();

		_drawable->setVertexAttribArray(0, _vertex0);
		_drawable->setVertexAttribArray(1, _vertex1);
		_drawable->setVertexAttribArray(2, _vertex2);
		_drawable->setVertexAttribArray(3, _vertex3);
		_drawable->setVertexAttribArray(4, _vertex4);
		_drawable->setVertexAttribArray(5, _vertex5);

		_parent = parent;
		_parent->addChild(_drawable);
	}

	mPostPartLineRender::~mPostPartLineRender()
	{
		_parent->removeChild(_drawable);
	}

	void mPostPartLineRender::setStateSet(std::shared_ptr<mxr::StateSet> state)
	{
		_drawable->setStateSet(state);
	}

	mPostPartPointRender::mPostPartPointRender(std::shared_ptr<mxr::Group> parent)
	{
		_drawable = MakeAsset<Drawable>();
		_vertex0 = MakeAsset<Vec3Array>();
		_vertex1 = MakeAsset<Vec3Array>();
		_vertex2 = MakeAsset<FloatArray>();
		_vertex3 = MakeAsset<FloatArray>();
		_vertex4 = MakeAsset<Vec3Array >();
		_vertex5 = MakeAsset<FloatArray>();

		_drawable->setVertexAttribArray(0, _vertex0);
		_drawable->setVertexAttribArray(1, _vertex1);
		_drawable->setVertexAttribArray(2, _vertex2);
		_drawable->setVertexAttribArray(3, _vertex3);
		_drawable->setVertexAttribArray(4, _vertex4);
		_drawable->setVertexAttribArray(5, _vertex5);

		_parent = parent;
		_parent->addChild(_drawable);
	}

	mPostPartPointRender::~mPostPartPointRender()
	{
		_parent->removeChild(_drawable);
	}

	void mPostPartPointRender::setStateSet(std::shared_ptr<mxr::StateSet> state)
	{
		_drawable->setStateSet(state);
	}

	mPostPartEdgeLineRender::mPostPartEdgeLineRender(std::shared_ptr<mxr::Group> parent)
	{
		_drawable = MakeAsset<Drawable>();
		_vertex0 = MakeAsset<mxr::Vec3Array>();
		_vertex1 = MakeAsset<mxr::Vec3Array>();
		_vertex2 = MakeAsset<mxr::FloatArray>();

		_drawable->setVertexAttribArray(0, _vertex0);
		_drawable->setVertexAttribArray(1, _vertex1);
		_drawable->setVertexAttribArray(2, _vertex2);

		_parent = parent;
		_parent->addChild(_drawable);
	}

	mPostPartEdgeLineRender::~mPostPartEdgeLineRender()
	{
		_parent->removeChild(_drawable);
	}

	void mPostPartEdgeLineRender::setStateSet(std::shared_ptr<mxr::StateSet> state)
	{
		_drawable->setStateSet(state);
	}

}