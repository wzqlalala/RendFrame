#include "mPostMeshPickThread.h"
#include "mPostMeshPickThread.h"
#include "mPostMeshPickData.h"


//MDataPost
#include "mDataPost1.h"
#include "mOneFrameData1.h"
#include "mPostMeshPartData1.h"
#include "mPostMeshData1.h"
#include "mPostMeshFaceData1.h"
#include "mPostMeshLineData1.h"
#include "mPostMeshNodeData1.h"
#include "mPostOneFrameRendData.h"

//MViewBasic
#include "mMeshViewEnum.h"
#include "mPickToolClass.h"

//Qt
#include <QVector2D>
#include <QVector4D>
#include <QString>
#include <QMutex>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <math.h>

using namespace MViewBasic;
using namespace MDataPost;
namespace MDataPost
{
	QMutex postPickMutex;
	mPostMeshPickThread::mPostMeshPickThread(mPostMeshPickData *postMeshPickData)
	{
		_isfinished = false;
		_postMeshPickData = postMeshPickData;
		
	}

	mPostMeshPickThread::~mPostMeshPickThread()
	{
	}

	void mPostMeshPickThread::setPickMode(PickMode pickMode, MultiplyPickMode multiplyPickMode)
	{
		_pickMode = pickMode;
		_multiplyPickMode = multiplyPickMode;
	}

	void mPostMeshPickThread::setPickFilter(MViewBasic::PickFilter * pickFilter)
	{
		_pickFilter = pickFilter;
	}

	void mPostMeshPickThread::appendPartSpaceTree(QString partName, Space::SpaceTree * spaceTree)
	{
		_partSpaceTrees[partName] = spaceTree;
	}

	void mPostMeshPickThread::setCurrentFrameRend(mOneFrameData1 * oneFrameData, mPostOneFrameRendData * postOneFrameRendData)
	{
		_oneFrameData = oneFrameData;
		_oneFrameRendData = postOneFrameRendData;
	}

	void mPostMeshPickThread::setCuttingPlaneNormalVertex(QVector<QPair<QVector3D,QVector3D>> postCuttingNormalVertex)
	{
		_postCuttingNormalVertex = postCuttingNormalVertex;
	}

	void mPostMeshPickThread::setPickElementTypeFilter(std::set<int> pickElementTypeFilter)
	{
		_pickElementTypeFilter = pickElementTypeFilter;
	}

	void mPostMeshPickThread::setMatrix(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model)
	{
		_projection = projection;
		_view = view;
		_model = model;
	}

	void mPostMeshPickThread::setMatrix(QMatrix4x4 pvm)
	{
		_pvm = pvm;
	}

	void mPostMeshPickThread::setLocation(const QPoint& pos, float depth)
	{
		_pos = pos;
		soloQuad = QVector<QVector2D>{ QVector2D(pos.x() + 3,pos.y() + 3),QVector2D(pos.x() + 3,pos.y() - 3),QVector2D(pos.x() - 3,pos.y() - 3),QVector2D(pos.x() - 3,pos.y() + 3) };
		_depth = depth;
		//_pickSoloOrMutiply = MViewBasic::SoloPick;
	}

	void mPostMeshPickThread::setLocation(int centerX, int centerY, int boxW, int boxY, QVector<QVector2D> pickQuad)
	{
		_centerX = centerX;
		_centerY = centerY;
		_boxW = boxW;
		_boxY = boxY;
		multiQuad = pickQuad;
		//_pickSoloOrMutiply = MViewBasic::MultiplyPick;
	}

	void mPostMeshPickThread::setWidget(int w, int h)
	{
		_Win_WIDTH = w;
		_Win_HEIGHT = h;
	}

	bool mPostMeshPickThread::isFinished()
	{
		return _isfinished;
	}

	void mPostMeshPickThread::setFinished()
	{
		_isfinished = false;
	}

	void mPostMeshPickThread::doSoloPick(QString partName, Space::SpaceTree * spaceTree)
	{
		//判断该部件是否存在碰撞
		//判断点选是否在部件的包围盒内
		QVector3D worldVertex = ScreenvertexToWorldvertex(QVector3D(_pos.x(), _pos.y(), _depth));
		Space::AABB aabb = spaceTree->space;
		if (!qFuzzyCompare(aabb.maxEdge.x(), aabb.minEdge.x()))
		{
			float f = (aabb.maxEdge.x() - aabb.minEdge.x()) * 1e-2;
			aabb.maxEdge[0] += f;
			aabb.minEdge[0] -= f;
		}
		else
		{
			aabb.maxEdge[0] += 0.1;
			aabb.minEdge[0] -= 0.1;
		}
		if (!qFuzzyCompare(aabb.maxEdge.y(), aabb.minEdge.y()))
		{
			float f = (aabb.maxEdge.y() - aabb.minEdge.y()) * 1e-2;
			aabb.maxEdge[1] += f;
			aabb.minEdge[1] -= f;
		}
		else
		{
			aabb.maxEdge[1] += 0.1;
			aabb.minEdge[1] -= 0.1;
		}
		if (!qFuzzyCompare(aabb.maxEdge.z(), aabb.minEdge.z()))
		{
			float f = (aabb.maxEdge.z() - aabb.minEdge.z()) * 1e-2;
			aabb.maxEdge[2] += f;
			aabb.minEdge[2] -= f;
		}
		else
		{
			aabb.maxEdge[2] += 0.1;
			aabb.minEdge[2] -= 0.1;
		}
		if (worldVertex.x() > aabb.maxEdge.x() || worldVertex.x() < aabb.minEdge.x()
			|| worldVertex.y() > aabb.maxEdge.y() || worldVertex.y() < aabb.minEdge.y()
			|| worldVertex.z() > aabb.maxEdge.z() || worldVertex.z() < aabb.minEdge.z())
		{
			return;
		}
		switch (*_pickFilter)
		{
		case PickFilter::PickNothing: break;
			//case PickAny:SoloPickAny(); break;
			//case PickPoint:SoloPickPoint(); break;
		case PickFilter::Pick1DMesh:SoloPick1DMesh(partName); break;
		case PickFilter::Pick2DMesh:SoloPick2DMesh(partName); break;
		case PickFilter::PickNode:
		case PickFilter::PickNodeOrder:SoloPickNode(partName); break;
		case PickFilter::PickAnyMesh:SoloPickAnyMesh(partName); break;
		case PickFilter::PickMeshFace:SoloPickMeshFace(partName); break;
			//case PickMeshPart:SoloPickMeshPart(); break;
			//case PickNodeByPart:SoloPickNodeByPart(); break;
			//case PickAnyMeshByPart:SoloPickAnyMeshByPart(); break;
		case PickFilter::PickNodeByLineAngle:SoloPickNodeByLineAngle(partName); break;
		case PickFilter::PickNodeByFaceAngle:SoloPickNodeByFaceAngle(partName); break;
		case PickFilter::Pick1DMeshByAngle:SoloPick1DMeshByAngle(partName); break;
		case PickFilter::Pick2DMeshByAngle:SoloPick2DMeshByAngle(partName); break;
			//case PickMeshLineByAngle:SoloPickMeshLineByAngle(); break;
		case PickFilter::PickMeshFaceByAngle:SoloPickMeshFaceByAngle(partName); break;
		default:break;
		}
	}

	void mPostMeshPickThread::doQuadPick(QString partName, Space::SpaceTree * spaceTree)
	{
		//判断该部件是否存在碰撞	
		switch (*_pickFilter)
		{
		case PickFilter::PickNothing: break;
			//case PickAny:MultiplyPickAny(); break;
			//case PickPoint:MultiplyPickPoint(); break;
		case PickFilter::Pick1DMesh:MultiplyPick1DMesh(partName, spaceTree); break;
		case PickFilter::Pick2DMesh:MultiplyPick2DMesh(partName, spaceTree); break;
		case PickFilter::PickNode:MultiplyPickNode(partName, spaceTree); break;
		case PickFilter::PickAnyMesh:MultiplyPickAnyMesh(partName, spaceTree); break;
		case PickFilter::PickMeshFace:MultiplyPickMeshFace(partName, spaceTree); break;
			//case PickMeshPart:MultiplyPickMeshPart(); break;
			//case PickNodeByPart:MultiplyPickNodeByPart(); break;
			//case PickAnyMeshByPart:MultiplyPickAnyMeshByPart(); break;
		default:break;
		}
	}

