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

	void mPostMeshPickThread::setLocation(QVector3D centerPoint, QVector3D centerDirection, double radius, QVector2D centerScreenPoint, double screenRadius)
	{
		_centerPoint = centerPoint;
		_centerDirection = centerDirection;
		_radius = radius;
		_centerScreenPoint = centerScreenPoint;
		_screenRadius = screenRadius;
	}

	void mPostMeshPickThread::setLocation(QVector<QVector2D> pickQuad)
	{
		if (pickQuad.size() < 1)
		{
			return;
		}
		multiQuad = QVector<QVector2D>{ pickQuad.first(), QVector2D(pickQuad.first().x(), pickQuad.last().y()), pickQuad.last(), QVector2D(pickQuad.last().x(), pickQuad.first().y()) };
		_centerBox = (pickQuad.first() + pickQuad.last()) / 2.0;
		_boxXY_2 = pickQuad.first() - _centerBox; _boxXY_2[0] = qAbs(_boxXY_2[0]); _boxXY_2[1] = qAbs(_boxXY_2[1]);	
		//_centerX = centerX;
		//_centerY = centerY;
		//_boxW = boxW;
		//_boxY = boxY;
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
		Space::AABB aabb(spaceTree->space.maxEdge, spaceTree->space.minEdge);
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

	void mPostMeshPickThread::doAnglePick()
	{
		switch (*_pickFilter)
		{
		case PickFilter::PickNothing:break;
		case PickFilter::PickNodeByLineAngle:SoloPickNodeByLineAngle(); break;
		case PickFilter::PickNodeByFaceAngle:SoloPickNodeByFaceAngle(); break;
		case PickFilter::Pick1DMeshByAngle:SoloPick1DMeshByAngle(); break;
		case PickFilter::PickMeshLineByAngle:SoloPickMeshLineByAngle(); break;
		case PickFilter::Pick2DMeshByAngle:SoloPick2DMeshByAngle(); break;
		case PickFilter::PickMeshFaceByAngle:SoloPickMeshFaceByAngle(); break;
		//case PickAnyPositionValue:SoloPickCuttingPlaneVertex(); break;
		default:break;
		}
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
				QVector<QVector2D> tempQVector2D;
				std::set<float> depthlist;
				QVector<QVector3D> vertexs = _oneFrameData->getMeshVertexs(meshData, dis, deformationScale);
				if (isVertexCuttingByPlane(vertexs))
				{
					continue;
				}
				WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
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
				QVector<QVector2D> tempQVector2D;
				std::set<float> depthlist;
				QVector<QVector3D> vertexs = _oneFrameData->getMeshVertexs(meshData, dis, deformationScale);
				if (isVertexCuttingByPlane(vertexs))
				{
					continue;
				}
				WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
				if (mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, meshData->getMeshType()) && *depthlist.begin() < _meshdepth)
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
					std::set<float> depthlist;
					QVector<QVector3D> vertexs = _oneFrameData->getMeshFaceVertexs(meshFaceData, dis, deformationScale);
					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}
					WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
					if (mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, meshFaceData->getNodeIndex().size() == 3 ? MeshTri:MeshQuad ) && *depthlist.begin() < _meshdepth)
					{
						_meshdepth = *depthlist.begin();
						_pickMeshid = meshID;
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
				QVector<QVector2D> tempQVector2D;
				std::set<float> depthlist;
				QVector<QVector3D> vertexs = _oneFrameData->getMeshVertexs(meshData, dis, deformationScale);
				if (isVertexCuttingByPlane(vertexs))
				{
					continue;
				}
				WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
				if (mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, meshData->getMeshType()) && *depthlist.begin() < _meshdepth)
				{
					_meshdepth = *depthlist.begin();
					_pickMeshid = meshID;
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
					QVector<QVector2D> tempQVector2D;
					std::set<float> depthlist;
					QVector<QVector3D> vertexs = _oneFrameData->getMeshVertexs(meshData, dis, deformationScale);
					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}
					WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
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
					std::set<float> depthlist;
					QVector<QVector3D> vertexs = _oneFrameData->getMeshFaceVertexs(meshFaceData, dis, deformationScale);
					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}
					WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
					if (mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, meshFaceData->getNodeIndex().size() == 3 ? MeshTri : MeshQuad) && *depthlist.begin() < _meshFacedepth)
					{
						_meshFacedepth = *depthlist.begin();
						_pickMeshFaceid = meshFaceData->getMeshFaceID();
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
					QVector<QVector2D> tempQVector2D;
					std::set<float> depthlist;
					QVector<QVector3D> vertexs = _oneFrameData->getMeshVertexs(meshData, dis, deformationScale);
					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}
					WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
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
				QVector<QVector2D> tempQVector2D;
				std::set<float> depthlist;
				QVector<QVector3D> vertexs = _oneFrameData->getMeshVertexs(meshData, dis, deformationScale);
				if (isVertexCuttingByPlane(vertexs))
				{
					continue;
				}
				WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
				if (mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, meshData->getMeshType()) && *depthlist.begin() < _meshdepth)
				{
					_meshdepth = *depthlist.begin();
					_pickMeshid = meshID;
					partName1 = partName;
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
					std::set<float> depthlist;
					QVector<QVector3D> vertexs = _oneFrameData->getMeshFaceVertexs(meshFaceData, dis, deformationScale);
					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}
					WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);

					if (*depthlist.begin() < _meshFacedepth && mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, meshFaceData->getNodeIndex().size() == 3 ? MeshTri : MeshQuad))
					{
						_meshFacedepth = *depthlist.begin();
						_pickMeshFaceid = meshFaceData->getMeshFaceID();
						partName2 = partName;
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
					QVector<QVector2D> tempQVector2D;
					std::set<float> depthlist;
					QVector<QVector3D> vertexs = _oneFrameData->getMeshVertexs(meshData, dis, deformationScale);
					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}
					WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
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
				QVector<QVector2D> tempQVector2D;
				std::set<float> depthlist;
				QVector<QVector3D> vertexs = _oneFrameData->getMeshVertexs(meshData, dis, deformationScale);
				if (isVertexCuttingByPlane(vertexs))
				{
					continue;
				}
				WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
				if (mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, meshData->getMeshType()) && *depthlist.begin() < _meshdepth)
				{
					_meshdepth = *depthlist.begin();
					_pickMeshid = meshID;
					//partName = _partName;
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
					std::set<float> depthlist;
					QVector<QVector3D> vertexs = _oneFrameData->getMeshFaceVertexs(meshFaceData, dis, deformationScale);
					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}
					WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);

					if (*depthlist.begin() < _meshFacedepth && mPickToolClass::IsQuadPointInMesh(_pos, tempQVector2D, meshFaceData->getNodeIndex().size() == 3 ? MeshTri : MeshQuad))
					{
						_meshFacedepth = *depthlist.begin();
						_pickMeshFaceid = meshFaceData->getMeshFaceID();
						//partName = _partName;
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

			QVector3D p1 = ScreenvertexToWorldvertex(QVector3D(_centerBox.x() - _boxXY_2.x(), _centerBox.y() + _boxXY_2.y(), 0.0));
			QVector3D p2 = ScreenvertexToWorldvertex(QVector3D(_centerBox + _boxXY_2, 0.0));

			QVector3D p3 = ScreenvertexToWorldvertex(QVector3D(_centerBox - _boxXY_2, 0.0));
			QVector3D p4 = ScreenvertexToWorldvertex(QVector3D(_centerBox.x() - _boxXY_2.x(), _centerBox.y() + _boxXY_2.y(), 0.0));

			QVector3D p5 = ScreenvertexToWorldvertex(QVector3D(_centerBox - _boxXY_2, 0.0));
			QVector3D p6 = ScreenvertexToWorldvertex(QVector3D(_centerBox - _boxXY_2, 1.0));


			QVector3D cen = ScreenvertexToWorldvertex(QVector3D(_centerBox, 0.5));
			QVector3D o_size = QVector3D(p2.distanceToPoint(p1), p4.distanceToPoint(p3), p6.distanceToPoint(p5));


			Space::OBB obb;
			obb.Set((p2 - p1).normalized(), (p3 - p4).normalized(), (p5 - p6).normalized(), cen, o_size);

			QVector<mPostMeshData1*> meshDataAll;
			QVector<mPostMeshData1*> meshDataContain;
			Space::getOBBToMeshData(spaceTree, obb, meshDataAll, meshDataContain);

			meshDataAll += meshDataContain;
			//for (int i = 0; i < meshDataContain.size(); i++)
			//{
			//	if (meshDataContain[i]->getMeshVisual())
			//	{
			//		QVector<int> nodeIndex = meshDataContain[i]->getNodeIndex();
			//		for (int j = 0; j < nodeIndex.size(); ++j)
			//		{
			//			pickNodeDatas.insert(nodeIndex[j]);
			//		}
			//	}
			//}

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
			QVector<mPostMeshData1*> meshDatas = partData->getMeshDatas0() + partData->getMeshDatas1() + partData->getMeshDatas2() + meshDataAll;
			for (mPostMeshData1 *meshData : meshDatas)
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
					if (mPickToolClass::IsPointInQuad(ap1, _centerBox, _boxXY_2))
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
					QVector<QVector3D> vertexs = _oneFrameData->getMeshVertexs(meshData, dis, deformationScale);
					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0));
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1));
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2 };
					if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, multiQuad, MeshBeam) || mPickToolClass::IsMeshPointInQuad(tempQVector2D, _centerBox.x(), _centerBox.y(), _boxXY_2.x(), _boxXY_2.y()))
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
				if (_pickElementTypeFilter.find(meshData->getElementType()) == _pickElementTypeFilter.end())
				{
					continue;
				}
				QVector<QVector3D> vertexs = _oneFrameData->getMeshVertexs(meshData, dis, deformationScale);
				if (isVertexCuttingByPlane(vertexs))
				{
					continue;
				}
				QVector2D center = WorldvertexToScreenvertex(getCenter(vertexs));
				if (mPickToolClass::IsPointInQuad(center, _centerBox, _boxXY_2))
				{
					pickMeshDatas.insert(meshID);
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

			QVector3D p1 = ScreenvertexToWorldvertex(QVector3D(_centerBox.x() - _boxXY_2.x(), _centerBox.y() + _boxXY_2.y(), 0.0));
			QVector3D p2 = ScreenvertexToWorldvertex(QVector3D(_centerBox + _boxXY_2, 0.0));

			QVector3D p3 = ScreenvertexToWorldvertex(QVector3D(_centerBox - _boxXY_2, 0.0));
			QVector3D p4 = ScreenvertexToWorldvertex(QVector3D(_centerBox.x() - _boxXY_2.x(), _centerBox.y() + _boxXY_2.y(), 0.0));

			QVector3D p5 = ScreenvertexToWorldvertex(QVector3D(_centerBox - _boxXY_2, 0.0));
			QVector3D p6 = ScreenvertexToWorldvertex(QVector3D(_centerBox - _boxXY_2, 1.0));


			QVector3D cen = ScreenvertexToWorldvertex(QVector3D(_centerBox, 0.5));
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
			//set<int> meshIDs = partData->getMeshIDs3();
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
				QVector<QVector3D> vertexs = _oneFrameData->getMeshVertexs(meshData, dis, deformationScale);		
				QVector2D center = WorldvertexToScreenvertex(getCenter(vertexs));
				if (mPickToolClass::IsPointInQuad(center, _centerBox, _boxXY_2))
				{
					pickMeshDatas.insert(meshID);
				}
			}

			//二维网格和0维网格
			QVector<mPostMeshData1*> meshDatas = partData->getMeshDatas2() + partData->getMeshDatas0();
			for (auto meshData : meshDatas)
			{			
				if (meshData == nullptr)
				{
					continue;
				}
				if (!meshData->getMeshVisual())
				{
					continue;
				}
				QVector<QVector3D> vertexs = _oneFrameData->getMeshVertexs(meshData, dis, deformationScale);
				QVector2D center = WorldvertexToScreenvertex(getCenter(vertexs));
				if (mPickToolClass::IsPointInQuad(center, _centerBox, _boxXY_2))
				{
					pickMeshDatas.insert(meshData->getMeshID());
				}
			}

			//一维网格
			meshDatas = partData->getMeshDatas1();
			for (auto meshData : meshDatas)
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
				if (meshData->getMeshType() == MeshBeam)
				{
					QVector<QVector3D> vertexs = _oneFrameData->getMeshVertexs(meshData, dis, deformationScale);

					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}

					QVector2D ap1 = WorldvertexToScreenvertex(vertexs.at(0));
					QVector2D ap2 = WorldvertexToScreenvertex(vertexs.at(1));
					QVector<QVector2D> tempQVector2D = QVector<QVector2D>{ ap1, ap2 };
					if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, multiQuad, MeshBeam) || mPickToolClass::IsMeshPointInQuad(tempQVector2D, _centerBox.x(), _centerBox.y(), _boxXY_2.x(), _boxXY_2.y()))
					{
						pickMeshDatas.insert(meshData->getMeshID());
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
					QVector<QVector3D> vertexs = _oneFrameData->getMeshFaceVertexs(meshFaceData, dis, deformationScale);
					QVector2D center = WorldvertexToScreenvertex(getCenter(vertexs));
					if (mPickToolClass::IsPointInQuad(center, _centerBox, _boxXY_2))
					{
						pickMeshFaceDatas.insert(meshFaceData->getMeshFaceID());
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

	void mPostMeshPickThread::SoloPickNodeByLineAngle()
	{
		QPair<PickObjectType, QPair<QString, int>> pickObjectID = _postMeshPickData->getSoloPickNodeDataByLineAngle();
		if (pickObjectID.first == PickObjectType::Mesh1D)
		{
			int meshID = pickObjectID.second.second;
			if (meshID == 0)
			{
				return;
			}
			//拾取到的节点ID
			std::set<int> pickNodeIDs;
			//判断过的单元ID
			std::set<int> judgeMeshIDs;
			//存储相邻单元ID的队列
			QQueue<int> queue;
			//存储相邻单元的方向的队列
			QQueue<QVector3D> queueDirection;
			//方向
			QVector3D direction, lastDirection;
			//角度值
			float angleValue;

			mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
			if (meshData != nullptr)
			{
				if (meshData->getMeshType() == MeshBeam)
				{
					QVector<int> nodeIDs = meshData->getNodeIndex();
					if (nodeIDs.size() < 2)
					{
						return;
					}
					mPostMeshNodeData1* nodeData1 = _oneFrameData->getNodeDataByID(nodeIDs.at(0));
					mPostMeshNodeData1* nodeData2 = _oneFrameData->getNodeDataByID(nodeIDs.at(1));

					lastDirection = (nodeData2->getNodeVertex() - nodeData1->getNodeVertex()).normalized();
					queue.enqueue(meshID);
					queueDirection.enqueue(lastDirection);
				}
			}

			while (!queue.isEmpty() && !queueDirection.isEmpty())
			{
				meshID = queue.dequeue();
				lastDirection = queueDirection.dequeue();
				if (!judgeMeshIDs.insert(meshID).second)
				{
					continue;
				}

				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData != nullptr && meshData->getMeshVisual())
				{
					if (meshData->getMeshType() == MeshBeam)
					{
						QVector<int> nodeIDs = meshData->getNodeIndex();
						if (nodeIDs.size() < 2)
						{
							continue;
						}
						mPostMeshNodeData1* nodeData1 = _oneFrameData->getNodeDataByID(nodeIDs.at(0));
						mPostMeshNodeData1* nodeData2 = _oneFrameData->getNodeDataByID(nodeIDs.at(1));

						direction = (nodeData2->getNodeVertex() - nodeData1->getNodeVertex()).normalized();

						float x = QVector3D::dotProduct(direction, lastDirection);

						if (x > 1.0)
						{
							angleValue = 0;
						}
						else
						{
							angleValue = fabs(180 * acos(QVector3D::dotProduct(direction, lastDirection)) / 3.1415926);
						}
						if (_pickAngleValue > angleValue || _pickAngleValue > (180 - angleValue))
						{
							set<int> adjacentMeshIDs;
							if (pickNodeIDs.insert(nodeIDs.at(0)).second)
							{
								std::set<int> ids = nodeData1->getNodeInMeshIDs();
								adjacentMeshIDs.insert(ids.begin(), ids.end());
							}
							if (pickNodeIDs.insert(nodeIDs.at(1)).second)
							{
								std::set<int> ids = nodeData2->getNodeInMeshIDs();
								adjacentMeshIDs.insert(ids.begin(), ids.end());
							}
							for (int i : adjacentMeshIDs)
							{
								queueDirection.enqueue(direction);
								queue.enqueue(i);
							}
						}
					}
				}

			}


			_postMeshPickData->setMultiplyPickNodeData(pickNodeIDs);

			return;
		}
		else
		{
			int meshLineID = pickObjectID.second.second;
			if (meshLineID == 0)
			{
				return;
			}
			//拾取到的节点ID
			std::set<int> pickNodeIDs;
			//判断过的单元线ID
			std::set<int> judgeMeshLineIDs;
			//存储相邻单元线ID的队列
			QQueue<int> queue;
			//存储相邻单元的方向的队列
			QQueue<QVector3D> queueDirection;
			//方向
			QVector3D direction, lastDirection;
			//角度值
			float angleValue;

			mPostMeshLineData1 *meshLineData = _oneFrameData->getMeshLineDataByID(meshLineID);
			if (meshLineData != nullptr)
			{
				int nodeID1 = meshLineData->getMeshLineNodeIndex1();
				int nodeID2 = meshLineData->getMeshLineNodeIndex2();
				mPostMeshNodeData1* nodeData1 = _oneFrameData->getNodeDataByID(nodeID1);
				mPostMeshNodeData1* nodeData2 = _oneFrameData->getNodeDataByID(nodeID2);

				lastDirection = (nodeData2->getNodeVertex() - nodeData1->getNodeVertex()).normalized();
				queue.enqueue(meshLineID);
				queueDirection.enqueue(lastDirection);
			}

			while (!queue.isEmpty() && !queueDirection.isEmpty())
			{
				meshLineID = queue.dequeue();
				lastDirection = queueDirection.dequeue();
				if (!judgeMeshLineIDs.insert(meshLineID).second)
				{
					continue;
				}

				mPostMeshLineData1 *meshLineData = _oneFrameData->getMeshLineDataByID(meshLineID);
				if (meshLineData != nullptr)
				{
					mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshLineData->getMeshID());
					if (meshData == nullptr || !meshData->getMeshVisual())
					{
						continue;
					}
					int nodeID1 = meshLineData->getMeshLineNodeIndex1();
					int nodeID2 = meshLineData->getMeshLineNodeIndex2();
					mPostMeshNodeData1* nodeData1 = _oneFrameData->getNodeDataByID(nodeID1);
					mPostMeshNodeData1* nodeData2 = _oneFrameData->getNodeDataByID(nodeID2);

					direction = (nodeData2->getNodeVertex() - nodeData1->getNodeVertex()).normalized();

					float x = QVector3D::dotProduct(direction, lastDirection);

					if (x > 1.0)
					{
						angleValue = 0;
					}
					else
					{
						angleValue = fabs(180 * acos(QVector3D::dotProduct(direction, lastDirection)) / 3.1415926);
					}
					if (_pickAngleValue > angleValue || _pickAngleValue > (180 - angleValue))
					{
						set<int> adjacentMeshLineIDs;
						if (pickNodeIDs.insert(nodeID1).second)
						{
							std::set<int> ids = nodeData1->getNodeInMeshLineIDs();

							adjacentMeshLineIDs.insert(ids.begin(), ids.end());
						}
						if (pickNodeIDs.insert(nodeID2).second)
						{
							std::set<int> ids = nodeData2->getNodeInMeshLineIDs();

							adjacentMeshLineIDs.insert(ids.begin(), ids.end());
						}
						for (int i : adjacentMeshLineIDs)
						{
							queueDirection.enqueue(direction);
							queue.enqueue(i);
						}
					}
				}

			}


			_postMeshPickData->setMultiplyPickNodeData(pickNodeIDs);

			return;
		}
		return;
	}

	void mPostMeshPickThread::SoloPickNodeByFaceAngle()
	{
		const QHash<int, QVector3D> &dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();

		QPair<PickObjectType, QPair<QString, int>> pickObjectID = _postMeshPickData->getSoloPickNodeDataByFaceAngle();
		QString partName = pickObjectID.second.first;
		if (pickObjectID.first == PickObjectType::Mesh2D)
		{
			int meshID = pickObjectID.second.second;
			if (meshID == 0)
			{
				return;
			}
			//拾取到的节点ID
			std::set<int> pickNodeIDs;
			//判断过的单元ID
			std::set<int> judgeMeshIDs;
			//存储相邻单元ID的队列
			QQueue<int> queue;
			//存储相邻单元的方向的队列
			QQueue<QVector3D> queueDirection;
			//方向
			QVector3D direction, lastDirection;
			//角度值
			float angleValue;

			mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
			if (meshData != nullptr)
			{
				if (meshData->getMeshType() == MeshQuad || meshData->getMeshType() == MeshTri)
				{
					QVector<int> nodeIDs = meshData->getNodeIndex();
					if (nodeIDs.size() < 3)
					{
						return;
					}
					mPostMeshNodeData1* nodeData1 = _oneFrameData->getNodeDataByID(nodeIDs.at(0));
					mPostMeshNodeData1* nodeData2 = _oneFrameData->getNodeDataByID(nodeIDs.at(1));
					mPostMeshNodeData1* nodeData3 = _oneFrameData->getNodeDataByID(nodeIDs.at(2));

					QVector<QVector3D> vertexs = {
					nodeData1->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(0)),
					nodeData2->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(1)),
					nodeData3->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(2)) };

					lastDirection = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();
					queue.enqueue(meshID);
					queueDirection.enqueue(lastDirection);
				}
			}

			while (!queue.isEmpty() && !queueDirection.isEmpty())
			{
				meshID = queue.dequeue();
				lastDirection = queueDirection.dequeue();
				if (!judgeMeshIDs.insert(meshID).second)
				{
					continue;
				}

				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
				if (meshData != nullptr && meshData->getMeshVisual())
				{
					if (meshData->getMeshType() == MeshTri)
					{
						QVector<int> nodeIDs = meshData->getNodeIndex();
						if (nodeIDs.size() < 3)
						{
							continue;
						}
						mPostMeshNodeData1* nodeData1 = _oneFrameData->getNodeDataByID(nodeIDs.at(0));
						mPostMeshNodeData1* nodeData2 = _oneFrameData->getNodeDataByID(nodeIDs.at(1));
						mPostMeshNodeData1* nodeData3 = _oneFrameData->getNodeDataByID(nodeIDs.at(2));

						QVector<QVector3D> vertexs = {
						nodeData1->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(0)),
						nodeData2->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(1)),
						nodeData3->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(2)) };

						direction = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();

						float x = QVector3D::dotProduct(direction, lastDirection);

						if (x > 1.0)
						{
							angleValue = 0;
						}
						else
						{
							angleValue = fabs(180 * acos(x) / 3.1415926);
						}
						if (_pickAngleValue > angleValue)
						{
							set<int> adjacentMeshIDs;
							if (!isVertexCuttingByPlane(vertexs.at(0)) && pickNodeIDs.insert(nodeIDs.at(0)).second)
							{
								std::set<int> ids = nodeData1->getNodeInMeshIDs();
								adjacentMeshIDs.insert(ids.begin(), ids.end());
							}
							if (!isVertexCuttingByPlane(vertexs.at(1)) && pickNodeIDs.insert(nodeIDs.at(1)).second)
							{

								std::set<int> ids = nodeData2->getNodeInMeshIDs();
								adjacentMeshIDs.insert(ids.begin(), ids.end());
							}
							if (!isVertexCuttingByPlane(vertexs.at(2)) && pickNodeIDs.insert(nodeIDs.at(2)).second)
							{
								std::set<int> ids = nodeData3->getNodeInMeshIDs();
								adjacentMeshIDs.insert(ids.begin(), ids.end());
							}
							for (int i : adjacentMeshIDs)
							{
								queueDirection.enqueue(direction);
								queue.enqueue(i);
							}
						}
					}
					else if (meshData->getMeshType() == MeshQuad)
					{
						QVector<int> nodeIDs = meshData->getNodeIndex();
						if (nodeIDs.size() < 4)
						{
							continue;
						}
						mPostMeshNodeData1* nodeData1 = _oneFrameData->getNodeDataByID(nodeIDs.at(0));
						mPostMeshNodeData1* nodeData2 = _oneFrameData->getNodeDataByID(nodeIDs.at(1));
						mPostMeshNodeData1* nodeData3 = _oneFrameData->getNodeDataByID(nodeIDs.at(2));
						mPostMeshNodeData1* nodeData4 = _oneFrameData->getNodeDataByID(nodeIDs.at(3));

						QVector<QVector3D> vertexs = {
						nodeData1->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(0)),
						nodeData2->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(1)),
						nodeData3->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(2)),
						nodeData4->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(3)) };

						direction = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();

						float x = QVector3D::dotProduct(direction, lastDirection);

						if (x > 1.0)
						{
							angleValue = 0;
						}
						else
						{
							angleValue = fabs(180 * acos(x) / 3.1415926);
						}
						if (_pickAngleValue > angleValue)
						{
							set<int> adjacentMeshIDs;
							if (!isVertexCuttingByPlane(vertexs.at(0)) && pickNodeIDs.insert(nodeIDs.at(0)).second)
							{
								std::set<int> ids = nodeData1->getNodeInMeshIDs();
								adjacentMeshIDs.insert(ids.begin(), ids.end());
							}
							if (!isVertexCuttingByPlane(vertexs.at(1)) && pickNodeIDs.insert(nodeIDs.at(1)).second)
							{
								std::set<int> ids = nodeData2->getNodeInMeshIDs();
								adjacentMeshIDs.insert(ids.begin(), ids.end());
							}
							if (!isVertexCuttingByPlane(vertexs.at(2)) && pickNodeIDs.insert(nodeIDs.at(2)).second)
							{
								std::set<int> ids = nodeData3->getNodeInMeshIDs();
								adjacentMeshIDs.insert(ids.begin(), ids.end());
							}
							if (!isVertexCuttingByPlane(vertexs.at(3)) && pickNodeIDs.insert(nodeIDs.at(3)).second)
							{
								std::set<int> ids = nodeData4->getNodeInMeshIDs();
								adjacentMeshIDs.insert(ids.begin(), ids.end());
							}
							for (int i : adjacentMeshIDs)
							{
								queueDirection.enqueue(direction);
								queue.enqueue(i);
							}
						}
					}
				}

			}


			_postMeshPickData->setMultiplyPickNodeData(pickNodeIDs);

			return;
		}
		else
		{
			int meshFaceID = pickObjectID.second.second;
			if (meshFaceID == 0)
			{
				return;
			}
			//拾取到的节点ID
			std::set<int> pickNodeIDs;
			//判断过的单元面ID
			std::set<int> judgeMeshFaceIDs;
			//判断过的单元ID
			std::set<int> judgeMeshIDs;
			//存储相邻单元线ID的队列
			QQueue<int> queue;
			//存储相邻单元的方向的队列
			QQueue<QVector3D> queueDirection;
			//方向
			QVector3D direction, lastDirection;
			//角度值
			float angleValue;

			mPostMeshFaceData1 *meshFaceData = _oneFrameData->getMeshFaceDataByID(meshFaceID);
			if (meshFaceData != nullptr)
			{
				QVector<int> nodeIDs = meshFaceData->getNodeIndex();
				int meshID = meshFaceData->getMeshID1();
				if (nodeIDs.size() < 3)
				{
					return;
				}
				mPostMeshNodeData1* nodeData1 = _oneFrameData->getNodeDataByID(nodeIDs.at(0));
				mPostMeshNodeData1* nodeData2 = _oneFrameData->getNodeDataByID(nodeIDs.at(1));
				mPostMeshNodeData1* nodeData3 = _oneFrameData->getNodeDataByID(nodeIDs.at(2));

				QVector<QVector3D> vertexs = {
					nodeData1->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(0)),
					nodeData2->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(1)),
					nodeData3->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(2)) };

				lastDirection = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();
				if (!_oneFrameData->getMeshDataByID(meshID)->getMeshVisual())
				{
					lastDirection = -lastDirection;
				}

				queue.enqueue(meshFaceID);
				queueDirection.enqueue(lastDirection);

			}

			while (!queue.isEmpty() && !queueDirection.isEmpty())
			{
				meshFaceID = queue.dequeue();
				lastDirection = queueDirection.dequeue();
				if (!judgeMeshFaceIDs.insert(meshFaceID).second)
				{
					continue;
				}

				mPostMeshFaceData1 *meshFaceData = _oneFrameData->getMeshFaceDataByID(meshFaceID);
				if (meshFaceData != nullptr)
				{
					//获取单元面所在的单元
					int meshID = meshFaceData->getMeshID1();
					QVector<int> nodeIDs = meshFaceData->getNodeIndex(meshID);
					if (nodeIDs.size() < 3)
					{
						continue;
					}
					if (partName != _oneFrameData->getMeshDataByID(meshID)->getPartName())
					{
						meshID = meshFaceData->getMeshID2();
						if (meshID <= 0)
						{
							continue;
						}
					}

					mPostMeshNodeData1* nodeData1 = _oneFrameData->getNodeDataByID(nodeIDs.at(0));
					mPostMeshNodeData1* nodeData2 = _oneFrameData->getNodeDataByID(nodeIDs.at(1));
					mPostMeshNodeData1* nodeData3 = _oneFrameData->getNodeDataByID(nodeIDs.at(2));

					QVector<QVector3D> vertexs = {
					nodeData1->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(0)),
					nodeData2->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(1)),
					nodeData3->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(2)) };

					direction = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();
					if (!_oneFrameData->getMeshDataByID(meshID)->getMeshVisual())
					{
						direction = -direction;
					}
					float x = QVector3D::dotProduct(direction, lastDirection);

					if (x > 1.0)
					{
						angleValue = 0;
					}
					else
					{
						angleValue = fabs(180 * acos(x) / 3.1415926);
					}
					if (_pickAngleValue > angleValue)
					{

						set<int> adjacentMeshIDs;
						if (!isVertexCuttingByPlane(vertexs.at(0)) && pickNodeIDs.insert(nodeIDs.at(0)).second)
						{
							std::set<int> ids = nodeData1->getNodeInMeshIDs();
							adjacentMeshIDs.insert(ids.begin(), ids.end());
						}
						if (!isVertexCuttingByPlane(vertexs.at(1)) && pickNodeIDs.insert(nodeIDs.at(1)).second)
						{
							std::set<int> ids = nodeData2->getNodeInMeshIDs();
							adjacentMeshIDs.insert(ids.begin(), ids.end());
						}
						if (!isVertexCuttingByPlane(vertexs.at(2)) && pickNodeIDs.insert(nodeIDs.at(2)).second)
						{
							std::set<int> ids = nodeData3->getNodeInMeshIDs();
							adjacentMeshIDs.insert(ids.begin(), ids.end());
						}
						if (nodeIDs.size() == 4)
						{
							mPostMeshNodeData1* nodeData4 = _oneFrameData->getNodeDataByID(nodeIDs.at(3));

							if (!isVertexCuttingByPlane(nodeData4->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(3))) && pickNodeIDs.insert(nodeIDs.at(3)).second)
							{
								std::set<int> ids = nodeData4->getNodeInMeshIDs();
								adjacentMeshIDs.insert(ids.begin(), ids.end());
							}
						}
						adjacentMeshIDs.erase(meshID);
						for (int mID : adjacentMeshIDs)
						{
							if (!judgeMeshIDs.insert(mID).second)
							{
								continue;
							}
							mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(mID);
							if (meshData == nullptr || !meshData->getMeshVisual())
							{
								continue;
							}
							QVector<int> meshFaceIDs = meshData->getMeshFaces();
							for (int meshFaceID : meshFaceIDs)
							{
								mPostMeshFaceData1 *meshFaceData = _oneFrameData->getMeshFaceDataByID(meshFaceID);
								if (meshFaceData != nullptr && meshFaceData->getVisual())
								{
									QVector<int> nodeIDs1 = meshFaceData->getNodeIndex();
									bool isIn{ false };
									for (int i = 0; i < 3; i++)
									{
										if (nodeIDs1.contains(nodeIDs[i]))
										{
											queueDirection.enqueue(direction);
											queue.enqueue(meshFaceID);
											isIn = true;
											break;
										}
									}
									if (!isIn && nodeIDs.size() >= 4)
									{
										if (nodeIDs1.contains(nodeIDs[3]))
										{
											queueDirection.enqueue(direction);
											queue.enqueue(meshFaceID);
										}
									}
								}
							}
						}
					}
				}

			}


			_postMeshPickData->setMultiplyPickNodeData(pickNodeIDs);


		}
		return;
	}

	void mPostMeshPickThread::SoloPick1DMeshByAngle()
	{
		const QHash<int, QVector3D> &dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();

		QPair<QString, int> partNameMeshID = _postMeshPickData->getSoloPickMeshDataByAngle();
		mPostMeshPartData1 *partData = _oneFrameData->getMeshPartDataByPartName(partNameMeshID.first);
		int meshID = partNameMeshID.second;
		if (partData == nullptr || meshID == 0)
		{
			return;
		}

		//拾取到的单元ID
		std::set<int> pickMeshIDs;
		//判断过的单元ID
		std::set<int> judgeMeshIDs;
		//存储相邻单元ID的队列
		QQueue<int> queue;
		//存储相邻单元的方向的队列
		QQueue<QVector3D> queueDirection;
		//方向
		QVector3D direction, lastDirection;
		//角度值
		float angleValue;

		mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
		if (meshData != nullptr)
		{
			if (meshData->getMeshType() == MeshBeam)
			{
				QVector<int> nodeIDs = meshData->getNodeIndex();
				if (nodeIDs.size() < 2)
				{
					return;
				}
				mPostMeshNodeData1* nodeData1 = _oneFrameData->getNodeDataByID(nodeIDs.at(0));
				mPostMeshNodeData1* nodeData2 = _oneFrameData->getNodeDataByID(nodeIDs.at(1));

				QVector<QVector3D> vertexs = {
					nodeData1->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(0)),
					nodeData2->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(1)) };

				lastDirection = (vertexs.at(1) - vertexs.at(0)).normalized();
				queue.enqueue(meshID);
				queueDirection.enqueue(lastDirection);
			}
		}

		while (!queue.isEmpty() && !queueDirection.isEmpty())
		{
			meshID = queue.dequeue();
			lastDirection = queueDirection.dequeue();
			if (!judgeMeshIDs.insert(meshID).second)
			{
				continue;
			}

			mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
			if (meshData != nullptr && meshData->getMeshVisual())
			{
				if (meshData->getMeshType() == MeshBeam)
				{
					QVector<int> nodeIDs = meshData->getNodeIndex();
					if (nodeIDs.size() < 2)
					{
						continue;
					}
					mPostMeshNodeData1* nodeData1 = _oneFrameData->getNodeDataByID(nodeIDs.at(0));
					mPostMeshNodeData1* nodeData2 = _oneFrameData->getNodeDataByID(nodeIDs.at(1));

					QVector<QVector3D> vertexs = {
					nodeData1->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(0)),
					nodeData2->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(1)) };

					direction = (vertexs.at(1) - vertexs.at(0)).normalized();

					float x = QVector3D::dotProduct(direction, lastDirection);

					if (x > 1.0)
					{
						angleValue = 0;
					}
					else
					{
						angleValue = fabs(180 * acos(QVector3D::dotProduct(direction, lastDirection)) / 3.1415926);
					}
					if (_pickAngleValue > angleValue || _pickAngleValue > (180 - angleValue))
					{
						if (isVertexCuttingByPlane(vertexs))
						{
							continue;
						}
						if (pickMeshIDs.insert(meshID).second)
						{
							set<int> adjacentMeshIDs;
							set<int> id1 = nodeData1->getNodeInMeshIDs();
							set<int> id2 = nodeData2->getNodeInMeshIDs();
							adjacentMeshIDs.insert(id1.begin(), id1.end());
							adjacentMeshIDs.insert(id2.begin(), id2.end());
							for (int i : adjacentMeshIDs)
							{
								queueDirection.enqueue(direction);
								queue.enqueue(i);
							}
						}
					}
				}
			}

		}


		_postMeshPickData->setMultiplyPickMeshData(pickMeshIDs);

		return;
	}

	void mPostMeshPickThread::SoloPickMeshLineByAngle()
	{
		QPair<QString, int> partNameMeshLineID = _postMeshPickData->getSoloPickMeshLineDataByAngle();
		int meshLineID = partNameMeshLineID.second;
		if (meshLineID == 0)
		{
			return;
		}
		//拾取到的节点ID
		std::set<int> pickMeshLineIDs;
		//判断过的单元线ID
		std::set<int> judgeMeshLineIDs;
		//存储相邻单元线ID的队列
		QQueue<int> queue;
		//存储相邻单元的方向的队列
		QQueue<QVector3D> queueDirection;
		//方向
		QVector3D direction, lastDirection;
		//角度值
		float angleValue;

		mPostMeshLineData1 *meshLineData = _oneFrameData->getMeshLineDataByID(meshLineID);
		if (meshLineData != nullptr)
		{
			int nodeID1 = meshLineData->getMeshLineNodeIndex1();
			int nodeID2 = meshLineData->getMeshLineNodeIndex2();
			mPostMeshNodeData1* nodeData1 = _oneFrameData->getNodeDataByID(nodeID1);
			mPostMeshNodeData1* nodeData2 = _oneFrameData->getNodeDataByID(nodeID2);

			lastDirection = (nodeData2->getNodeVertex() - nodeData1->getNodeVertex()).normalized();
			queue.enqueue(meshLineID);
			queueDirection.enqueue(lastDirection);
		}

		while (!queue.isEmpty() && !queueDirection.isEmpty())
		{
			meshLineID = queue.dequeue();
			lastDirection = queueDirection.dequeue();
			if (!judgeMeshLineIDs.insert(meshLineID).second)
			{
				continue;
			}

			mPostMeshLineData1 *meshLineData = _oneFrameData->getMeshLineDataByID(meshLineID);
			if (meshLineData != nullptr)
			{
				mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshLineData->getMeshID());
				if (meshData == nullptr || !meshData->getMeshVisual())
				{
					continue;
				}
				int nodeID1 = meshLineData->getMeshLineNodeIndex1();
				int nodeID2 = meshLineData->getMeshLineNodeIndex2();
				mPostMeshNodeData1* nodeData1 = _oneFrameData->getNodeDataByID(nodeID1);
				mPostMeshNodeData1* nodeData2 = _oneFrameData->getNodeDataByID(nodeID2);

				direction = (nodeData2->getNodeVertex() - nodeData1->getNodeVertex()).normalized();

				float x = QVector3D::dotProduct(direction, lastDirection);

				if (x > 1.0)
				{
					angleValue = 0;
				}
				else
				{
					angleValue = fabs(180 * acos(QVector3D::dotProduct(direction, lastDirection)) / 3.1415926);
				}
				if (_pickAngleValue > angleValue || _pickAngleValue > (180 - angleValue))
				{
					if (pickMeshLineIDs.insert(meshLineID).second)
					{
						set<int> adjacentMeshLineIDs;
						std::set<int> id1 = nodeData1->getNodeInMeshLineIDs();
						std::set<int> id2 = nodeData2->getNodeInMeshLineIDs();
						adjacentMeshLineIDs.insert(id1.begin(), id1.end());
						adjacentMeshLineIDs.insert(id2.begin(), id2.end());
						for (int i : adjacentMeshLineIDs)
						{
							queueDirection.enqueue(direction);
							queue.enqueue(i);
						}
					}

				}
			}

		}


		//_postMeshPickData->setMultiplyPickMeshLineData(pickMeshLineIDs);

		return;
	}

	void mPostMeshPickThread::SoloPick2DMeshByAngle()
	{
		const QHash<int, QVector3D> &dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();

		QPair<QString, int> partNameMeshID = _postMeshPickData->getSoloPickMeshDataByAngle();
		mPostMeshPartData1 *partData = _oneFrameData->getMeshPartDataByPartName(partNameMeshID.first);
		int meshID = partNameMeshID.second;
		if (partData == nullptr || meshID == 0)
		{
			return;
		}

		//拾取到的单元ID
		std::set<int> pickMeshIDs;
		//判断过的单元ID
		std::set<int> judgeMeshIDs;
		//存储相邻单元ID的队列
		QQueue<int> queue;
		//存储相邻单元的方向的队列
		QQueue<QVector3D> queueDirection;
		//方向
		QVector3D direction, lastDirection;
		//角度值
		float angleValue;

		mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
		if (meshData != nullptr)
		{
			if (meshData->getMeshType() == MeshQuad || meshData->getMeshType() == MeshTri)
			{
				QVector<int> nodeIDs = meshData->getNodeIndex();
				if (nodeIDs.size() < 3)
				{
					return;
				}
				mPostMeshNodeData1* nodeData1 = _oneFrameData->getNodeDataByID(nodeIDs.at(0));
				mPostMeshNodeData1* nodeData2 = _oneFrameData->getNodeDataByID(nodeIDs.at(1));
				mPostMeshNodeData1* nodeData3 = _oneFrameData->getNodeDataByID(nodeIDs.at(2));

				QVector<QVector3D> vertexs = {
					nodeData1->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(0)),
					nodeData2->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(1)),
					nodeData3->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(2)) };

				lastDirection = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();
				queue.enqueue(meshID);
				queueDirection.enqueue(lastDirection);
			}
		}

		while (!queue.isEmpty() && !queueDirection.isEmpty())
		{
			meshID = queue.dequeue();
			lastDirection = queueDirection.dequeue();
			if (!judgeMeshIDs.insert(meshID).second)
			{
				continue;
			}

			mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(meshID);
			if (meshData != nullptr && meshData->getMeshVisual())
			{
				if (meshData->getMeshType() == MeshTri)
				{
					QVector<int> nodeIDs = meshData->getNodeIndex();
					if (nodeIDs.size() < 3)
					{
						continue;
					}
					mPostMeshNodeData1* nodeData1 = _oneFrameData->getNodeDataByID(nodeIDs.at(0));
					mPostMeshNodeData1* nodeData2 = _oneFrameData->getNodeDataByID(nodeIDs.at(1));
					mPostMeshNodeData1* nodeData3 = _oneFrameData->getNodeDataByID(nodeIDs.at(2));

					QVector<QVector3D> vertexs = {
					nodeData1->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(0)),
					nodeData2->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(1)),
					nodeData3->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(2)) };

					direction = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();

					float x = QVector3D::dotProduct(direction, lastDirection);

					if (x > 1.0)
					{
						angleValue = 0;
					}
					else
					{
						angleValue = fabs(180 * acos(x) / 3.1415926);
					}
					if (_pickAngleValue > angleValue)
					{
						if (isVertexCuttingByPlane(vertexs))
						{
							continue;
						}
						if (pickMeshIDs.insert(meshID).second)
						{
							set<int> adjacentMeshIDs;

							set<int> id1 = nodeData1->getNodeInMeshIDs();
							set<int> id2 = nodeData2->getNodeInMeshIDs();
							set<int> id3 = nodeData3->getNodeInMeshIDs();

							adjacentMeshIDs.insert(id1.begin(), id1.end());
							adjacentMeshIDs.insert(id2.begin(), id2.end());
							adjacentMeshIDs.insert(id3.begin(), id3.end());
							for (int i : adjacentMeshIDs)
							{
								queueDirection.enqueue(direction);
								queue.enqueue(i);
							}
						}
					}
				}
				else if (meshData->getMeshType() == MeshQuad)
				{
					QVector<int> nodeIDs = meshData->getNodeIndex();
					if (nodeIDs.size() < 4)
					{
						continue;
					}
					mPostMeshNodeData1* nodeData1 = _oneFrameData->getNodeDataByID(nodeIDs.at(0));
					mPostMeshNodeData1* nodeData2 = _oneFrameData->getNodeDataByID(nodeIDs.at(1));
					mPostMeshNodeData1* nodeData3 = _oneFrameData->getNodeDataByID(nodeIDs.at(2));
					mPostMeshNodeData1* nodeData4 = _oneFrameData->getNodeDataByID(nodeIDs.at(3));

					QVector<QVector3D> vertexs = {
						nodeData1->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(0)),
						nodeData2->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(1)),
						nodeData3->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(2)),
						nodeData4->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(3)) };

					direction = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();

					float x = QVector3D::dotProduct(direction, lastDirection);
					if (x > 1.0)
					{
						angleValue = 0;
					}
					else
					{
						angleValue = fabs(180 * acos(x) / 3.1415926);
					}
					if (_pickAngleValue > angleValue)
					{
						if (isVertexCuttingByPlane(vertexs))
						{
							continue;
						}
						if (pickMeshIDs.insert(meshID).second)
						{
							set<int> adjacentMeshIDs;

							set<int> id1 = nodeData1->getNodeInMeshIDs();
							set<int> id2 = nodeData2->getNodeInMeshIDs();
							set<int> id3 = nodeData3->getNodeInMeshIDs();
							set<int> id4 = nodeData4->getNodeInMeshIDs();

							adjacentMeshIDs.insert(id1.begin(), id1.end());
							adjacentMeshIDs.insert(id2.begin(), id2.end());
							adjacentMeshIDs.insert(id3.begin(), id3.end());
							adjacentMeshIDs.insert(id4.begin(), id4.end());


							for (int i : adjacentMeshIDs)
							{
								queueDirection.enqueue(direction);
								queue.enqueue(i);
							}
						}
					}
				}
			}

		}


		_postMeshPickData->setMultiplyPickMeshData(pickMeshIDs);

		return;
	}

	void mPostMeshPickThread::SoloPickMeshFaceByAngle()
	{
		const QHash<int, QVector3D> &dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();

		QPair<QString, int> partNameMeshID = _postMeshPickData->getSoloPickMeshFaceDataByAngle();
		mPostMeshPartData1 *partData = _oneFrameData->getMeshPartDataByPartName(partNameMeshID.first);
		int meshFaceID = partNameMeshID.second;
		if (partData == nullptr || meshFaceID == 0)
		{
			return;
		}

		//拾取到的单元面ID
		std::set<int> pickMeshFaceIDs;
		//判断过的单元面ID
		std::set<int> judgeMeshFaceIDs;
		//判断过的单元ID
		std::set<int> judgeMeshIDs;
		//存储相邻单元面ID的队列
		QQueue<int> queue;
		//存储相邻单元的方向的队列
		QQueue<QVector3D> queueDirection;
		//方向
		QVector3D direction, lastDirection;
		//角度值
		float angleValue;

		mPostMeshFaceData1 *meshFaceData = _oneFrameData->getMeshFaceDataByID(meshFaceID);
		if (meshFaceData != nullptr)
		{
			QVector<int> nodeIDs = meshFaceData->getNodeIndex();
			int meshID = meshFaceData->getMeshID1();
			if (nodeIDs.size() < 3)
			{
				return;
			}
			mPostMeshNodeData1* nodeData1 = _oneFrameData->getNodeDataByID(nodeIDs.at(0));
			mPostMeshNodeData1* nodeData2 = _oneFrameData->getNodeDataByID(nodeIDs.at(1));
			mPostMeshNodeData1* nodeData3 = _oneFrameData->getNodeDataByID(nodeIDs.at(2));
			QVector<QVector3D> vertexs = {
					nodeData1->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(0)),
					nodeData2->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(1)),
					nodeData3->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(2)) };

			lastDirection = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();
			if (!_oneFrameData->getMeshDataByID(meshID)->getMeshVisual())
			{
				lastDirection = -lastDirection;
			}

			queue.enqueue(meshFaceID);
			queueDirection.enqueue(lastDirection);

		}

		while (!queue.isEmpty() && !queueDirection.isEmpty())
		{
			meshFaceID = queue.dequeue();
			lastDirection = queueDirection.dequeue();
			if (!judgeMeshFaceIDs.insert(meshFaceID).second)
			{
				continue;
			}

			mPostMeshFaceData1 *meshFaceData = _oneFrameData->getMeshFaceDataByID(meshFaceID);
			if (meshFaceData != nullptr)
			{
				//获取单元面所在的单元
				int meshID = meshFaceData->getMeshID1();
				if (partNameMeshID.first != _oneFrameData->getMeshDataByID(meshID)->getPartName())
				{
					meshID = meshFaceData->getMeshID2();
					if (meshID <= 0)
					{
						continue;
					}
				}
				QVector<int> nodeIDs = meshFaceData->getNodeIndex();
				if (nodeIDs.size() < 3)
				{
					continue;
				}
				mPostMeshNodeData1* nodeData1 = _oneFrameData->getNodeDataByID(nodeIDs.at(0));
				mPostMeshNodeData1* nodeData2 = _oneFrameData->getNodeDataByID(nodeIDs.at(1));
				mPostMeshNodeData1* nodeData3 = _oneFrameData->getNodeDataByID(nodeIDs.at(2));

				QVector<QVector3D> vertexs = {
				nodeData1->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(0)),
				nodeData2->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(1)),
				nodeData3->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(2)) };

				direction = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();
				if (!_oneFrameData->getMeshDataByID(meshID)->getMeshVisual())
				{
					direction = -direction;
				}
				float x = QVector3D::dotProduct(direction, lastDirection);

				if (x > 1.0)
				{
					angleValue = 0;
				}
				else
				{
					angleValue = fabs(180 * acos(x) / 3.1415926);
				}
				if (_pickAngleValue > angleValue)
				{
					mPostMeshNodeData1* nodeData4;
					if (nodeIDs.size() == 4)
					{
						nodeData4 = _oneFrameData->getNodeDataByID(nodeIDs.at(3));
						vertexs.append(nodeData4->getNodeVertex() + deformationScale * dis.value(nodeIDs.at(3)));
					}
					if (isVertexCuttingByPlane(vertexs))
					{
						continue;
					}
					if (pickMeshFaceIDs.insert(meshFaceID).second)
					{
						set<int> adjacentMeshIDs;
						set<int> id1 = nodeData1->getNodeInMeshIDs();
						set<int> id2 = nodeData2->getNodeInMeshIDs();
						set<int> id3 = nodeData3->getNodeInMeshIDs();

						adjacentMeshIDs.insert(id1.begin(), id1.end());
						adjacentMeshIDs.insert(id2.begin(), id2.end());
						adjacentMeshIDs.insert(id3.begin(), id3.end());

						if (nodeIDs.size() == 4)
						{
							//mPostMeshNodeData1* nodeData4 = _oneFrameData->getNodeDataByID(nodeIDs.at(3));
							set<int> id4 = nodeData4->getNodeInMeshIDs();
							adjacentMeshIDs.insert(id4.begin(), id4.end());

						}
						adjacentMeshIDs.erase(meshID);
						for (int mID : adjacentMeshIDs)
						{
							if (!judgeMeshIDs.insert(mID).second)
							{
								continue;
							}
							mPostMeshData1 *meshData = _oneFrameData->getMeshDataByID(mID);
							if (meshData == nullptr || !meshData->getMeshVisual())
							{
								continue;
							}
							QVector<int> meshFaceIDs = meshData->getMeshFaces();
							for (int meshFaceID : meshFaceIDs)
							{
								mPostMeshFaceData1 *meshFaceData = _oneFrameData->getMeshFaceDataByID(meshFaceID);
								if (meshFaceData != nullptr && meshFaceData->getVisual())
								{
									QVector<int> nodeIDs1 = meshFaceData->getNodeIndex();
									bool isIn{ false };
									for (int i = 0; i < 3; i++)
									{
										if (nodeIDs1.contains(nodeIDs[i]))
										{
											queueDirection.enqueue(direction);
											queue.enqueue(meshFaceID);
											isIn = true;
											break;
										}
									}
									if (!isIn && nodeIDs.size() >= 4)
									{
										if (nodeIDs1.contains(nodeIDs[3]))
										{
											queueDirection.enqueue(direction);
											queue.enqueue(meshFaceID);
										}
									}
								}
							}

						}
					}
				}

			}

		}


		_postMeshPickData->setMultiplyPickMeshFaceData(pickMeshFaceIDs);

		return;
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
				futures.append(QtConcurrent::run(this, &mPostMeshPickThread::doSoloPick, iter.key(),iter.value()));
			}
			while (!futures.empty())
			{
				futures.back().waitForFinished();
				futures.takeLast();
			}

			_postMeshPickData->setSoloPickData();
			if (*_pickFilter == PickFilter::PickNodeByLineAngle || *_pickFilter == PickFilter::PickNodeByFaceAngle || *_pickFilter == PickFilter::Pick2DMeshByAngle || *_pickFilter == PickFilter::Pick1DMeshByAngle
				|| *_pickFilter == PickFilter::PickMeshLineByAngle || *_pickFilter == PickFilter::PickMeshFaceByAngle)
			{
				//做后续的拾取
				QtConcurrent::run(this, &mPostMeshPickThread::doAnglePick).waitForFinished();
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
				case MultiplyPickMode::QuadPick:futures.append(QtConcurrent::run(this, &mPostMeshPickThread::doQuadPick, iter.key(), iter.value())); break;
				case MultiplyPickMode::RoundPick:futures.append(QtConcurrent::run(this, &mPostMeshPickThread::doRoundPick, iter.key(), iter.value())); break;
				case MultiplyPickMode::PolygonPick:futures.append(QtConcurrent::run(this, &mPostMeshPickThread::doPolygonPick, iter.key(), iter.value())); break;
				default:
					break;
				}

			}
			while (!futures.empty())
			{
				futures.back().waitForFinished();
				futures.takeLast();
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

	void mPostMeshPickThread::WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs, QVector<QVector2D> &Screenvertexs, set<float>& depths)
	{
		for (auto Worldvertex : Worldvertexs)
		{
			float depth;
			Screenvertexs.append(WorldvertexToScreenvertex(Worldvertex, depth));
			depths.insert(depth);
		}
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
	QVector3D mPostMeshPickThread::getCenter(QVector<QVector3D> vertexs)
	{
		QVector3D nodePos;
		for (auto node : vertexs)
		{
			nodePos += node;
		}
		return nodePos / vertexs.size();
	}
}