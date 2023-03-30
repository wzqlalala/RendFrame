#include "mPostCuttingPlaneRender.h"

//Qt
#include <QMatrix4x4>

//MDataPost
#include "mOneFrameData1.h"
#include "mPostOneFrameRendData.h"
#include "mPostMeshData1.h"
#include "mPostMeshNodeData1.h"
#include "mPostMeshPartData1.h"

#include "mBasicEnum.h"

#include <algorithm>
#include <math.h>
#include <renderpch.h>

using namespace MBasicFunction;
using namespace mxr;
namespace MPostRend
{
	mPostCuttingPlaneData::mPostCuttingPlaneData(mOneFrameData1 *oneFrameData, mPostOneFrameRendData *currentFrameRendData, QVector3D normal, QVector3D vertex, bool hasVector)
	{
		//后处理模型数据
		_oneFrameData = oneFrameData;
		//后处理变量数据
		_currentFrameRendData = currentFrameRendData;

		_cuttingPlane = QVector4D(normal, -QVector3D::dotProduct(normal, vertex));
		_normal = normal;
		_vertex = vertex;
		_hasVector = hasVector;
		isCutting = true;
		caculateCuttingPlaneData();
	}

	mPostCuttingPlaneData::~mPostCuttingPlaneData()
	{

	}

	void mPostCuttingPlaneData::reverseCuttingPlaneNormal()
	{
		_cuttingPlane = -_cuttingPlane;
	}

	void mPostCuttingPlaneData::setCuttingData(QVector3D normal, QVector3D vertex)
	{
		_cuttingPlane = QVector4D(normal, -QVector3D::dotProduct(normal, vertex));
		_normal = normal;
		_vertex = vertex;
		caculateCuttingPlaneData();
	}

	QVector4D mPostCuttingPlaneData::getCuttingPlane()
	{
		return _cuttingPlane;
	}

	QVector<QVector3D> mPostCuttingPlaneData::getTriVertex()
	{
		return _triPlane;
	}

	QVector<float> mPostCuttingPlaneData::getTriTexture()
	{
		return _triValue;
	}

	QVector<QVector3D> mPostCuttingPlaneData::getTriVectorValue()
	{
		return _triVectorValue;
	}

	void mPostCuttingPlaneData::setPlaneData(QVector3D normal, QVector3D centervertex, float maxR)
	{
		maxR /= 1.732f;
		QVector3D leftUp = QVector3D{ -maxR,maxR,0 };
		QVector3D leftDown = QVector3D{ -maxR,-maxR,0 };
		QVector3D rightDown = QVector3D{ maxR,-maxR,0 };
		QVector3D rightUp = QVector3D{ maxR,maxR,0 };
		centerVertex = centervertex;

		theta = -180 * acos(QVector3D::dotProduct(normal, QVector3D{ 0, 0, 1 }) / (normal.length())) / 3.1415926;
		axis = QVector3D::crossProduct(normal, QVector3D{ 0, 0, 1 });
		_planeVertex = QVector<QVector3D>{ leftUp ,leftDown ,rightDown, rightDown,rightUp,leftUp };

		QMatrix4x4 model;
		model.setToIdentity();
		model.translate(centerVertex);
		model.rotate(theta, axis);
		for (int i = 0; i < _planeVertex.size(); i++)
		{
			QVector4D vertex = model * QVector4D(_planeVertex[i], 1.0);
			_planeVertex[i] = QVector3D(vertex);
		}

	}


	QVector3D mPostCuttingPlaneData::getCuttingPlaneVertex()
	{
		return _vertex;
	}

	QVector3D mPostCuttingPlaneData::getCuttingPlaneNormal()
	{
		return _normal;
	}