	void mPostMeshPickThread::doRoundPick(QString partName, Space::SpaceTree * spaceTree)
	{

	}

	void mPostMeshPickThread::doPolygonPick(QString partName, Space::SpaceTree * spaceTree)
	{

	}

	void mPostMeshPickThread::SoloPickNode(QString partName)
	{
		int _picknodeid = 0;
		float _nodedepth = 1;
		float depth = 1;

		const QHash<int, QVector3D> &dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();
		//for (QString _partName : _partNames)
		{
			mPostMeshPartData1 *partData = _oneFrameData->getMeshPartDataByPartName(partName);
			if (partData == nullptr || !partData->getPartVisual())
			{
				return;
			}

			//Space::SpaceTree*tree = partData->spaceTree;
			//QVector3D sv = ScreenvertexToWorldvertex(QVector3D(_pos.x(), _pos.y(), pos_depth));
			//QVector<mPostMeshData1*> meshDataAll;
			//Space::SpaceTree* require = nullptr;
			//Space::getPointToSpaceTree(tree, sv, require);
			//if (require != nullptr)
			//{
			//	meshDataAll = require->meshs;
			//}
			//
			//for (mPostMeshData1 *meshData : meshDataAll)
			//{
			//	if (meshData == nullptr)
			//	{
			//		continue;
			//	}
			//	if (!meshData->getMeshVisual())
			//	{
			//		continue;
			//	}
			//	QVector<int> index = meshData->getNodeIndex();
			//	for (int j = 0; j < index.size(); ++j)
			//	{
			//		QVector3D vertex0 = _oneFrameData->getNodeDataByID(index.at(j))->getNodeVertex() + deformationScale * dis.value(index.at(j));
			//		if (isVertexCuttingByPlane(vertex0))
			//		{
			//			continue;
			//		}
			//		QVector2D ap1 = WorldvertexToScreenvertex(vertex0, depth);
			//		if (fabs(ap1.x() - _pos.x()) <= 5 && fabs(ap1.y() - _pos.y()) <= 5 && depth < _nodedepth)
			//		{
			//			_nodedepth = depth;
			//			_picknodeid = index.at(j);
			//		}
			//	}
			//}
			/*
			//三维网格的节点
			set<int> meshIDs = partData->getMeshIDs3();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				for (int j = 0; j < index.size(); ++j)
				{
					QVector3D vertex0 = _oneFrameData->getNodeDataByID(index.at(j))->getNodeVertex() + deformationScale * dis.value(index.at(j));
					if (isVertexCuttingByPlane(vertex0))
					{
						continue;
					}
					QVector2D ap1 = WorldvertexToScreenvertex(vertex0, depth);
					if (fabs(ap1.x() - _pos.x()) <= 5 && fabs(ap1.y() - _pos.y()) <= 5 && depth < _nodedepth)
					{
						_nodedepth = depth;
						_picknodeid = index.at(j);
					}
				}
			}*/
			//获取所有的网格面ID
			QVector<mPostMeshFaceData1*> meshFaceIDs = partData->getMeshFaceData();
			for (mPostMeshFaceData1* meshFaceData : meshFaceIDs)
			{

				if (meshFaceData == nullptr)
				{
					continue;
				}

				if (meshFaceData->getVisual())//判断这个单元面是不是外表面
				{
					//int meshID = _oneFrameData->MeshFaceIsSurface1(meshFaceData);
					QVector<QVector2D> tempQVector2D;
					QVector<int> index = meshFaceData->getNodeIndex();
					for (int j = 0; j < index.size(); ++j)
					{
						QVector3D vertex0 = _oneFrameData->getNodeDataByID(index.at(j))->getNodeVertex() + deformationScale * dis.value(index.at(j));
						if (isVertexCuttingByPlane(vertex0))
						{
							continue;
						}
						QVector2D ap1 = WorldvertexToScreenvertex(vertex0, depth);
						if (fabs(ap1.x() - _pos.x()) <= 5 && fabs(ap1.y() - _pos.y()) <= 5 && depth < _nodedepth)
						{
							_nodedepth = depth;
							_picknodeid = index.at(j);
						}
					}
				}
			}


			//一维网格的节点
			std::set<int> meshIDs = partData->getMeshIDs1();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				for (int j = 0; j < index.size(); ++j)
				{
					QVector3D vertex0 = _oneFrameData->getNodeDataByID(index.at(j))->getNodeVertex() + deformationScale * dis.value(index.at(j));

					if (isVertexCuttingByPlane(vertex0))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertex0, depth);
					if (fabs(ap1.x() - _pos.x()) <= 5 && fabs(ap1.y() - _pos.y()) <= 5 && depth < _nodedepth)
					{
						_nodedepth = depth;
						_picknodeid = index.at(j);
					}
				}
			}