	QPair<QVector<QVector3D>, QVector<QVector3D>> mPostCuttingPlaneData::getCuttingNodeData()
	{
		//只判断节点矢量
		QVector3D vertex0, vertex1, vertex2, vertex3, vertex4, vertex5, vertex6, vertex7;
		if (_currentFrameRendData == nullptr)
		{
			return QPair<QVector<QVector3D>, QVector<QVector3D>>();
		}
		if (_oneFrameData == nullptr)
		{
			return QPair<QVector<QVector3D>, QVector<QVector3D>>();
		}

		QVector<QVector3D> resVertexs;
		QVector<QVector3D> resValues;

		QHash<int, QVector3D> vectorValues = _currentFrameRendData->getVectorRendData();
		const QHash<int, QVector3D> &dis = _currentFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _currentFrameRendData->getDeformationScale();

		auto iter = _oneFrameData->getMeshPartIterator();
		while (iter.hasNext())
		{
			iter.next();
			QString partName = iter.key();
			mPostMeshPartData1 *partData = iter.value();
			if (partData == nullptr)
			{
				continue;
			}
			if (!partData->getPartVisual())
			{
				continue;
			}

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

				QVector<QVector3D> sss;
				QVector<QVector3D> isInvectorValue;
				QVector<int> index = meshData->getNodeIndex();
				if (meshData->getMeshType() == MeshTet)
				{
					vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
					vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
					vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
					vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));

					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(1))}, sss, isInvectorValue))
					{
					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex2}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(2))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(2))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex3}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex3}, QVector<QVector3D>{vectorValues.value(index.at(2)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
					{

					}
				}
				else if (meshData->getMeshType() == MeshPyramid)
				{
					vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
					vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
					vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
					vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
					vertex4 = _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));

					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(1))}, sss, isInvectorValue))
					{
					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(2))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex3}, QVector<QVector3D>{vectorValues.value(index.at(2)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex4}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(4))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex4}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(4))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex4}, QVector<QVector3D>{vectorValues.value(index.at(2)), vectorValues.value(index.at(4))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex3, vertex4}, QVector<QVector3D>{vectorValues.value(index.at(3)), vectorValues.value(index.at(4))}, sss, isInvectorValue))
					{

					}
				}
				else if (meshData->getMeshType() == MeshWedge)
				{
					vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
					vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
					vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
					vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
					vertex4 = _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));
					vertex5 = _oneFrameData->getNodeDataByID(index.at(5))->getNodeVertex() + deformationScale * dis.value(index.at(5));

					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(1))}, sss, isInvectorValue))
					{
					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(2))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex0}, QVector<QVector3D>{vectorValues.value(index.at(2)), vectorValues.value(index.at(0))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex4}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(4))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex5}, QVector<QVector3D>{vectorValues.value(index.at(2)), vectorValues.value(index.at(5))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex3, vertex4}, QVector<QVector3D>{vectorValues.value(index.at(3)), vectorValues.value(index.at(4))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex4, vertex5}, QVector<QVector3D>{vectorValues.value(index.at(4)), vectorValues.value(index.at(5))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex5, vertex3}, QVector<QVector3D>{vectorValues.value(index.at(5)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
					{

					}
				}
				else if (meshData->getMeshType() == MeshHex)
				{
					vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
					vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
					vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
					vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
					vertex4 = _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));
					vertex5 = _oneFrameData->getNodeDataByID(index.at(5))->getNodeVertex() + deformationScale * dis.value(index.at(5));
					vertex6 = _oneFrameData->getNodeDataByID(index.at(6))->getNodeVertex() + deformationScale * dis.value(index.at(6));
					vertex7 = _oneFrameData->getNodeDataByID(index.at(7))->getNodeVertex() + deformationScale * dis.value(index.at(7));

					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(1))}, sss, isInvectorValue))
					{
					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex4}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(4))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(2))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex5}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(5))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex3}, QVector<QVector3D>{vectorValues.value(index.at(2)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex6}, QVector<QVector3D>{vectorValues.value(index.at(2)), vectorValues.value(index.at(6))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex3, vertex7}, QVector<QVector3D>{vectorValues.value(index.at(3)), vectorValues.value(index.at(7))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex4, vertex5}, QVector<QVector3D>{vectorValues.value(index.at(4)), vectorValues.value(index.at(5))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex4, vertex7}, QVector<QVector3D>{vectorValues.value(index.at(4)), vectorValues.value(index.at(7))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex5, vertex6}, QVector<QVector3D>{vectorValues.value(index.at(5)), vectorValues.value(index.at(6))}, sss, isInvectorValue))
					{

					}
					if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex6, vertex7}, QVector<QVector3D>{vectorValues.value(index.at(6)), vectorValues.value(index.at(7))}, sss, isInvectorValue))
					{

					}
				}

				resVertexs.append(sss);
				resValues.append(isInvectorValue);
			}
		}
		return QPair<QVector<QVector3D>, QVector<QVector3D>>{ resVertexs, resValues};
	}

	QPair<QVector<QVector3D>, QVector<double>> mPostCuttingPlaneData::getCuttingNodeData(QHash<int, double> values, MViewBasic::NodeOrElement nodeOrElement)
	{
		QVector<QVector3D> trivertexs;
		QVector<double> trivalues;

		QVector3D vertex0, vertex1, vertex2, vertex3, vertex4, vertex5, vertex6, vertex7;
		if (_currentFrameRendData == nullptr)
		{
			return QPair<QVector<QVector3D>, QVector<double>>(trivertexs, trivalues);
		}
		int frameID = _currentFrameRendData->getRendID();
		if (_oneFrameData == nullptr)
		{
			return QPair<QVector<QVector3D>, QVector<double>>(trivertexs, trivalues);
		}

		if (nodeOrElement == MViewBasic::PostNode)
		{
			const QHash<int, QVector3D> &dis = _currentFrameRendData->getNodeDisplacementData();
			QVector3D deformationScale = _currentFrameRendData->getDeformationScale();
			auto iter = _oneFrameData->getMeshPartIterator();
			while (iter.hasNext())
			{
				iter.next();
				QString partName = iter.key();
				mPostMeshPartData1 *partData = iter.value();
				if (partData == nullptr)
				{
					continue;
				}
				if (!partData->getPartVisual())
				{
					continue;
				}
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

					QVector<int> index = meshData->getNodeIndex();
					QVector<QPair<QVector3D, double>> sss;
					if (meshData->getMeshType() == MeshTet)
					{
						vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
						vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
						vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
						vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));

						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, QVector<double>{values.value(index.at(0)), values.value(index.at(1))}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex2}, QVector<double>{values.value(index.at(0)), values.value(index.at(2))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, QVector<double>{values.value(index.at(0)), values.value(index.at(3))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, QVector<double>{values.value(index.at(1)), values.value(index.at(2))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex3}, QVector<double>{values.value(index.at(1)), values.value(index.at(3))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex3}, QVector<double>{values.value(index.at(2)), values.value(index.at(3))}, sss))
						{

						}
					}
					else if (meshData->getMeshType() == MeshPyramid)
					{
						vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
						vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
						vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
						vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
						vertex4 = _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));

						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, QVector<double>{values.value(index.at(0)), values.value(index.at(1))}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, QVector<double>{values.value(index.at(1)), values.value(index.at(2))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex3}, QVector<double>{values.value(index.at(2)), values.value(index.at(3))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, QVector<double>{values.value(index.at(0)), values.value(index.at(3))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex4}, QVector<double>{values.value(index.at(0)), values.value(index.at(4))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex4}, QVector<double>{values.value(index.at(1)), values.value(index.at(4))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex4}, QVector<double>{values.value(index.at(2)), values.value(index.at(4))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex3, vertex4}, QVector<double>{values.value(index.at(3)), values.value(index.at(4))}, sss))
						{

						}
					}
					else if (meshData->getMeshType() == MeshWedge)
					{
						vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
						vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
						vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
						vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
						vertex4 = _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));
						vertex5 = _oneFrameData->getNodeDataByID(index.at(5))->getNodeVertex() + deformationScale * dis.value(index.at(5));

						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, QVector<double>{values.value(index.at(0)), values.value(index.at(1))}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, QVector<double>{values.value(index.at(1)), values.value(index.at(2))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex0}, QVector<double>{values.value(index.at(2)), values.value(index.at(0))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, QVector<double>{values.value(index.at(0)), values.value(index.at(3))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex4}, QVector<double>{values.value(index.at(1)), values.value(index.at(4))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex5}, QVector<double>{values.value(index.at(2)), values.value(index.at(5))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex3, vertex4}, QVector<double>{values.value(index.at(3)), values.value(index.at(4))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex4, vertex5}, QVector<double>{values.value(index.at(4)), values.value(index.at(5))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex5, vertex3}, QVector<double>{values.value(index.at(5)), values.value(index.at(3))}, sss))
						{

						}
					}
					else if (meshData->getMeshType() == MeshHex)
					{
						vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
						vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
						vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
						vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
						vertex4 = _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));
						vertex5 = _oneFrameData->getNodeDataByID(index.at(5))->getNodeVertex() + deformationScale * dis.value(index.at(5));
						vertex6 = _oneFrameData->getNodeDataByID(index.at(6))->getNodeVertex() + deformationScale * dis.value(index.at(6));
						vertex7 = _oneFrameData->getNodeDataByID(index.at(7))->getNodeVertex() + deformationScale * dis.value(index.at(7));

						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, QVector<double>{values.value(index.at(0)), values.value(index.at(1))}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, QVector<double>{values.value(index.at(0)), values.value(index.at(3))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex4}, QVector<double>{values.value(index.at(0)), values.value(index.at(4))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, QVector<double>{values.value(index.at(1)), values.value(index.at(2))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex5}, QVector<double>{values.value(index.at(1)), values.value(index.at(5))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex3}, QVector<double>{values.value(index.at(2)), values.value(index.at(3))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex6}, QVector<double>{values.value(index.at(2)), values.value(index.at(6))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex3, vertex7}, QVector<double>{values.value(index.at(3)), values.value(index.at(7))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex4, vertex5}, QVector<double>{values.value(index.at(4)), values.value(index.at(5))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex4, vertex7}, QVector<double>{values.value(index.at(4)), values.value(index.at(7))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex5, vertex6}, QVector<double>{values.value(index.at(5)), values.value(index.at(6))}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex6, vertex7}, QVector<double>{values.value(index.at(6)), values.value(index.at(7))}, sss))
						{

						}
					}
					int num = sss.size();

/*					if (num == 3)
					{
						trivertexs.append(sss.at(0).first);
						trivertexs.append(sss.at(1).first);
						trivertexs.append(sss.at(2).first);
						trivalues.append(sss.at(0).second);
						trivalues.append(sss.at(1).second);
						trivalues.append(sss.at(2).second);
					}
					else */if (num > 2)
					{
						QVector3D center;
						for (int i = 0; i < num; ++i)
						{
							center += sss.at(i).first;
						}
						center = center / num;

						QVector3D OO = (sss.first().first - center).normalized();

						sort(sss.begin(), sss.end(), [this, center, OO](QPair<QVector3D, double> a, QPair<QVector3D, double> b)
						{
							QVector3D OA = a.first - center;
							QVector3D OB = b.first - center;

							//向量OA和向量OB的叉积
							QVector3D voa = QVector3D::crossProduct(OA, OO);
							QVector3D vob = QVector3D::crossProduct(OB, OO);

							double cosA = QVector3D::dotProduct(OA, OO) / (OA.length());
							double cosB = QVector3D::dotProduct(OB, OO) / (OB.length());

							if (QVector3D::dotProduct(voa, _normal) < 0)
							{
								cosA = -(cosA + 2);
							}
							if (QVector3D::dotProduct(vob, _normal) < 0)
							{
								cosB = -(cosB + 2);
							}
							return cosA < cosB;
						});

						//三角化
						for (int i = 1; i < num - 1; ++i)
						{
							trivertexs.append(sss.at(0).first);
							trivertexs.append(sss.at(i).first);
							trivertexs.append(sss.at(i + 1).first);
							trivalues.append(sss.at(0).second);
							trivalues.append(sss.at(i).second);
							trivalues.append(sss.at(i + 1).second);
						}
					}
				}
			}

		}
		else
		{
			const QHash<int, QVector3D> &dis = _currentFrameRendData->getNodeDisplacementData();
			QVector3D deformationScale = _currentFrameRendData->getDeformationScale();
			auto iter = _oneFrameData->getMeshPartIterator();
			while (iter.hasNext())
			{
				iter.next();
				QString partName = iter.key();
				mPostMeshPartData1 *partData = iter.value();
				if (partData == nullptr)
				{
					continue;
				}
				if (!partData->getPartVisual())
				{
					continue;
				}

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
					QVector<int> index = meshData->getNodeIndex();
					QVector<QVector3D> sss;
					double value = values.value(meshID);
					if (meshData->getMeshType() == MeshTet)
					{
						vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
						vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
						vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
						vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));

						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex2}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex3}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex3}, sss))
						{
						}
					}
					else if (meshData->getMeshType() == MeshPyramid)
					{
						vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
						vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
						vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
						vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
						vertex4 = _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));

						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex3}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex4}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex4}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex4}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex3, vertex4}, sss))
						{

						}
					}
					else if (meshData->getMeshType() == MeshWedge)
					{
						vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
						vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
						vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
						vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
						vertex4 = _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));
						vertex5 = _oneFrameData->getNodeDataByID(index.at(5))->getNodeVertex() + deformationScale * dis.value(index.at(5));

						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex0}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex4}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex5}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex3, vertex4}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex4, vertex5}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex5, vertex3}, sss))
						{

						}
					}
					else if (meshData->getMeshType() == MeshHex)
					{
						vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
						vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
						vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
						vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
						vertex4 = _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));
						vertex5 = _oneFrameData->getNodeDataByID(index.at(5))->getNodeVertex() + deformationScale * dis.value(index.at(5));
						vertex6 = _oneFrameData->getNodeDataByID(index.at(6))->getNodeVertex() + deformationScale * dis.value(index.at(6));
						vertex7 = _oneFrameData->getNodeDataByID(index.at(7))->getNodeVertex() + deformationScale * dis.value(index.at(7));

						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex4}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex5}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex3}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex6}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex3, vertex7}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex4, vertex5}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex4, vertex7}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex5, vertex6}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex6, vertex7}, sss))
						{
						}
					}
					int num = sss.size();