			//二维网格的节点
			meshIDs = partData->getMeshIDs2();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				for (int j = 0; j < index.size(); ++j)
				{
					QVector3D vertex0 = _oneFrameData->getNodeDataByID(index.at(j))->getNodeVertex() + deformationScale * dis.value(index.at(j));

					if (isVertexCuttingByPlane(vertex0))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertex0, depth);
					if (fabs(ap1.x() - _pos.x()) <= 5 && fabs(ap1.y() - _pos.y()) <= 5 && depth < _nodedepth)
					{
						_nodedepth = depth;
						_picknodeid = index.at(j);
					}
				}
			}

			//0维网格的节点
			meshIDs = partData->getMeshIDs0();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				for (int j = 0; j < index.size(); ++j)
				{
					QVector3D vertex0 = _oneFrameData->getNodeDataByID(index.at(j))->getNodeVertex() + deformationScale * dis.value(index.at(j));

					if (isVertexCuttingByPlane(vertex0))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertex0, depth);
					if (fabs(ap1.x() - _pos.x()) <= 5 && fabs(ap1.y() - _pos.y()) <= 5 && depth < _nodedepth)
					{
						_nodedepth = depth;
						_picknodeid = index.at(j);
					}
				}
			}
		}
		if (_picknodeid == 0)
		{
			return;
		}
		postPickMutex.lock();
		_postMeshPickData->setSoloPickNodeData(_picknodeid, _nodedepth);
		postPickMutex.unlock();
		return;
	}

	void mPostMeshPickThread::SoloPick1DMesh(QString partName)
	{
		int _pickMeshid = 0;
		float _meshdepth = 1;
		float depth = 1.0;
		float depth1 = 1.0, depth2 = 1.0, depth3 = 1.0, depth4 = 1.0, depth5 = 1.0, depth6 = 1.0, depth7 = 1.0, depth8 = 1.0;
		const QHash<int, QVector3D> &dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();
		mPostMeshPartData1 *partData = _oneFrameData->getMeshPartDataByPartName(partName);
		if (partData == nullptr || !partData->getPartVisual())
		{
			return;
		}

		//一维网格
		set<int> meshIDs = partData->getMeshIDs1();
		for (int meshID : meshIDs)
		{
			mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
			if (meshData == nullptr)
			{
				continue;
			}
			if (!meshData->getMeshVisual())
			{
				continue;
			}
			QVector<int> index = meshData->getNodeIndex();
			if ((meshData->getMeshType() == MeshBeam) && (_pickElementTypeFilter.find(meshData->getElementType()) != _pickElementTypeFilter.end()))
			{
				QVector<QVector3D> vertexs = {
					_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
					_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)) };

				if (isVertexCuttingByPlane(vertexs))
				{
					continue;
				}

				QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0), depth1);
				QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1), depth2);
				QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2 };
				std::set<float> depthlist{ depth1,depth2 };
				if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, soloQuad, MeshBeam) && *depthlist.begin() < _meshdepth)
				{
					_meshdepth = *depthlist.begin();
					_pickMeshid = meshID;
				}
			}
		}

		if (_pickMeshid == 0)
		{
			return;
		}
		postPickMutex.lock();
		_postMeshPickData->setSoloPickMeshData(_pickMeshid, _meshdepth);
		postPickMutex.unlock();
		return;
	}

	void mPostMeshPickThread::SoloPick2DMesh(QString partName)
	{
		int _pickMeshid = 0;
		float _meshdepth = 1;
		float depth = 1.0;
		const QHash<int, QVector3D> &dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();
		float depth1 = 1.0, depth2 = 1.0, depth3 = 1.0, depth4 = 1.0, depth5 = 1.0, depth6 = 1.0, depth7 = 1.0, depth8 = 1.0;
		//for (QString _partName : _partNames)
		{
			mPostMeshPartData1 *partData = _oneFrameData->getMeshPartDataByPartName(partName);
			if (partData == nullptr || !partData->getPartVisual())
			{
				return;
			}

			//二维网格
			set<int> meshIDs = partData->getMeshIDs2();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				if (_pickElementTypeFilter.find(meshData->getElementType()) == _pickElementTypeFilter.end())
				{
					continue;
				}
				if (meshData->getMeshType() == MeshTri)
				{
					QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0), depth1);
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1), depth2);
					QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2), depth3);
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3 };
					std::set<float> depthlist{ depth1,depth2,depth3 };
					if (mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, MeshTri) && *depthlist.begin() < _meshdepth)
					{
						_meshdepth = *depthlist.begin();
						_pickMeshid = meshID;
					}
				}
				else if (meshData->getMeshType() == MeshQuad)
				{
					QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)),
						_oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0), depth1);
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1), depth2);
					QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2), depth3);
					QVector2D ap4 = WorldvertexToScreenvertex(vertexs.at(3), depth4);
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3, ap4 };
					std::set<float> depthlist{ depth1,depth2,depth3,depth4 };
					if (mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, MeshQuad) && *depthlist.begin() < _meshdepth)
					{
						_meshdepth = *depthlist.begin();
						_pickMeshid = meshID;
					}
				}
			}
		}
		if (_pickMeshid == 0)
		{
			return;
		}
		postPickMutex.lock();
		_postMeshPickData->setSoloPickMeshData(_pickMeshid, _meshdepth);
		postPickMutex.unlock();
		return;
	}

	void mPostMeshPickThread::SoloPickAnyMesh(QString partName)
	{
		int _pickMeshid = 0;
		float _meshdepth = 1;
		float depth = 1.0;
		const QHash<int, QVector3D> &dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();
		float depth1 = 1.0, depth2 = 1.0, depth3 = 1.0, depth4 = 1.0, depth5 = 1.0, depth6 = 1.0, depth7 = 1.0, depth8 = 1.0;
		//for (QString _partName : _partNames)
		{
			mPostMeshPartData1 *partData = _oneFrameData->getMeshPartDataByPartName(partName);
			if (partData == nullptr || !partData->getPartVisual())
			{
				return;
			}
			//if (_partName.contains("pshell"))
			//{
			//	float depth9 = 1.0;
			//	depth9++;
			//	qDebug() << depth9;
			//}
			//三维网格
			set<int> meshIDs;
			//Space::SpaceTree*tree = partData->spaceTree;
			//QVector3D sv = ScreenvertexToWorldvertex(QVector3D(_pos.x(), _pos.y(), pos_depth));

			//QVector<mPostMeshData1*> meshData;

			//Space::SpaceTree* require = nullptr;
			//Space::getPointToSpaceTree(tree, sv, require);
			//if (require != nullptr)
			//{
			//	meshData = require->meshs;
			//}


			//set<mPostMeshFaceData1*> meshFaceIDs;
			//for (int i = 0; i < meshData.size(); i++)
			//{
			//	QVector<mPostMeshFaceData1*> faces = meshData[i]->getFace();
			//	for (int j = 0; j < faces.size(); j++)
			//	{
			//		meshFaceIDs.insert(faces[j]);
			//	}
			//}

			//获取所有的网格面ID
			QVector<mPostMeshFaceData1*> meshFaceIDs = partData->getMeshFaceData();
			for (mPostMeshFaceData1* meshFaceData : meshFaceIDs)
			{

				if (meshFaceData == nullptr)
				{
					continue;
				}

				if (meshFaceData->getVisual())//判断这个单元面是不是外表面
				{
					int meshID = _oneFrameData->MeshFaceIsSurface1(meshFaceData);
					QVector<QVector2D> tempQVector2D;
					QVector<int> index = meshFaceData->getNodeIndex();
					if (index.size() == 3)
					{
						QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)) };

						if (isVertexCuttingByPlane(vertexs))
						{
							continue;
						}
						QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0), depth1);
						QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1), depth2);
						QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2), depth3);
						tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3 };
						std::set<float> depthlist{ depth1,depth2,depth3 };
						if (*depthlist.begin() < _meshdepth && mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, MeshTri))
						{
							_meshdepth = *depthlist.begin();
							_pickMeshid = meshID;
						}
					}
					else if (meshFaceData->getNodeIndex().size() == 4)
					{
						QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)),
						_oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3)) };

						if (isVertexCuttingByPlane(vertexs))
						{
							continue;
						}
						QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0), depth1);
						QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1), depth2);
						QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2), depth3);
						QVector2D ap4 = WorldvertexToScreenvertex(vertexs.at(3), depth4);
						tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3, ap4 };
						std::set<float> depthlist{ depth1,depth2,depth3, depth4 };
						if (*depthlist.begin() < _meshdepth && mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, MeshQuad))
						{
							_meshdepth = *depthlist.begin();
							_pickMeshid = meshID;
						}
					}
				}
			}

			//二维网格
			meshIDs = partData->getMeshIDs2();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				if (meshData->getMeshType() == MeshTri)
				{
					QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0), depth1);
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1), depth2);
					QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2), depth3);
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3 };
					std::set<float> depthlist{ depth1,depth2,depth3 };
					if (mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, MeshTri) && *depthlist.begin() < _meshdepth)
					{
						_meshdepth = *depthlist.begin();
						_pickMeshid = meshID;
					}
				}
				else if (meshData->getMeshType() == MeshQuad)
				{
					QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)),
						_oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0), depth1);
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1), depth2);
					QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2), depth3);
					QVector2D ap4 = WorldvertexToScreenvertex(vertexs.at(3), depth4);
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3, ap4 };
					std::set<float> depthlist{ depth1,depth2,depth3,depth4 };
					if (mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, MeshQuad) && *depthlist.begin() < _meshdepth)
					{
						_meshdepth = *depthlist.begin();
						_pickMeshid = meshID;
					}
				}
			}

			//一维网格
			meshIDs = partData->getMeshIDs1();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				if (meshData->getMeshType() == MeshBeam)
				{
					QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0), depth1);
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1), depth2);
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2 };
					std::set<float> depthlist{ depth1,depth2 };
					if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, soloQuad, MeshBeam) && *depthlist.begin() < _meshdepth)
					{
						_meshdepth = *depthlist.begin();
						_pickMeshid = meshID;
					}
				}
			}

			//0维网格
			meshIDs = partData->getMeshIDs0();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				//0维网格
				if (meshData->getMeshType() == MeshPoint)
				{
					QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0), depth);
					if (fabs(ap1.x() - _pos.x()) <= 3 && fabs(ap1.y() - _pos.y()) <= 3 && depth < _meshdepth)
					{
						_meshdepth = depth;
						_pickMeshid = meshID;
					}
				}
			}
		}
		if (_pickMeshid == 0)
		{
			return;
		}
		postPickMutex.lock();
		_postMeshPickData->setSoloPickMeshData(_pickMeshid, _meshdepth);
		postPickMutex.unlock();
		return;
	}

	void mPostMeshPickThread::SoloPickMeshFace(QString partName)
	{
		int _pickMeshFaceid = 0;
		float _meshFacedepth = 1;
		float depth1 = 1.0, depth2 = 1.0, depth3 = 1.0, depth4 = 1.0, depth5 = 1.0, depth6 = 1.0, depth7 = 1.0, depth8 = 1.0;
		const QHash<int, QVector3D> &dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();
		//for (QString _partName : _partNames)
		{
			mPostMeshPartData1 *partData = _oneFrameData->getMeshPartDataByPartName(partName);
			if (partData == nullptr || !partData->getPartVisual())
			{
				return;
			}

			//获取所有的网格面ID
			QVector<mPostMeshFaceData1*> meshFaceIDs = partData->getMeshFaceData();
			for (mPostMeshFaceData1* meshFaceData : meshFaceIDs)
			{
				if (meshFaceData == nullptr)
				{
					continue;
				}
				if (meshFaceData->getVisual())//判断这个单元面是不是外表面
				{
					QVector<QVector2D> tempQVector2D;
					QVector<int> index = meshFaceData->getNodeIndex();
					if (index.size() == 3)
					{
						QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)) };

						if (isVertexCuttingByPlane(vertexs))
						{
							continue;
						}

						QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0), depth1);
						QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1), depth2);
						QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2), depth3);
						tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3 };
						std::set<float> depthlist{ depth1,depth2,depth3 };
						if (*depthlist.begin() < _meshFacedepth && mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, MeshTri))
						{
							_meshFacedepth = *depthlist.begin();
							_pickMeshFaceid = meshFaceData->getMeshFaceID();
						}
					}
					else if (meshFaceData->getNodeIndex().size() == 4)
					{
						QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)),
						_oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3)) };

						if (isVertexCuttingByPlane(vertexs))
						{
							continue;
						}

						QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0), depth1);
						QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1), depth2);
						QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2), depth3);
						QVector2D ap4 = WorldvertexToScreenvertex(vertexs.at(3), depth4);
						tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3, ap4 };
						std::set<float> depthlist{ depth1,depth2,depth3, depth4 };
						if (*depthlist.begin() < _meshFacedepth && mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, MeshQuad))
						{
							_meshFacedepth = *depthlist.begin();
							_pickMeshFaceid = meshFaceData->getMeshFaceID();
						}
					}
				}
			}
		}
		if (_pickMeshFaceid == 0)
		{
			return;
		}
		postPickMutex.lock();
		_postMeshPickData->setSoloPickMeshFaceData(_pickMeshFaceid, _meshFacedepth);
		postPickMutex.unlock();
		return;
	}

	void mPostMeshPickThread::SoloPickNodeByLineAngle(QString partName)
	{
		int _pickMeshid = 0;
		float _meshdepth = 1;
		float depth = 1.0;
		float depth1 = 1.0, depth2 = 1.0, depth3 = 1.0, depth4 = 1.0, depth5 = 1.0, depth6 = 1.0, depth7 = 1.0, depth8 = 1.0;
		const QHash<int, QVector3D> &dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();
		//for (QString _partName : _partNames)
		{
			mPostMeshPartData1 *partData = _oneFrameData->getMeshPartDataByPartName(partName);
			if (partData == nullptr || !partData->getPartVisual())
			{
				return;
			}

			//一维网格
			set<int> meshIDs = partData->getMeshIDs1();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				if ((meshData->getMeshType() == MeshBeam) && (_pickElementTypeFilter.find(meshData->getElementType()) != _pickElementTypeFilter.end()))
				{
					QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0), depth1);
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1), depth2);
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2 };
					std::set<float> depthlist{ depth1,depth2 };
					if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, soloQuad, MeshBeam) && *depthlist.begin() < _meshdepth)
					{
						_meshdepth = *depthlist.begin();
						_pickMeshid = meshID;
					}
				}
			}
		}
		if (_pickMeshid == 0)
		{
			return;
		}
		postPickMutex.lock();
		//_postMeshPickData->setSolo(_pickMeshid, _partName, _meshdepth);
		postPickMutex.unlock();
	}

	void mPostMeshPickThread::SoloPickNodeByFaceAngle(QString partName)
	{
		int _pickMeshid = 0;
		float _meshdepth = 1;
		QString partName1;
		//表面网格
		int _pickMeshFaceid = 0;
		float _meshFacedepth = 1;
		QString partName2;
		float depth = 1.0;
		float depth1 = 1.0, depth2 = 1.0, depth3 = 1.0, depth4 = 1.0, depth5 = 1.0, depth6 = 1.0, depth7 = 1.0, depth8 = 1.0;
		const QHash<int, QVector3D> &dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();
		//for (QString _partName : _partNames)
		{
			mPostMeshPartData1 *partData = _oneFrameData->getMeshPartDataByPartName(partName);
			if (partData == nullptr || !partData->getPartVisual())
			{
				return;
			}

			//二维网格
			set<int> meshIDs = partData->getMeshIDs2();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				if (meshData->getMeshType() == MeshTri)
				{
					QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0), depth1);
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1), depth2);
					QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2), depth3);
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3 };
					std::set<float> depthlist{ depth1,depth2,depth3 };
					if (mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, MeshTri) && *depthlist.begin() < _meshdepth)
					{
						_meshdepth = *depthlist.begin();
						_pickMeshid = meshID;
						partName1 = partName;
					}
				}
				else if (meshData->getMeshType() == MeshQuad)
				{
					QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)),
						_oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0), depth1);
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1), depth2);
					QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2), depth3);
					QVector2D ap4 = WorldvertexToScreenvertex(vertexs.at(3), depth4);
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3, ap4 };
					std::set<float> depthlist{ depth1,depth2,depth3,depth4 };
					if (mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, MeshQuad) && *depthlist.begin() < _meshdepth)
					{
						_meshdepth = *depthlist.begin();
						_pickMeshid = meshID;
						partName1 = partName;
					}
				}
			}


			//获取所有的网格面ID
			QVector<mPostMeshFaceData1*> meshFaceIDs = partData->getMeshFaceData();
			for (mPostMeshFaceData1* meshFaceData : meshFaceIDs)
			{

				if (meshFaceData == nullptr)
				{
					continue;
				}

				if (meshFaceData->getVisual())//判断这个单元面是不是外表面
				{
					QVector<QVector2D> tempQVector2D;
					QVector<int> index = meshFaceData->getNodeIndex();
					if (index.size() == 3)
					{
						QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)) };

						if (isVertexCuttingByPlane(vertexs))
						{
							continue;
						}
						QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0), depth1);
						QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1), depth2);
						QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2), depth3);
						tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3 };
						std::set<float> depthlist{ depth1,depth2,depth3 };
						if (*depthlist.begin() < _meshFacedepth && mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, MeshTri))
						{
							_meshFacedepth = *depthlist.begin();
							_pickMeshFaceid = meshFaceData->getMeshFaceID();
							partName2 = partName;
						}
					}
					else if (meshFaceData->getNodeIndex().size() == 4)
					{
						QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)),
						_oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3)) };

						if (isVertexCuttingByPlane(vertexs))
						{
							continue;
						}
						QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0), depth1);
						QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1), depth2);
						QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2), depth3);
						QVector2D ap4 = WorldvertexToScreenvertex(vertexs.at(3), depth4);
						tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3, ap4 };
						std::set<float> depthlist{ depth1,depth2,depth3, depth4 };
						if (*depthlist.begin() < _meshFacedepth && mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, MeshQuad))
						{
							_meshFacedepth = *depthlist.begin();
							_pickMeshFaceid = meshFaceData->getMeshFaceID();
							partName2 = partName;
						}
					}
				}
			}
		}
		if (_pickMeshid != 0)
		{
			postPickMutex.lock();
			_postMeshPickData->setSoloPickMeshDataByAngle(_pickMeshid, partName1, _meshdepth);
			postPickMutex.unlock();
		}
		if (_pickMeshFaceid != 0)
		{
			postPickMutex.lock();
			_postMeshPickData->setSoloPickMeshFaceDataByAngle(_pickMeshFaceid, partName2, _meshFacedepth);
			postPickMutex.unlock();
		}
		return;
	}

	void mPostMeshPickThread::SoloPick1DMeshByAngle(QString partName)
	{
		int _pickMeshid = 0;
		float _meshdepth = 1;
		float depth = 1.0;
		float depth1 = 1.0, depth2 = 1.0, depth3 = 1.0, depth4 = 1.0, depth5 = 1.0, depth6 = 1.0, depth7 = 1.0, depth8 = 1.0;
		const QHash<int, QVector3D> &dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();
		//for (QString _partName : _partNames)
		{
			mPostMeshPartData1 *partData = _oneFrameData->getMeshPartDataByPartName(partName);
			if (partData == nullptr || !partData->getPartVisual())
			{
				return;
			}

			//一维网格
			set<int> meshIDs = partData->getMeshIDs1();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				if ((meshData->getMeshType() == MeshBeam) && (_pickElementTypeFilter.find(meshData->getElementType()) != _pickElementTypeFilter.end()))
				{
					QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0), depth1);
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1), depth2);
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2 };
					std::set<float> depthlist{ depth1,depth2 };
					if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, soloQuad, MeshBeam) && *depthlist.begin() < _meshdepth)
					{
						_meshdepth = *depthlist.begin();
						_pickMeshid = meshID;
					}
				}
			}
		}
		if (_pickMeshid == 0)
		{
			return;
		}
		postPickMutex.lock();
		_postMeshPickData->setSoloPickMeshDataByAngle(_pickMeshid, partName,_meshdepth);
		postPickMutex.unlock();
		return;
	}

	void mPostMeshPickThread::SoloPick2DMeshByAngle(QString partName)
	{
		int _pickMeshid = 0;
		float _meshdepth = 1;
		float depth = 1.0;
		float depth1 = 1.0, depth2 = 1.0, depth3 = 1.0, depth4 = 1.0, depth5 = 1.0, depth6 = 1.0, depth7 = 1.0, depth8 = 1.0;
		const QHash<int, QVector3D> &dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();
		//for (QString _partName : _partNames)
		{
			mPostMeshPartData1 *partData = _oneFrameData->getMeshPartDataByPartName(partName);
			if (partData == nullptr || !partData->getPartVisual())
			{
				return;
			}

			//三维网格
			set<int> meshIDs;
			//二维网格
			meshIDs = partData->getMeshIDs2();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				if (meshData->getMeshType() == MeshTri)
				{
					QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0), depth1);
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1), depth2);
					QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2), depth3);
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3 };
					std::set<float> depthlist{ depth1,depth2,depth3 };
					if (mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, MeshTri) && *depthlist.begin() < _meshdepth)
					{
						_meshdepth = *depthlist.begin();
						_pickMeshid = meshID;
						//partName = _partName;
					}
				}
				else if (meshData->getMeshType() == MeshQuad)
				{
					QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)),
						_oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0), depth1);
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1), depth2);
					QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2), depth3);
					QVector2D ap4 = WorldvertexToScreenvertex(vertexs.at(3), depth4);
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3, ap4 };
					std::set<float> depthlist{ depth1,depth2,depth3,depth4 };
					if (mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, MeshQuad) && *depthlist.begin() < _meshdepth)
					{
						_meshdepth = *depthlist.begin();
						_pickMeshid = meshID;
						//partName = _partName;
					}
				}
			}
		}
		if (_pickMeshid == 0)
		{
			return;
		}
		postPickMutex.lock();
		_postMeshPickData->setSoloPickMeshDataByAngle(_pickMeshid, partName, _meshdepth);
		postPickMutex.unlock();
		return;
	}

	void mPostMeshPickThread::SoloPickMeshFaceByAngle(QString partName)
	{
		//表面网格
		int _pickMeshFaceid = 0;
		float _meshFacedepth = 1;
		float depth1 = 1.0, depth2 = 1.0, depth3 = 1.0, depth4 = 1.0;
		const QHash<int, QVector3D> &dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();
		//for (QString _partName : _partNames)
		{
			mPostMeshPartData1 *partData = _oneFrameData->getMeshPartDataByPartName(partName);
			if (partData == nullptr || !partData->getPartVisual())
			{
				return;
			}

			//获取所有的网格面ID
			QVector<mPostMeshFaceData1*> meshFaceIDs = partData->getMeshFaceData();
			for (mPostMeshFaceData1* meshFaceData : meshFaceIDs)
			{

				if (meshFaceData == nullptr)
				{
					continue;
				}

				if (meshFaceData->getVisual())//判断这个单元面是不是外表面
				{
					QVector<QVector2D> tempQVector2D;
					QVector<int> index = meshFaceData->getNodeIndex();
					if (index.size() == 3)
					{
						QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)) };

						if (isVertexCuttingByPlane(vertexs))
						{
							continue;
						}
						QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0), depth1);
						QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1), depth2);
						QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2), depth3);
						tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3 };
						std::set<float> depthlist{ depth1,depth2,depth3 };
						if (*depthlist.begin() < _meshFacedepth && mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, MeshTri))
						{
							_meshFacedepth = *depthlist.begin();
							_pickMeshFaceid = meshFaceData->getMeshFaceID();
							//partName = _partName;
						}
					}
					else if (meshFaceData->getNodeIndex().size() == 4)
					{
						QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)),
						_oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3)) };

						if (isVertexCuttingByPlane(vertexs))
						{
							continue;
						}
						QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0), depth1);
						QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1), depth2);
						QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2), depth3);
						QVector2D ap4 = WorldvertexToScreenvertex(vertexs.at(3), depth4);
						tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3, ap4 };
						std::set<float> depthlist{ depth1,depth2,depth3, depth4 };
						if (*depthlist.begin() < _meshFacedepth && mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, MeshQuad))
						{
							_meshFacedepth = *depthlist.begin();
							_pickMeshFaceid = meshFaceData->getMeshFaceID();
							//partName = _partName;
						}
					}
				}
			}
		}
		if (_pickMeshFaceid != 0)
		{
			postPickMutex.lock();
			_postMeshPickData->setSoloPickMeshFaceDataByAngle(_pickMeshFaceid, partName, _meshFacedepth);
			postPickMutex.unlock();
		}

		return;
	}

	void mPostMeshPickThread::MultiplyPickNode(QString partName, Space::SpaceTree* spaceTree)
	{
		std::set<int> pickNodeDatas;
		const QHash<int, QVector3D> &dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();
		//for (QString _partName : _partNames)
		{
			mPostMeshPartData1 *partData = _oneFrameData->getMeshPartDataByPartName(partName);
			if (partData == nullptr || !partData->getPartVisual())
			{
				return;
			}

			QVector3D p1 = ScreenvertexToWorldvertex(QVector3D(_centerX - _boxW / 2.0, _centerY + _boxY / 2.0, 0.0));
			QVector3D p2 = ScreenvertexToWorldvertex(QVector3D(_centerX + _boxW / 2.0, _centerY + _boxY / 2.0, 0.0));

			QVector3D p3 = ScreenvertexToWorldvertex(QVector3D(_centerX - _boxW / 2.0, _centerY - _boxY / 2.0, 0.0));
			QVector3D p4 = ScreenvertexToWorldvertex(QVector3D(_centerX - _boxW / 2.0, _centerY + _boxY / 2.0, 0.0));

			QVector3D p5 = ScreenvertexToWorldvertex(QVector3D(_centerX - _boxW / 2.0, _centerY - _boxY / 2.0, 0.0));
			QVector3D p6 = ScreenvertexToWorldvertex(QVector3D(_centerX - _boxW / 2.0, _centerY - _boxY / 2.0, 1.0));


			QVector3D cen = ScreenvertexToWorldvertex(QVector3D(_centerX, _centerY, 0.5));
			QVector3D o_size = QVector3D(p2.distanceToPoint(p1), p4.distanceToPoint(p3), p6.distanceToPoint(p5));


			Space::OBB obb;
			obb.Set((p2 - p1).normalized(), (p3 - p4).normalized(), (p5 - p6).normalized(), cen, o_size);


			//Space::SpaceTree* spaceTree /*= partData->spaceTree*/;
			QVector<mPostMeshData1*> meshDataAll;
			QVector<mPostMeshData1*> meshDataContain;
			Space::getOBBToMeshData(spaceTree, obb, meshDataAll, meshDataContain);
			for (int i = 0; i < meshDataContain.size(); i++)
			{
				if (meshDataContain[i]->getMeshVisual())
				{
					QVector<int> nodeIndex = meshDataContain[i]->getNodeIndex();
					for (int j = 0; j < nodeIndex.size(); ++j)
					{
						pickNodeDatas.insert(nodeIndex[j]);
					}
				}
			}

			//测试裁剪
			if (_postCuttingNormalVertex.size() != 0)
			{
				QVector<int> eraseNodeIDs;
				for (int nodeID : pickNodeDatas)
				{
					QVector3D vertex = _oneFrameData->getNodeDataByID(nodeID)->getNodeVertex() + deformationScale * dis.value(nodeID);
					if (isVertexCuttingByPlane(vertex))
					{
						eraseNodeIDs.append(nodeID);
					}
				}
				for (int nodeID : eraseNodeIDs)
				{
					pickNodeDatas.erase(nodeID);
				}
			}

			//三维网格的节点
			set<int> meshIDs = partData->getMeshIDs3();
			for (mPostMeshData1 *meshData : meshDataAll)
			{

				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				for (int j = 0; j < index.size(); ++j)
				{
					QVector3D vertex0 = _oneFrameData->getNodeDataByID(index.at(j))->getNodeVertex() + deformationScale * dis.value(index.at(j));
					if (isVertexCuttingByPlane(vertex0))
					{
						continue;
					}
					QVector2D ap1 = WorldvertexToScreenvertex(vertex0);
					if (fabs(ap1.x() - _centerX) <= _boxW / 2.0 && fabs(ap1.y() - _centerY) <= _boxY / 2.0)
					{
						pickNodeDatas.insert(index.at(j));
					}
				}
			}

			//一维网格的节点
			meshIDs = partData->getMeshIDs1();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				for (int j = 0; j < index.size(); ++j)
				{
					QVector3D vertex0 = _oneFrameData->getNodeDataByID(index.at(j))->getNodeVertex() + deformationScale * dis.value(index.at(j));

					if (isVertexCuttingByPlane(vertex0))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertex0);
					if (fabs(ap1.x() - _centerX) <= _boxW / 2.0 && fabs(ap1.y() - _centerY) <= _boxY / 2.0)
					{
						pickNodeDatas.insert(index.at(j));
					}
				}
			}

			//二维网格的节点
			meshIDs = partData->getMeshIDs2();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				for (int j = 0; j < index.size(); ++j)
				{
					QVector3D vertex0 = _oneFrameData->getNodeDataByID(index.at(j))->getNodeVertex() + deformationScale * dis.value(index.at(j));

					if (isVertexCuttingByPlane(vertex0))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertex0);
					if (fabs(ap1.x() - _centerX) <= _boxW / 2.0 && fabs(ap1.y() - _centerY) <= _boxY / 2.0)
					{
						pickNodeDatas.insert(index.at(j));
					}
				}
			}

			//0维网格的节点
			meshIDs = partData->getMeshIDs0();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				for (int j = 0; j < index.size(); ++j)
				{
					QVector3D vertex0 = _oneFrameData->getNodeDataByID(index.at(j))->getNodeVertex() + deformationScale * dis.value(index.at(j));

					if (isVertexCuttingByPlane(vertex0))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertex0);
					if (fabs(ap1.x() - _centerX) <= _boxW / 2.0 && fabs(ap1.y() - _centerY) <= _boxY / 2.0)
					{
						pickNodeDatas.insert(index.at(j));
					}
				}
			}
		}
		if (pickNodeDatas.size() == 0)
		{
			return;
		}
		postPickMutex.lock();
		_postMeshPickData->setMultiplyPickNodeData(pickNodeDatas);
		postPickMutex.unlock();
	}

	void mPostMeshPickThread::MultiplyPick1DMesh(QString partName, Space::SpaceTree* spaceTree)
	{
		std::set<int> pickMeshDatas;

		const QHash<int, QVector3D> &dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();
		//for (QString _partName : _partNames)
		{
			mPostMeshPartData1 *partData = _oneFrameData->getMeshPartDataByPartName(partName);
			if (partData == nullptr || !partData->getPartVisual())
			{
				return;
			}

			//一维网格
			set<int> meshIDs = partData->getMeshIDs1();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				if ((meshData->getMeshType() == MeshBeam) && (_pickElementTypeFilter.find(meshData->getElementType()) != _pickElementTypeFilter.end()))
				{
					QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0));
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1));
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2 };
					if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, multiQuad, MeshBeam) || mPickToolClass::IsMeshPointInQuad(tempQVector2D, _centerX, _centerY, _boxW, _boxY))
					{
						pickMeshDatas.insert(meshID);
					}
				}
			}
		}
		if (pickMeshDatas.size() == 0)
		{
			return;
		}
		postPickMutex.lock();
		_postMeshPickData->setMultiplyPickMeshData(pickMeshDatas);
		postPickMutex.unlock();
	}

	void mPostMeshPickThread::MultiplyPick2DMesh(QString partName, Space::SpaceTree* spaceTree)
	{
		std::set<int> pickMeshDatas;

		const QHash<int, QVector3D> &dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();
		//for (QString _partName : _partNames)
		{
			mPostMeshPartData1 *partData = _oneFrameData->getMeshPartDataByPartName(partName);
			if (partData == nullptr || !partData->getPartVisual())
			{
				return;
			}

			set<int> meshIDs = partData->getMeshIDs2();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				if (_pickElementTypeFilter.find(meshData->getElementType()) == _pickElementTypeFilter.end())
				{
					continue;
				}
				if (meshData->getMeshType() == MeshTri)
				{
					QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0));
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1));
					QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2));
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3 };
					if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, multiQuad, MeshTri) || mPickToolClass::IsMeshPointInQuad(tempQVector2D, _centerX, _centerY, _boxW, _boxY))
					{
						pickMeshDatas.insert(meshID);
					}
				}
				else if (meshData->getMeshType() == MeshQuad)
				{
					QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)),
						_oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0));
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1));
					QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2));
					QVector2D ap4 = WorldvertexToScreenvertex(vertexs.at(3));
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3, ap4 };
					if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, multiQuad, MeshQuad) || mPickToolClass::IsMeshPointInQuad(tempQVector2D, _centerX, _centerY, _boxW, _boxY))
					{
						pickMeshDatas.insert(meshID);
					}
				}
			}
		}
		if (pickMeshDatas.size() == 0)
		{
			return;
		}
		postPickMutex.lock();
		_postMeshPickData->setMultiplyPickMeshData(pickMeshDatas);
		postPickMutex.unlock();

	}

	void mPostMeshPickThread::MultiplyPickAnyMesh(QString partName, Space::SpaceTree* spaceTree)
	{
		std::set<int> pickMeshDatas;
		const QHash<int, QVector3D> &dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();
		//for (QString _partName : _partNames)
		{
			mPostMeshPartData1 *partData = _oneFrameData->getMeshPartDataByPartName(partName);
			if (partData == nullptr || !partData->getPartVisual())
			{
				//continue;
			}

			QVector3D p1 = ScreenvertexToWorldvertex(QVector3D(_centerX - _boxW / 2.0, _centerY + _boxY / 2.0, 0.0));
			QVector3D p2 = ScreenvertexToWorldvertex(QVector3D(_centerX + _boxW / 2.0, _centerY + _boxY / 2.0, 0.0));

			QVector3D p3 = ScreenvertexToWorldvertex(QVector3D(_centerX - _boxW / 2.0, _centerY - _boxY / 2.0, 0.0));
			QVector3D p4 = ScreenvertexToWorldvertex(QVector3D(_centerX - _boxW / 2.0, _centerY + _boxY / 2.0, 0.0));

			QVector3D p5 = ScreenvertexToWorldvertex(QVector3D(_centerX - _boxW / 2.0, _centerY - _boxY / 2.0, 0.0));
			QVector3D p6 = ScreenvertexToWorldvertex(QVector3D(_centerX - _boxW / 2.0, _centerY - _boxY / 2.0, 1.0));


			QVector3D cen = ScreenvertexToWorldvertex(QVector3D(_centerX, _centerY, 0.5));
			QVector3D o_size = QVector3D(p2.distanceToPoint(p1), p4.distanceToPoint(p3), p6.distanceToPoint(p5));


			Space::OBB obb;
			obb.Set((p2 - p1).normalized(), (p3 - p4).normalized(), (p5 - p6).normalized(), cen, o_size);


			//Space::SpaceTree* spaceTree /*= partData->spaceTree*/;
			QVector<mPostMeshData1*> meshDataAll;
			QVector<mPostMeshData1*> meshDataContain;
			Space::getOBBToMeshData(spaceTree, obb, meshDataAll, meshDataContain);

			//测试裁剪
			if (_postCuttingNormalVertex.size() != 0)
			{
				for (mPostMeshData1 *meshData : meshDataContain)
				{
					if (meshData != nullptr && meshData->getMeshVisual())
					{
						QVector<int> index = meshData->getNodeIndex();
						QVector<QVector3D> vertexs;
						for (int nodeID : index)
						{
							vertexs.append(_oneFrameData->getNodeDataByID(nodeID)->getNodeVertex() + deformationScale * dis.value(nodeID));
						}
						if (!isVertexCuttingByPlane(vertexs))
						{
							pickMeshDatas.insert(meshData->getMeshID());
						}
					}
				}
			}
			else
			{
				for (int i = 0; i < meshDataContain.size(); i++)
				{
					if (meshDataContain[i]->getMeshVisual())
					{
						pickMeshDatas.insert(meshDataContain[i]->getMeshID());
					}
				}
			}

			//三维网格
			set<int> meshIDs = partData->getMeshIDs3();
			for (mPostMeshData1 *meshData : meshDataAll)
			{
				int meshID = meshData->getMeshID();
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				if (meshData->getMeshType() == MeshTet)
				{
					QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)),
						_oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0));
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1));
					QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2));
					QVector2D ap4 = WorldvertexToScreenvertex(vertexs.at(3));
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3, ap4 };
					if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, multiQuad, MeshTet) || mPickToolClass::IsMeshPointInQuad(tempQVector2D, _centerX, _centerY, _boxW, _boxY))
					{
						pickMeshDatas.insert(meshID);
					}
				}

				else if (meshData->getMeshType() == MeshPyramid)
				{
					QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)),
						_oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3)),
						_oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0));
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1));
					QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2));
					QVector2D ap4 = WorldvertexToScreenvertex(vertexs.at(3));
					QVector2D ap5 = WorldvertexToScreenvertex(vertexs.at(4));
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3, ap4, ap5 };
					if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, multiQuad, MeshPyramid) || mPickToolClass::IsMeshPointInQuad(tempQVector2D, _centerX, _centerY, _boxW, _boxY))
					{
						pickMeshDatas.insert(meshID);
					}
				}

				else if (meshData->getMeshType() == MeshWedge)
				{
					QVector<QVector3D> vertexs = {
						   _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						   _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						   _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)),
						   _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3)),
						   _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4)),
						   _oneFrameData->getNodeDataByID(index.at(5))->getNodeVertex() + deformationScale * dis.value(index.at(5)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0));
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1));
					QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2));
					QVector2D ap4 = WorldvertexToScreenvertex(vertexs.at(3));
					QVector2D ap5 = WorldvertexToScreenvertex(vertexs.at(4));
					QVector2D ap6 = WorldvertexToScreenvertex(vertexs.at(5));
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3, ap4, ap5, ap6 };
					if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, multiQuad, MeshWedge) || mPickToolClass::IsMeshPointInQuad(tempQVector2D, _centerX, _centerY, _boxW, _boxY))
					{
						pickMeshDatas.insert(meshID);
					}

				}

				else if (meshData->getMeshType() == MeshHex)
				{
					QVector<QVector3D> vertexs = {
						   _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						   _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						   _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)),
						   _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3)),
						   _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4)),
						   _oneFrameData->getNodeDataByID(index.at(5))->getNodeVertex() + deformationScale * dis.value(index.at(5)),
						   _oneFrameData->getNodeDataByID(index.at(6))->getNodeVertex() + deformationScale * dis.value(index.at(6)),
						  _oneFrameData->getNodeDataByID(index.at(7))->getNodeVertex() + deformationScale * dis.value(index.at(7)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0));
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1));
					QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2));
					QVector2D ap4 = WorldvertexToScreenvertex(vertexs.at(3));
					QVector2D ap5 = WorldvertexToScreenvertex(vertexs.at(4));
					QVector2D ap6 = WorldvertexToScreenvertex(vertexs.at(5));
					QVector2D ap7 = WorldvertexToScreenvertex(vertexs.at(6));
					QVector2D ap8 = WorldvertexToScreenvertex(vertexs.at(7));
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3, ap4, ap5, ap6 , ap7, ap8 };
					if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, multiQuad, MeshHex) || mPickToolClass::IsMeshPointInQuad(tempQVector2D, _centerX, _centerY, _boxW, _boxY))
					{
						pickMeshDatas.insert(meshID);
					}
				}
			}

			//二维网格	
			meshIDs = partData->getMeshIDs2();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				if (meshData->getMeshType() == MeshTri)
				{
					QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0));
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1));
					QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2));
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3 };
					if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, multiQuad, MeshTri) || mPickToolClass::IsMeshPointInQuad(tempQVector2D, _centerX, _centerY, _boxW, _boxY))
					{
						pickMeshDatas.insert(meshID);
					}
				}
				else if (meshData->getMeshType() == MeshQuad)
				{
					QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)),
						_oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0));
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1));
					QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2));
					QVector2D ap4 = WorldvertexToScreenvertex(vertexs.at(3));
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3, ap4 };
					if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, multiQuad, MeshQuad) || mPickToolClass::IsMeshPointInQuad(tempQVector2D, _centerX, _centerY, _boxW, _boxY))
					{
						pickMeshDatas.insert(meshID);
					}
				}
			}

			//一维网格
			meshIDs = partData->getMeshIDs1();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				if (meshData->getMeshType() == MeshBeam)
				{
					QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0));
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1));
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2 };
					if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, multiQuad, MeshBeam) || mPickToolClass::IsMeshPointInQuad(tempQVector2D, _centerX, _centerY, _boxW, _boxY))
					{
						pickMeshDatas.insert(meshID);
					}
				}
			}

			meshIDs = partData->getMeshIDs0();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<int> index = meshData->getNodeIndex();
				//0维网格
				if (meshData->getMeshType() == MeshPoint)
				{
					QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)) };

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0));
					if (fabs(ap1.x() - _centerX) <= _boxW / 2.0 && fabs(ap1.y() - _centerY) <= _boxY / 2.0)
					{
						pickMeshDatas.insert(meshID);
					}
				}
			}
		}

		if (pickMeshDatas.size() == 0)
		{
			return;
		}
		postPickMutex.lock();
		_postMeshPickData->setMultiplyPickMeshData(pickMeshDatas);
		postPickMutex.unlock();
	}

	void mPostMeshPickThread::MultiplyPickMeshFace(QString partName, Space::SpaceTree* spaceTree)
	{
		std::set<int> pickMeshFaceDatas;
		const QHash<int, QVector3D> &dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();
		//for (QString _partName : _partNames)
		{
			mPostMeshPartData1 *partData = _oneFrameData->getMeshPartDataByPartName(partName);
			if (partData == nullptr || !partData->getPartVisual())
			{
				return;
			}

			//获取所有的网格面ID
			QVector<mPostMeshFaceData1*> meshFaceIDs = partData->getMeshFaceData();
			for (mPostMeshFaceData1* meshFaceData : meshFaceIDs)
			{
				if (meshFaceData == nullptr)
				{
					continue;
				}
				if (meshFaceData->getVisual())//判断这个单元面是不是外表面
				{
					QVector<QVector2D> tempQVector2D;
					QVector<int> index = meshFaceData->getNodeIndex();
					if (index.size() == 3)
					{
						QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)) };

						if (isVertexCuttingByPlane(vertexs))
						{
							continue;
						}

						QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0));
						QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1));
						QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2));
						tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3 };
						if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, multiQuad, MeshTri) || mPickToolClass::IsMeshPointInQuad(tempQVector2D, _centerX, _centerY, _boxW, _boxY))
						{
							pickMeshFaceDatas.insert(meshFaceData->getMeshFaceID());
						}
					}
					else if (index.size() == 4)
					{
						QVector<QVector3D> vertexs = {
						_oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0)),
						_oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1)),
						_oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2)),
						_oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3)) };

						if (isVertexCuttingByPlane(vertexs))
						{
							continue;
						}

						QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0));
						QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1));
						QVector2D ap3 = WorldvertexToScreenvertex(vertexs.at(2));
						QVector2D ap4 = WorldvertexToScreenvertex(vertexs.at(3));
						tempQVector2D = QVector<QVector2D>{ ap1, ap2, ap3, ap4 };
						if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, multiQuad, MeshQuad) || mPickToolClass::IsMeshPointInQuad(tempQVector2D, _centerX, _centerY, _boxW, _boxY))
						{
							pickMeshFaceDatas.insert(meshFaceData->getMeshFaceID());
						}
					}
				}
			}
		}
		if (pickMeshFaceDatas.size() == 0)
		{
			return;
		}
		postPickMutex.lock();
		_postMeshPickData->setMultiplyPickMeshFaceData(pickMeshFaceDatas);
		postPickMutex.unlock();

	}

	bool mPostMeshPickThread::IsSoloPickMeshPart(MDataPost::mPostMeshPartData1 * meshPartData, float &depth)
	{
		return false;
	}

	bool mPostMeshPickThread::isMultiplyPickMeshPart(MDataPost::mPostMeshPartData1 * meshPartData)
	{
		return false;
	}

	void mPostMeshPickThread::startPick()
	{
		_isfinished = false;
		QVector<QFuture<void>> futures;
		if (_pickMode == PickMode::SoloPick)
		{
			QHashIterator<QString, Space::SpaceTree*> iter(_partSpaceTrees);
			while (iter.hasNext())
			{
				iter.next();
				QtConcurrent::run(this, &mPostMeshPickThread::doSoloPick, iter.key(),iter.value());
			}
			while (!futures.empty())
			{
				futures.back().waitForFinished();
				futures.takeLast();
			}

			_postMeshPickData->setSoloPickData();
			if (*_pickFilter == PickFilter::PickNodeByLineAngle || *_pickFilter == PickFilter::PickNodeByFaceAngle)
			{
				//做后续的拾取

			}
		}
		else
		{
			QHashIterator<QString, Space::SpaceTree*> iter(_partSpaceTrees);
			while (iter.hasNext())
			{
				iter.next();
				switch (_multiplyPickMode)
				{
				case MultiplyPickMode::QuadPick:QtConcurrent::run(this, &mPostMeshPickThread::doQuadPick, iter.key(), iter.value()); break;
				case MultiplyPickMode::RoundPick:QtConcurrent::run(this, &mPostMeshPickThread::doRoundPick, iter.key(), iter.value()); break;
				case MultiplyPickMode::PolygonPick:QtConcurrent::run(this, &mPostMeshPickThread::doPolygonPick, iter.key(), iter.value()); break;
				default:
					break;
				}

			}
		}
		

		/*
		if (_pickSoloOrMutiply == MViewBasic::SoloPick)//单选
		{
			switch (_pickFilter)
			{
			case PickNothing: break;
			//case PickAny:SoloPickAny(); break;
			//case PickPoint:SoloPickPoint(); break;
			case Pick1DMesh:SoloPick1DMesh(); break;
			case Pick2DMesh:SoloPick2DMesh(); break;
			case PickNode:
			case PickNodeOrder:SoloPickNode(); break;
			case PickAnyMesh:SoloPickAnyMesh(); break;
			case PickMeshFace:SoloPickMeshFace(); break;
			//case PickMeshPart:SoloPickMeshPart(); break;
			//case PickNodeByPart:SoloPickNodeByPart(); break;
			//case PickAnyMeshByPart:SoloPickAnyMeshByPart(); break;
			case PickNodeByLineAngle:SoloPickNodeByLineAngle(); break;
			case PickNodeByFaceAngle:SoloPickNodeByFaceAngle(); break;
			case Pick1DMeshByAngle:SoloPick1DMeshByAngle(); break;
			case Pick2DMeshByAngle:SoloPick2DMeshByAngle(); break;
			//case PickMeshLineByAngle:SoloPickMeshLineByAngle(); break;
			case PickMeshFaceByAngle:SoloPickMeshFaceByAngle(); break;
			default:break;
			}
		}
		else//框选
		{
			switch (_pickFilter)
			{
			case PickNothing: break;
			//case PickAny:MultiplyPickAny(); break;
			//case PickPoint:MultiplyPickPoint(); break;
			case Pick1DMesh:MultiplyPick1DMesh(); break;
			case Pick2DMesh:MultiplyPick2DMesh(); break;
			case PickNode:MultiplyPickNode(); break;
			case PickAnyMesh:MultiplyPickAnyMesh(); break;
			case PickMeshFace:MultiplyPickMeshFace(); break;
			//case PickMeshPart:MultiplyPickMeshPart(); break;
			//case PickNodeByPart:MultiplyPickNodeByPart(); break;
			//case PickAnyMeshByPart:MultiplyPickAnyMeshByPart(); break;
			default:break;
			}
		}
		*/

	}

	QVector2D mPostMeshPickThread::WorldvertexToScreenvertex(QVector3D Worldvertex)
	{
		QVector4D res = _pvm * QVector4D(Worldvertex, 1.0);
		return QVector2D(((res.x() / res.w()) / 2.0 + 0.5)*_Win_WIDTH, _Win_HEIGHT - (res.y() / res.w() / 2.0 + 0.5)*_Win_HEIGHT);
	}

	QVector3D mPostMeshPickThread::ScreenvertexToWorldvertex(QVector3D vertex)
	{
		QMatrix4x4 i_mvp = (_pvm).inverted();
		float ndc_x = 2.0*vertex.x() / (float)_Win_WIDTH - 1.0f;
		float ndc_y = 1.0f - (2.0f*vertex.y() / (float)_Win_HEIGHT);
		float ndc_z = vertex.z() * 2 - 1.0;

		QVector4D world = i_mvp * QVector4D(ndc_x, ndc_y, ndc_z, 1.0);
		return QVector3D(world.x() / world.w(), world.y() / world.w(), world.z() / world.w());
	}

	QVector2D mPostMeshPickThread::WorldvertexToScreenvertex(QVector3D Worldvertex, float& depth)
	{
		QVector4D res = _pvm * QVector4D(Worldvertex, 1.0);
		depth = res.z();
		return QVector2D(((res.x() / res.w()) / 2.0 + 0.5)*_Win_WIDTH, _Win_HEIGHT - (res.y() / res.w() / 2.0 + 0.5)*_Win_HEIGHT);
	}

	bool mPostMeshPickThread::isVertexCuttingByPlane(QVector3D vertex)
	{
		//测试所有的裁剪平面
		for (QPair<QVector3D, QVector3D> normalVertex: _postCuttingNormalVertex)
		{
			if (vertex.distanceToPlane(normalVertex.second, normalVertex.first)<0)
			{
				return true;
			}
		}

		//没有被裁剪
		return false;
	}
	bool mPostMeshPickThread::isVertexCuttingByPlane(QVector<QVector3D> vertexs)
	{
		if (_postCuttingNormalVertex.size()==0)
		{
			//没有被裁剪
			return false;
		}
		for (QVector3D vertex : vertexs)
		{
			//测试所有的裁剪平面
			for (QPair<QVector3D, QVector3D> normalVertex : _postCuttingNormalVertex)
			{
				//被裁剪
				if (vertex.distanceToPlane(normalVertex.second, normalVertex.first) < 0)
				{
					return true;
				}
			}
		}
		//全部没有被裁剪
		return false;
	}
}