/*					if (num == 3)
					{
						trivertexs.append(sss.at(0));
						trivertexs.append(sss.at(1));
						trivertexs.append(sss.at(2));
						trivalues.append(value);
						trivalues.append(value);
						trivalues.append(value);
					}
					else */
					if (num > 2)
					{
						QVector3D center;
						for (int i = 0; i < num; ++i)
						{
							center += sss.at(i);
						}
						center = center / num;

						QVector3D OO = (sss.first() - center).normalized();

						sort(sss.begin(), sss.end(), [this, center, OO](QVector3D a, QVector3D b)
						{
							QVector3D OA = a - center;
							QVector3D OB = b - center;

							//向量OA和向量OB的叉积
							QVector3D voa = QVector3D::crossProduct(OA, OO);
							QVector3D vob = QVector3D::crossProduct(OB, OO);

							double cosA = QVector3D::dotProduct(OA, OO) / (OA.length());
							double cosB = QVector3D::dotProduct(OB, OO) / (OB.length());

							if (QVector3D::dotProduct(voa, _normal) < 0)
							{
								cosA = -(cosA + 2);
							}
							if (QVector3D::dotProduct(vob, _normal) < 0)
							{
								cosB = -(cosB + 2);
							}
							return cosA < cosB;
						});

						//三角化
						for (int i = 1; i < num - 1; ++i)
						{
							trivertexs.append(sss.at(0));
							trivertexs.append(sss.at(i));
							trivertexs.append(sss.at(i + 1));
							trivalues.append(value);
							trivalues.append(value);
							trivalues.append(value);
						}
					}
				}
			}
		}

		return QPair<QVector<QVector3D>, QVector<double>>(trivertexs, trivalues);
	}

	QPair<QVector<QVector3D>, QVector<QVector3D>> mPostCuttingPlaneData::getCuttingNodeData(QHash<int, QVector3D> vectorValues, MViewBasic::NodeOrElement nodeOrElement)
	{
		QVector<QVector3D> trivertexs;
		QVector<QVector3D> trivalues;

		QVector3D vertex0, vertex1, vertex2, vertex3, vertex4, vertex5, vertex6, vertex7;
		if (_currentFrameRendData == nullptr)
		{
			return QPair<QVector<QVector3D>, QVector<QVector3D>>(trivertexs, trivalues);
		}
		int frameID = _currentFrameRendData->getRendID();
		if (_oneFrameData == nullptr)
		{
			return QPair<QVector<QVector3D>, QVector<QVector3D>>(trivertexs, trivalues);
		}

		if (_currentFrameRendData->getNodeOrElement() == MViewBasic::PostNode)
		{
			const QHash<int, QVector3D> &dis = _currentFrameRendData->getNodeDisplacementData();
			QVector3D deformationScale = _currentFrameRendData->getDeformationScale();
			auto iter = _oneFrameData->getMeshPartIterator();
			while (iter.hasNext())
			{
				iter.next();
				QString partName = iter.key();
				mPostMeshPartData1 *partData = iter.value();
				if (partData == nullptr)
				{
					continue;
				}
				if (!partData->getPartVisual())
				{
					continue;
				}

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

					QVector<QVector3D> sss;
					QVector<QVector3D> isInvectorValue;
					QVector<int> index = meshData->getNodeIndex();
					if (meshData->getMeshType() == MeshTet)
					{
						vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
						vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
						vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
						vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));

						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(1))}, sss, isInvectorValue))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex2}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(2))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(2))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex3}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex3}, QVector<QVector3D>{vectorValues.value(index.at(2)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
						{

						}
					}
					else if (meshData->getMeshType() == MeshPyramid)
					{
						vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
						vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
						vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
						vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
						vertex4 = _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));

						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(1))}, sss, isInvectorValue))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(2))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex3}, QVector<QVector3D>{vectorValues.value(index.at(2)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex4}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(4))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex4}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(4))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex4}, QVector<QVector3D>{vectorValues.value(index.at(2)), vectorValues.value(index.at(4))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex3, vertex4}, QVector<QVector3D>{vectorValues.value(index.at(3)), vectorValues.value(index.at(4))}, sss, isInvectorValue))
						{

						}
					}
					else if (meshData->getMeshType() == MeshWedge)
					{
						vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
						vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
						vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
						vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
						vertex4 = _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));
						vertex5 = _oneFrameData->getNodeDataByID(index.at(5))->getNodeVertex() + deformationScale * dis.value(index.at(5));

						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(1))}, sss, isInvectorValue))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(2))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex0}, QVector<QVector3D>{vectorValues.value(index.at(2)), vectorValues.value(index.at(0))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex4}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(4))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex5}, QVector<QVector3D>{vectorValues.value(index.at(2)), vectorValues.value(index.at(5))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex3, vertex4}, QVector<QVector3D>{vectorValues.value(index.at(3)), vectorValues.value(index.at(4))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex4, vertex5}, QVector<QVector3D>{vectorValues.value(index.at(4)), vectorValues.value(index.at(5))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex5, vertex3}, QVector<QVector3D>{vectorValues.value(index.at(5)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
						{

						}
					}
					else if (meshData->getMeshType() == MeshHex)
					{
						vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
						vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
						vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
						vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
						vertex4 = _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));
						vertex5 = _oneFrameData->getNodeDataByID(index.at(5))->getNodeVertex() + deformationScale * dis.value(index.at(5));
						vertex6 = _oneFrameData->getNodeDataByID(index.at(6))->getNodeVertex() + deformationScale * dis.value(index.at(6));
						vertex7 = _oneFrameData->getNodeDataByID(index.at(7))->getNodeVertex() + deformationScale * dis.value(index.at(7));

						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(1))}, sss, isInvectorValue))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex4}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(4))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(2))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex5}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(5))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex3}, QVector<QVector3D>{vectorValues.value(index.at(2)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex6}, QVector<QVector3D>{vectorValues.value(index.at(2)), vectorValues.value(index.at(6))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex3, vertex7}, QVector<QVector3D>{vectorValues.value(index.at(3)), vectorValues.value(index.at(7))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex4, vertex5}, QVector<QVector3D>{vectorValues.value(index.at(4)), vectorValues.value(index.at(5))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex4, vertex7}, QVector<QVector3D>{vectorValues.value(index.at(4)), vectorValues.value(index.at(7))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex5, vertex6}, QVector<QVector3D>{vectorValues.value(index.at(5)), vectorValues.value(index.at(6))}, sss, isInvectorValue))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex6, vertex7}, QVector<QVector3D>{vectorValues.value(index.at(6)), vectorValues.value(index.at(7))}, sss, isInvectorValue))
						{

						}
					}
					int num = sss.size();

/*					if (num == 3)
					{
						trivertexs.append(sss.at(0));
						trivertexs.append(sss.at(1));
						trivertexs.append(sss.at(2));

						trivalues.append(isInvectorValue.at(0));
						trivalues.append(isInvectorValue.at(1));
						trivalues.append(isInvectorValue.at(2));
					}
					else */
					if (num > 2)
					{
						QVector3D center;
						for (int i = 0; i < num; ++i)
						{
							center += sss.at(i);
						}
						center = center / num;

						QVector3D OO = (sss.first() - center).normalized();

						sort(sss.begin(), sss.end(), [this, center, OO](QVector3D a, QVector3D b)
						{
							QVector3D OA = a - center;
							QVector3D OB = b - center;

							//向量OA和向量OB的叉积
							QVector3D voa = QVector3D::crossProduct(OA, OO);
							QVector3D vob = QVector3D::crossProduct(OB, OO);

							float cosA = QVector3D::dotProduct(OA, OO) / (OA.length());
							float cosB = QVector3D::dotProduct(OB, OO) / (OB.length());

							if (QVector3D::dotProduct(voa, _normal) < 0)
							{
								cosA = -(cosA + 2);
							}
							if (QVector3D::dotProduct(vob, _normal) < 0)
							{
								cosB = -(cosB + 2);
							}
							return cosA < cosB;
						});

						//三角化
						for (int i = 1; i < num - 1; ++i)
						{
							trivertexs.append(sss.at(0));
							trivertexs.append(sss.at(i));
							trivertexs.append(sss.at(i + 1));

							trivalues.append(isInvectorValue.at(0));
							trivalues.append(isInvectorValue.at(i));
							trivalues.append(isInvectorValue.at(i + 1));
						}
					}
				}
			}
		}

		return QPair<QVector<QVector3D>, QVector<QVector3D>>(trivertexs, trivalues);
	}

	void mPostCuttingPlaneData::caculateCuttingPlaneData()
	{
		QVector3D vertex0, vertex1, vertex2, vertex3, vertex4, vertex5, vertex6, vertex7;
		if (_currentFrameRendData == nullptr)
		{
			return;
		}
		int frameID = _currentFrameRendData->getRendID();
		if (_oneFrameData == nullptr)
		{
			return;
		}
		float fenmu = _currentFrameRendData->getMaxData() - _currentFrameRendData->getMinData();

		if (_currentFrameRendData->getNodeOrElement() == MViewBasic::PostNode)
		{
			//是否计算矢量
			if (!_hasVector)
			{
				QHash<int, float> values = _currentFrameRendData->getRendValue();
				const QHash<int, QVector3D> &dis = _currentFrameRendData->getNodeDisplacementData();
				QVector3D deformationScale = _currentFrameRendData->getDeformationScale();
				auto iter = _oneFrameData->getMeshPartIterator();
				while (iter.hasNext())
				{
					iter.next();
					QString partName = iter.key();
					mPostMeshPartData1 *partData = iter.value();
					if (partData == nullptr)
					{
						continue;
					}
					if (!partData->getPartVisual())
					{
						continue;
					}
					set<int> meshIDs = partData->getMeshIDs3();
					for (int meshID : meshIDs)
					{
						//if (meshID == 3366)
						//{
						//	continue;
						//}
						//if (meshID == 3108)
						//{
						//	continue;
						//}
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
						QVector<QPair<QVector3D, float>> sss;
						if (meshData->getMeshType() == MeshTet)
						{
							vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
							vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
							vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
							vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));

							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, QVector<float>{values.value(index.at(0)), values.value(index.at(1))}, sss))
							{
							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex2}, QVector<float>{values.value(index.at(0)), values.value(index.at(2))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, QVector<float>{values.value(index.at(0)), values.value(index.at(3))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, QVector<float>{values.value(index.at(1)), values.value(index.at(2))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex3}, QVector<float>{values.value(index.at(1)), values.value(index.at(3))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex3}, QVector<float>{values.value(index.at(2)), values.value(index.at(3))}, sss))
							{

							}
						}
						else if (meshData->getMeshType() == MeshPyramid)
						{
							vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
							vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
							vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
							vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
							vertex4 = _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));

							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, QVector<float>{values.value(index.at(0)), values.value(index.at(1))}, sss))
							{
							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, QVector<float>{values.value(index.at(1)), values.value(index.at(2))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex3}, QVector<float>{values.value(index.at(2)), values.value(index.at(3))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, QVector<float>{values.value(index.at(0)), values.value(index.at(3))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex4}, QVector<float>{values.value(index.at(0)), values.value(index.at(4))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex4}, QVector<float>{values.value(index.at(1)), values.value(index.at(4))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex4}, QVector<float>{values.value(index.at(2)), values.value(index.at(4))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex3, vertex4}, QVector<float>{values.value(index.at(3)), values.value(index.at(4))}, sss))
							{

							}
						}
						else if (meshData->getMeshType() == MeshWedge)
						{
							vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
							vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
							vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
							vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
							vertex4 = _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));
							vertex5 = _oneFrameData->getNodeDataByID(index.at(5))->getNodeVertex() + deformationScale * dis.value(index.at(5));

							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, QVector<float>{values.value(index.at(0)), values.value(index.at(1))}, sss))
							{
							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, QVector<float>{values.value(index.at(1)), values.value(index.at(2))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex0}, QVector<float>{values.value(index.at(2)), values.value(index.at(0))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, QVector<float>{values.value(index.at(0)), values.value(index.at(3))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex4}, QVector<float>{values.value(index.at(1)), values.value(index.at(4))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex5}, QVector<float>{values.value(index.at(2)), values.value(index.at(5))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex3, vertex4}, QVector<float>{values.value(index.at(3)), values.value(index.at(4))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex4, vertex5}, QVector<float>{values.value(index.at(4)), values.value(index.at(5))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex5, vertex3}, QVector<float>{values.value(index.at(5)), values.value(index.at(3))}, sss))
							{

							}
						}
						else if (meshData->getMeshType() == MeshHex)
						{
							vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
							vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
							vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
							vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
							vertex4 = _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));
							vertex5 = _oneFrameData->getNodeDataByID(index.at(5))->getNodeVertex() + deformationScale * dis.value(index.at(5));
							vertex6 = _oneFrameData->getNodeDataByID(index.at(6))->getNodeVertex() + deformationScale * dis.value(index.at(6));
							vertex7 = _oneFrameData->getNodeDataByID(index.at(7))->getNodeVertex() + deformationScale * dis.value(index.at(7));

							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, QVector<float>{values.value(index.at(0)), values.value(index.at(1))}, sss))
							{
							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, QVector<float>{values.value(index.at(0)), values.value(index.at(3))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex4}, QVector<float>{values.value(index.at(0)), values.value(index.at(4))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, QVector<float>{values.value(index.at(1)), values.value(index.at(2))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex5}, QVector<float>{values.value(index.at(1)), values.value(index.at(5))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex3}, QVector<float>{values.value(index.at(2)), values.value(index.at(3))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex6}, QVector<float>{values.value(index.at(2)), values.value(index.at(6))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex3, vertex7}, QVector<float>{values.value(index.at(3)), values.value(index.at(7))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex4, vertex5}, QVector<float>{values.value(index.at(4)), values.value(index.at(5))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex4, vertex7}, QVector<float>{values.value(index.at(4)), values.value(index.at(7))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex5, vertex6}, QVector<float>{values.value(index.at(5)), values.value(index.at(6))}, sss))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex6, vertex7}, QVector<float>{values.value(index.at(6)), values.value(index.at(7))}, sss))
							{

							}
						}
						int num = sss.size();

						//if (num == 3)
						{
	/*						_triPlane.append(sss.at(0).first);
							_triPlane.append(sss.at(1).first);
							_triPlane.append(sss.at(2).first);
							_triValue.append(sss.at(0).second);
							_triValue.append(sss.at(1).second);
							_triValue.append(sss.at(2).second);

							_triMeshID.append(QVector<float>(3, (float)meshID));*/
							//_triTexture.append((sss.at(0).second - _currentFrameRendData->getMinData()) / fenmu);
							//_triTexture.append((sss.at(1).second - _currentFrameRendData->getMinData()) / fenmu);
							//_triTexture.append((sss.at(2).second - _currentFrameRendData->getMinData()) / fenmu);
						}//
						if (num > 2)
						{
							QVector3D center;
							for (int i = 0; i < num; ++i)
							{
								center += sss.at(i).first;
							}
							center = center / num;

							QVector3D OO = (sss.first().first - center).normalized();

							sort(sss.begin(), sss.end(), [this, center, OO](QPair<QVector3D, float> a, QPair<QVector3D, float> b)
							{
								QVector3D OA = a.first - center;
								QVector3D OB = b.first - center;

								//向量OA和向量OB的叉积
								QVector3D voa = QVector3D::crossProduct(OA, OO);
								QVector3D vob = QVector3D::crossProduct(OB, OO);

								float cosA = QVector3D::dotProduct(OA, OO) / (OA.length());
								float cosB = QVector3D::dotProduct(OB, OO) / (OB.length());

								if (QVector3D::dotProduct(voa, _normal) < 0)
								{
									cosA = -(cosA + 2);
								}
								if (QVector3D::dotProduct(vob, _normal) < 0)
								{
									cosB = -(cosB + 2);
								}
								return cosA < cosB;
							});

							//排序
							//for (int i = 0; i < num; ++i)
							//{
							//	for (int j = 0; j < num - i - 1; ++j)
							//	{
							//		if (judgeVertexOrder(sss.at(j).first, sss.at(j + 1).first, center))
							//		{
							//			QPair<QVector3D, float> tmp = sss.at(j + 1);
							//			sss[j + 1] = sss[j];
							//			sss[j] = tmp;
							//		}
							//	}
							//}

							//三角化
							for (int i = 1; i < num - 1; ++i)
							{
								_triPlane.append(sss.at(0).first);
								_triPlane.append(sss.at(i).first);
								_triPlane.append(sss.at(i + 1).first);
								_triValue.append(sss.at(0).second);
								_triValue.append(sss.at(i).second);
								_triValue.append(sss.at(i + 1).second);
								_triMeshID.append(QVector<float>(3, (float)meshID));
								//_triTexture.append((sss.at(0).second - _currentFrameRendData->getMinData()) / fenmu);
								//_triTexture.append((sss.at(i).second - _currentFrameRendData->getMinData()) / fenmu);
								//_triTexture.append((sss.at(i + 1).second - _currentFrameRendData->getMinData()) / fenmu);

								qDebug() <<"计算法向" << QVector3D::normal((sss.at(i).first - sss.at(0).first), (sss.at(i + 1).first - sss.at(0).first));
							}
						}
					}
				}
			}
			else
			{
				QHash<int, float> values = _currentFrameRendData->getRendValue();
				QHash<int, QVector3D> vectorValues = _currentFrameRendData->getVectorRendData();
				const QHash<int, QVector3D> &dis = _currentFrameRendData->getNodeDisplacementData();
				QVector3D deformationScale = _currentFrameRendData->getDeformationScale();
				auto iter = _oneFrameData->getMeshPartIterator();
				while (iter.hasNext())
				{
					iter.next();
					QString partName = iter.key();
					mPostMeshPartData1 *partData = iter.value();
					if (partData == nullptr)
					{
						continue;
					}
					if (!partData->getPartVisual())
					{
						continue;
					}

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

						QVector<QPair<QVector3D, float>> sss;
						QVector<QVector3D> isInvectorValue;
						QVector<int> index = meshData->getNodeIndex();
						if (meshData->getMeshType() == MeshTet)
						{
							vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
							vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
							vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
							vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));

							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, QVector<float>{values.value(index.at(0)), values.value(index.at(1))}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(1))}, sss, isInvectorValue))
							{
							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex2}, QVector<float>{values.value(index.at(0)), values.value(index.at(2))}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(2))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, QVector<float>{values.value(index.at(0)), values.value(index.at(3))}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, QVector<float>{values.value(index.at(1)), values.value(index.at(2))}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(2))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex3}, QVector<float>{values.value(index.at(1)), values.value(index.at(3))}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex3}, QVector<float>{values.value(index.at(2)), values.value(index.at(3))}, QVector<QVector3D>{vectorValues.value(index.at(2)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
							{

							}
						}
						else if (meshData->getMeshType() == MeshPyramid)
						{
							vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
							vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
							vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
							vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
							vertex4 = _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));

							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, QVector<float>{values.value(index.at(0)), values.value(index.at(1))}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(1))}, sss, isInvectorValue))
							{
							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, QVector<float>{values.value(index.at(1)), values.value(index.at(2))}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(2))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex3}, QVector<float>{values.value(index.at(2)), values.value(index.at(3))}, QVector<QVector3D>{vectorValues.value(index.at(2)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, QVector<float>{values.value(index.at(0)), values.value(index.at(3))}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex4}, QVector<float>{values.value(index.at(0)), values.value(index.at(4))}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(4))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex4}, QVector<float>{values.value(index.at(1)), values.value(index.at(4))}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(4))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex4}, QVector<float>{values.value(index.at(2)), values.value(index.at(4))}, QVector<QVector3D>{vectorValues.value(index.at(2)), vectorValues.value(index.at(4))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex3, vertex4}, QVector<float>{values.value(index.at(3)), values.value(index.at(4))}, QVector<QVector3D>{vectorValues.value(index.at(3)), vectorValues.value(index.at(4))}, sss, isInvectorValue))
							{

							}
						}
						else if (meshData->getMeshType() == MeshWedge)
						{
							vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
							vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
							vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
							vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
							vertex4 = _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));
							vertex5 = _oneFrameData->getNodeDataByID(index.at(5))->getNodeVertex() + deformationScale * dis.value(index.at(5));

							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, QVector<float>{values.value(index.at(0)), values.value(index.at(1))}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(1))}, sss, isInvectorValue))
							{
							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, QVector<float>{values.value(index.at(1)), values.value(index.at(2))}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(2))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex0}, QVector<float>{values.value(index.at(2)), values.value(index.at(0))}, QVector<QVector3D>{vectorValues.value(index.at(2)), vectorValues.value(index.at(0))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, QVector<float>{values.value(index.at(0)), values.value(index.at(3))}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex4}, QVector<float>{values.value(index.at(1)), values.value(index.at(4))}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(4))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex5}, QVector<float>{values.value(index.at(2)), values.value(index.at(5))}, QVector<QVector3D>{vectorValues.value(index.at(2)), vectorValues.value(index.at(5))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex3, vertex4}, QVector<float>{values.value(index.at(3)), values.value(index.at(4))}, QVector<QVector3D>{vectorValues.value(index.at(3)), vectorValues.value(index.at(4))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex4, vertex5}, QVector<float>{values.value(index.at(4)), values.value(index.at(5))}, QVector<QVector3D>{vectorValues.value(index.at(4)), vectorValues.value(index.at(5))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex5, vertex3}, QVector<float>{values.value(index.at(5)), values.value(index.at(3))}, QVector<QVector3D>{vectorValues.value(index.at(5)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
							{

							}
						}
						else if (meshData->getMeshType() == MeshHex)
						{
							vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
							vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
							vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
							vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
							vertex4 = _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));
							vertex5 = _oneFrameData->getNodeDataByID(index.at(5))->getNodeVertex() + deformationScale * dis.value(index.at(5));
							vertex6 = _oneFrameData->getNodeDataByID(index.at(6))->getNodeVertex() + deformationScale * dis.value(index.at(6));
							vertex7 = _oneFrameData->getNodeDataByID(index.at(7))->getNodeVertex() + deformationScale * dis.value(index.at(7));

							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, QVector<float>{values.value(index.at(0)), values.value(index.at(1))}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(1))}, sss, isInvectorValue))
							{
							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, QVector<float>{values.value(index.at(0)), values.value(index.at(3))}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex4}, QVector<float>{values.value(index.at(0)), values.value(index.at(4))}, QVector<QVector3D>{vectorValues.value(index.at(0)), vectorValues.value(index.at(4))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, QVector<float>{values.value(index.at(1)), values.value(index.at(2))}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(2))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex5}, QVector<float>{values.value(index.at(1)), values.value(index.at(5))}, QVector<QVector3D>{vectorValues.value(index.at(1)), vectorValues.value(index.at(5))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex3}, QVector<float>{values.value(index.at(2)), values.value(index.at(3))}, QVector<QVector3D>{vectorValues.value(index.at(2)), vectorValues.value(index.at(3))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex6}, QVector<float>{values.value(index.at(2)), values.value(index.at(6))}, QVector<QVector3D>{vectorValues.value(index.at(2)), vectorValues.value(index.at(6))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex3, vertex7}, QVector<float>{values.value(index.at(3)), values.value(index.at(7))}, QVector<QVector3D>{vectorValues.value(index.at(3)), vectorValues.value(index.at(7))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex4, vertex5}, QVector<float>{values.value(index.at(4)), values.value(index.at(5))}, QVector<QVector3D>{vectorValues.value(index.at(4)), vectorValues.value(index.at(5))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex4, vertex7}, QVector<float>{values.value(index.at(4)), values.value(index.at(7))}, QVector<QVector3D>{vectorValues.value(index.at(4)), vectorValues.value(index.at(7))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex5, vertex6}, QVector<float>{values.value(index.at(5)), values.value(index.at(6))}, QVector<QVector3D>{vectorValues.value(index.at(5)), vectorValues.value(index.at(6))}, sss, isInvectorValue))
							{

							}
							if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex6, vertex7}, QVector<float>{values.value(index.at(6)), values.value(index.at(7))}, QVector<QVector3D>{vectorValues.value(index.at(6)), vectorValues.value(index.at(7))}, sss, isInvectorValue))
							{

							}
						}
						int num = sss.size();

						//if (num == 3)
						//{
						//	_triPlane.append(sss.at(0).first);
						//	_triPlane.append(sss.at(1).first);
						//	_triPlane.append(sss.at(2).first);
						//	_triValue.append(sss.at(0).second);
						//	_triValue.append(sss.at(1).second);
						//	_triValue.append(sss.at(2).second);
						//	_triMeshID.append(QVector<float>(3, (float)meshID));
						//	//_triTexture.append((sss.at(0).second - _currentFrameRendData->getMinData()) / fenmu);
						//	//_triTexture.append((sss.at(1).second - _currentFrameRendData->getMinData()) / fenmu);
						//	//_triTexture.append((sss.at(2).second - _currentFrameRendData->getMinData()) / fenmu);

						//	_triVectorValue.append(isInvectorValue.at(0));
						//	_triVectorValue.append(isInvectorValue.at(1));
						//	_triVectorValue.append(isInvectorValue.at(2));
						//}
						if (num > 2)
						{
							QVector3D center;
							for (int i = 0; i < num; ++i)
							{
								center += sss.at(i).first;
							}
							center = center / num;

							QVector3D OO = (sss.first().first - center).normalized();

							sort(sss.begin(), sss.end(), [this, center, OO](QPair<QVector3D, float> a, QPair<QVector3D, float> b)
							{
								QVector3D OA = a.first - center;
								QVector3D OB = b.first - center;

								//向量OA和向量OB的叉积
								QVector3D voa = QVector3D::crossProduct(OA, OO);
								QVector3D vob = QVector3D::crossProduct(OB, OO);

								float cosA = QVector3D::dotProduct(OA, OO) / (OA.length());
								float cosB = QVector3D::dotProduct(OB, OO) / (OB.length());

								if (QVector3D::dotProduct(voa, _normal) < 0)
								{
									cosA = -(cosA + 2);
								}
								if (QVector3D::dotProduct(vob, _normal) < 0)
								{
									cosB = -(cosB + 2);
								}
								return cosA < cosB;
							});

							//三角化
							for (int i = 1; i < num - 1; ++i)
							{
								_triPlane.append(sss.at(0).first);
								_triPlane.append(sss.at(i).first);
								_triPlane.append(sss.at(i + 1).first);
								_triValue.append(sss.at(0).second);
								_triValue.append(sss.at(i).second);
								_triValue.append(sss.at(i + 1).second);
								_triMeshID.append(QVector<float>(3, (float)meshID));
								//_triTexture.append((sss.at(0).second - _currentFrameRendData->getMinData()) / fenmu);
								//_triTexture.append((sss.at(i).second - _currentFrameRendData->getMinData()) / fenmu);
								//_triTexture.append((sss.at(i + 1).second - _currentFrameRendData->getMinData()) / fenmu);

								_triVectorValue.append(isInvectorValue.at(0));
								_triVectorValue.append(isInvectorValue.at(i));
								_triVectorValue.append(isInvectorValue.at(i + 1));
							}
						}
					}
				}
			}
		}
		else
		{
			QHash<int, float> values = _currentFrameRendData->getRendValue();
			const QHash<int, QVector3D> &dis = _currentFrameRendData->getNodeDisplacementData();
			QVector3D deformationScale = _currentFrameRendData->getDeformationScale();
			auto iter = _oneFrameData->getMeshPartIterator();
			while (iter.hasNext())
			{
				iter.next();
				QString partName = iter.key();
				mPostMeshPartData1 *partData = iter.value();
				if (partData == nullptr)
				{
					continue;
				}
				if (!partData->getPartVisual())
				{
					continue;
				}

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
					QVector<int> index = meshData->getNodeIndex();
					QVector<QVector3D> sss;
					float value = values.value(meshID);
					if (meshData->getMeshType() == MeshTet)
					{
						vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
						vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
						vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
						vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));

						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex2}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex3}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex3}, sss))
						{
						}
					}
					else if (meshData->getMeshType() == MeshPyramid)
					{
						vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
						vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
						vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
						vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
						vertex4 = _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));

						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex3}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex4}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex4}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex4}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex3, vertex4}, sss))
						{

						}
					}
					else if (meshData->getMeshType() == MeshWedge)
					{
						vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
						vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
						vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
						vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
						vertex4 = _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));
						vertex5 = _oneFrameData->getNodeDataByID(index.at(5))->getNodeVertex() + deformationScale * dis.value(index.at(5));

						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex0}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex4}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex5}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex3, vertex4}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex4, vertex5}, sss))
						{

						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex5, vertex3}, sss))
						{

						}
					}
					else if (meshData->getMeshType() == MeshHex)
					{
						vertex0 = _oneFrameData->getNodeDataByID(index.at(0))->getNodeVertex() + deformationScale * dis.value(index.at(0));
						vertex1 = _oneFrameData->getNodeDataByID(index.at(1))->getNodeVertex() + deformationScale * dis.value(index.at(1));
						vertex2 = _oneFrameData->getNodeDataByID(index.at(2))->getNodeVertex() + deformationScale * dis.value(index.at(2));
						vertex3 = _oneFrameData->getNodeDataByID(index.at(3))->getNodeVertex() + deformationScale * dis.value(index.at(3));
						vertex4 = _oneFrameData->getNodeDataByID(index.at(4))->getNodeVertex() + deformationScale * dis.value(index.at(4));
						vertex5 = _oneFrameData->getNodeDataByID(index.at(5))->getNodeVertex() + deformationScale * dis.value(index.at(5));
						vertex6 = _oneFrameData->getNodeDataByID(index.at(6))->getNodeVertex() + deformationScale * dis.value(index.at(6));
						vertex7 = _oneFrameData->getNodeDataByID(index.at(7))->getNodeVertex() + deformationScale * dis.value(index.at(7));

						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex1}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex3}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex0, vertex4}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex2}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex1, vertex5}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex3}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex2, vertex6}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex3, vertex7}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex4, vertex5}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex4, vertex7}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex5, vertex6}, sss))
						{
						}
						if (judgeLineIsCuttingByPlane(QVector<QVector3D>{vertex6, vertex7}, sss))
						{
						}
					}
					int num = sss.size();
					//if (num == 3)
					//{
					//	_triPlane.append(sss.at(0));
					//	_triPlane.append(sss.at(1));
					//	_triPlane.append(sss.at(2));
					//	_triValue.append(value);
					//	_triValue.append(value);
					//	_triValue.append(value);
					//	_triMeshID.append(QVector<float>(3, (float)meshID));
					//	//_triTexture.append((value - _currentFrameRendData->getMinData()) / fenmu);
					//	//_triTexture.append((value - _currentFrameRendData->getMinData()) / fenmu);
					//	//_triTexture.append((value - _currentFrameRendData->getMinData()) / fenmu);
					//}
					if (num > 2)
					{
						QVector3D center;
						for (int i = 0; i < num; ++i)
						{
							center += sss.at(i);
						}
						center = center / num;

						QVector3D OO = (sss.first() - center).normalized();

						sort(sss.begin(), sss.end(), [this, center, OO](QVector3D a, QVector3D b)
						{
							QVector3D OA = a - center;
							QVector3D OB = b - center;

							//向量OA和向量OB的叉积
							QVector3D voa = QVector3D::crossProduct(OA, OO);
							QVector3D vob = QVector3D::crossProduct(OB, OO);

							float cosA = QVector3D::dotProduct(OA, OO) / (OA.length());
							float cosB = QVector3D::dotProduct(OB, OO) / (OB.length());

							if (QVector3D::dotProduct(voa, _normal) < 0)
							{
								cosA = -(cosA + 2);
							}
							if (QVector3D::dotProduct(vob, _normal) < 0)
							{
								cosB = -(cosB + 2);
							}
							return cosA < cosB;
						});

						//三角化
						for (int i = 1; i < num - 1; ++i)
						{
							_triPlane.append(sss.at(0));
							_triPlane.append(sss.at(i));
							_triPlane.append(sss.at(i + 1));
							_triValue.append(value);
							_triValue.append(value);
							_triValue.append(value);
							_triMeshID.append(QVector<float>(3, (float)meshID));
							//_triTexture.append((value - _currentFrameRendData->getMinData()) / fenmu);
							//_triTexture.append((value - _currentFrameRendData->getMinData()) / fenmu);
							//_triTexture.append((value - _currentFrameRendData->getMinData()) / fenmu);
						}
					}
				}
			}
		}

	}

	mPostCuttingPlaneRender::mPostCuttingPlaneRender(std::shared_ptr<mxr::Group> parent, std::shared_ptr<mxr::StateSet> cuttingPlaneStateSet, std::shared_ptr<mxr::StateSet> transparentPlaneStateSet)
	{
		_parent = parent;
		_cuttingPlaneDrawable = nullptr;
		_transparentPlaneDrawable = nullptr;
		_cuttingPlaneStateSet = cuttingPlaneStateSet;
		_transparentPlaneStateSet = transparentPlaneStateSet;
		_postCuttingPlaneData = nullptr;
	}

	mPostCuttingPlaneRender::~mPostCuttingPlaneRender()
	{
		if (_cuttingPlaneDrawable)
		{
			_parent->removeChild(_cuttingPlaneDrawable);
		}
		//这儿析构后有问题
		if (_transparentPlaneDrawable)
		{
			_parent->removeChild(_transparentPlaneDrawable);
		}
	}

	void mPostCuttingPlaneRender::createCuttingPlane(mOneFrameData1 *oneFramData, mPostOneFrameRendData* currentFrameRendData,int index, QVector3D normal, QVector3D vertex, bool hasVector)
	{
		if (_postCuttingPlaneData)
		{
			delete _postCuttingPlaneData;
		}
		_postCuttingPlaneData = new mPostCuttingPlaneData(oneFramData, currentFrameRendData, normal, vertex, hasVector);
		UpdateCuttingPlane(index);
	}

	void mPostCuttingPlaneRender::setPlaneData(QVector3D normal, QVector3D centervertex, float maxR)
	{
		if (_postCuttingPlaneData)
		{
			_postCuttingPlaneData->setPlaneData(normal, centervertex, maxR);
		}

		if (_transparentPlaneDrawable)
		{
			_parent->removeChild(_transparentPlaneDrawable);
		}
		_transparentPlaneDrawable = MakeAsset<Drawable>();
		asset_ref<Vec3Array> _vertex1 = MakeAsset<Vec3Array>(_postCuttingPlaneData->_planeVertex);
		_transparentPlaneDrawable->setVertexAttribArray(0, _vertex1);
		_transparentPlaneDrawable->setStateSet(_transparentPlaneStateSet);
		_parent->addChild(_transparentPlaneDrawable);

	}

	QVector4D mPostCuttingPlaneRender::getCuttingPlane()
	{
		return _postCuttingPlaneData->getCuttingPlane();
	}

	void mPostCuttingPlaneRender::setIsShowCuttingPlane(bool isShow)
	{
		if (_transparentPlaneDrawable)
		{
			if (isShow)
			{
				_transparentPlaneDrawable->setNodeMask(0);
			}
			else
			{
				_transparentPlaneDrawable->setNodeMask(1);
			}
		}
	}

	void mPostCuttingPlaneRender::reverseCuttingPlaneNormal()
	{
		_postCuttingPlaneData->reverseCuttingPlaneNormal();
	}

	void mPostCuttingPlaneRender::updateCuttingPlaneIndex(int index)
	{
		if (_cuttingPlaneDrawable)
		{
			Array *array = _cuttingPlaneDrawable->getVertexAttribArray(2);
			array->updata(0, array->size() * 4, QVector<float>(array->size(), index).data());
		}
	}

	void mPostCuttingPlaneRender::UpdateCuttingPlane(int index)
	{
		if (_cuttingPlaneDrawable)
		{
			_parent->removeChild(_cuttingPlaneDrawable);
		}

		_cuttingPlaneDrawable = MakeAsset<Drawable>();
		asset_ref<Vec3Array> _vertex0 = MakeAsset<Vec3Array>(_postCuttingPlaneData->getTriVertex());
		asset_ref<FloatArray> texture = MakeAsset<FloatArray>(_postCuttingPlaneData->getTriTexture());
		QVector<float> indexs(_postCuttingPlaneData->getTriVertex().size(), index);
		_cuttingPlaneDrawable->setVertexAttribArray(0, _vertex0);
		_cuttingPlaneDrawable->setVertexAttribArray(1, texture);
		_cuttingPlaneDrawable->setVertexAttribArray(2, MakeAsset<FloatArray>(indexs));
		_cuttingPlaneDrawable->setStateSet(_cuttingPlaneStateSet);

		_parent->addChild(_cuttingPlaneDrawable);

	}
}