#pragma once



#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <qvector3d.h>
#include <qvector.h>
#include "renddata_global.h"
namespace MDataPost
{
	class mPostMeshPartData1;
	class mPostMeshData1;
	class mPostMeshNodeData1;
	class mPostMeshData1;
	class mPostMeshLineData1;
	class mPostMeshFaceData1;
	class mOneFrameData1;
}
namespace Space
{



	struct RENDDATA_EXPORT AABB
	{
		QVector3D maxEdge;
		QVector3D minEdge;
		AABB();
		AABB(QVector3D p);
		AABB(QVector3D p1, QVector3D p2);
		void init(QVector3D p);
		bool ContainPoint(QVector3D p1);
		void push(AABB aabb);
		void push(QVector3D p1, QVector3D p2);
		void push(QVector3D p1);
		void push(QVector<QVector3D> p1);
		bool empty();
		bool intersects(QVector3D origin, QVector3D dir, QVector3D &ptOnPlane);
		bool isContain(AABB aabb);
		bool IsIntersect(AABB aabb);
		float getLongestEdge();
	};




	

	

	struct RENDDATA_EXPORT OBB
	{

		//方向向量为单位向量

		//  X 轴方向向量
		QVector3D _axisX;
		// Y 轴方向向量
		QVector3D _axisY;
		// Z 轴方向向量		
		QVector3D _axisZ;
		// 中心点坐标
		QVector3D _center;
		// 三条边长度
		QVector3D _size;

		// 八个顶点坐标
		QVector3D _vertexs[8];


		void Set(QVector3D axisX, QVector3D axisY, QVector3D axisZ, QVector3D center, QVector3D size)
		{
			_axisX = axisX;
			_axisY = axisY;
			_axisZ = axisZ;
			_center = center;
			_size = size;

			_vertexs[0] = center + (axisX * size.x() + axisY * size.y() + axisZ * size.z()) * 0.5f;
			_vertexs[1] = center + (axisX * size.x() - axisY * size.y() + axisZ * size.z()) * 0.5f;
			_vertexs[2] = center + (axisX * size.x() + axisY * size.y() - axisZ * size.z()) * 0.5f;
			_vertexs[3] = center + (axisX * size.x() - axisY * size.y() - axisZ * size.z()) * 0.5f;
			_vertexs[4] = center + (-axisX * size.x() + axisY * size.y() - axisZ * size.z()) * 0.5f;
			_vertexs[5] = center + (-axisX * size.x() - axisY * size.y() - axisZ * size.z()) * 0.5f;
			_vertexs[6] = center + (-axisX * size.x() - axisY * size.y() + axisZ * size.z()) * 0.5f;
			_vertexs[7] = center + (-axisX * size.x() + axisY * size.y() + axisZ * size.z()) * 0.5f;

		}

		static bool IsIntersect3D(OBB a, OBB b)
		{
			QVector3D cross1 = QVector3D::crossProduct(a._axisX, b._axisX);
			QVector3D cross2 = QVector3D::crossProduct(a._axisX, b._axisY);
			QVector3D cross3 = QVector3D::crossProduct(a._axisX, b._axisZ);
			QVector3D cross4 = QVector3D::crossProduct(a._axisY, b._axisX);
			QVector3D cross5 = QVector3D::crossProduct(a._axisY, b._axisY);
			QVector3D cross6 = QVector3D::crossProduct(a._axisY, b._axisZ);
			QVector3D cross7 = QVector3D::crossProduct(a._axisZ, b._axisX);
			QVector3D cross8 = QVector3D::crossProduct(a._axisZ, b._axisY);
			QVector3D cross9 = QVector3D::crossProduct(a._axisZ, b._axisZ);

			// 如果有一个分离轴上不相交，则OBB1 和 OBB2 不相交
			return !(IsNotIntersectInAxis(a._vertexs, b._vertexs, a._axisX)
				|| IsNotIntersectInAxis(a._vertexs, b._vertexs, a._axisY)
				|| IsNotIntersectInAxis(a._vertexs, b._vertexs, a._axisZ)
				|| IsNotIntersectInAxis(a._vertexs, b._vertexs, b._axisX)
				|| IsNotIntersectInAxis(a._vertexs, b._vertexs, b._axisY)
				|| IsNotIntersectInAxis(a._vertexs, b._vertexs, b._axisZ)
				|| IsNotIntersectInAxis(a._vertexs, b._vertexs, cross1)
				|| IsNotIntersectInAxis(a._vertexs, b._vertexs, cross2)
				|| IsNotIntersectInAxis(a._vertexs, b._vertexs, cross3)
				|| IsNotIntersectInAxis(a._vertexs, b._vertexs, cross4)
				|| IsNotIntersectInAxis(a._vertexs, b._vertexs, cross5)
				|| IsNotIntersectInAxis(a._vertexs, b._vertexs, cross6)
				|| IsNotIntersectInAxis(a._vertexs, b._vertexs, cross7)
				|| IsNotIntersectInAxis(a._vertexs, b._vertexs, cross8)
				|| IsNotIntersectInAxis(a._vertexs, b._vertexs, cross9));
		}


		// OBB 在 axis 轴上没有相交
		static bool IsNotIntersectInAxis(QVector3D vertexs1[], QVector3D vertexs2[], QVector3D axis)
		{
			QVector<float> range1 = VertexProject(vertexs1, axis);
			QVector<float> range2 = VertexProject(vertexs2, axis);
			return range1[0] > range2[1] || range2[0] > range1[1];
		}


		// 顶点在轴上的投影的最小值和最大值
		static QVector<float> VertexProject(QVector3D vertexs[], QVector3D axis)
		{
			QVector<float> range;
			range.resize(2);
			for (int i = 0; i < 8; ++i)
			{
				QVector3D vertex = vertexs[i];
				float dot = QVector3D::dotProduct(vertex, axis);

				if (i == 0)
				{
					range[0] = range[1] = dot;
				}
				else
				{
					range[0] = std::min(range[0], dot);
					range[1] = std::max(range[1], dot);
				}
			}
			return range;
		}




		static bool IsIntersect3D(OBB a, AABB b)
		{
			OBB _b;
			_b.Set(QVector3D(1, 0, 0), QVector3D(0, 1, 0), QVector3D(0, 0, 1), (b.maxEdge + b.minEdge)*0.5, QVector3D(b.maxEdge - b.minEdge));
			return IsIntersect3D(a, _b);
		}


		static bool IsIntersect3D(OBB a, QVector3D b)
		{
			QVector3D dis = b - a._center;

			float _inx = fabs(QVector3D::dotProduct(dis, a._axisX));
			float _iny = fabs(QVector3D::dotProduct(dis, a._axisY));
			float _inz = fabs(QVector3D::dotProduct(dis, a._axisZ));


			if (_inx < (a._size.x()/2.0) && _iny < (a._size.y()/2.0) && _inz < (a._size.z()/2.0))
			{
				return true;
			}
			return false;

		}

		static bool obbInOtherobb(OBB a, OBB b)	//判断a是否在b里面
		{
			for (int i = 0; i < 8; i++)
			{
				if (!IsIntersect3D(b, a._vertexs[i]))
				{
					return false;
				}
			}
			return true;
		}


		static bool AABBInOtherobb(AABB b, OBB a)
		{
			
			OBB _b;
			_b.Set(QVector3D(1, 0, 0), QVector3D(0, 1, 0), QVector3D(0, 0, 1), (b.maxEdge + b.minEdge)*0.5, QVector3D(b.maxEdge - b.minEdge));
			return obbInOtherobb(_b, a);
			/*
			QVector<QVector3D> obbAxes{ a._axisX, a._axisY,a._axisZ };
			for (int i = 0; i < 3; ++i) {
				// 计算当前轴在 OBB 上的投影区间
				float obbMin = INFINITY, obbMax = -INFINITY;
				for (int j = 0; j < 8; ++j) {
					float projection = QVector3D::dotProduct(a._vertexs[j], obbAxes[i]);
					if (projection < obbMin)
						obbMin = projection;
					if (projection > obbMax)
						obbMax = projection;
				}

				// 计算当前轴在 AABB 上的投影区间
				float aabbProjectionMin = QVector3D::dotProduct(b.minEdge, obbAxes[i]);
				float aabbProjectionMax = QVector3D::dotProduct(b.maxEdge, obbAxes[i]);

				// 检查投影区间是否重叠，如果不重叠则返回 false
				if (aabbProjectionMax < obbMin || aabbProjectionMin > obbMax)
					return false;
			}

			// 如果所有投影区间都重叠，则返回 true
			return true;
			*/
		}
	};







	enum RENDDATA_EXPORT SpaceKind
	{
		TopFrontLeft,
		TopFrontRight,
		TopBackLeft,
		TopBackRight,
		BottomFrontLeft,
		BottomFrontRight,
		BottomBackLeft,
		BottomBackRight
	};

	class RENDDATA_EXPORT SpaceTree
	{
	public:
		
		AABB space;
		int depth;
		SpaceTree* topFrontLeft = nullptr;
		SpaceTree* topFrontRight = nullptr;
		SpaceTree* topBackLeft = nullptr;
		SpaceTree* topBackRight = nullptr;
		SpaceTree* bottomFrontLeft = nullptr;
		SpaceTree* bottomFrontRight = nullptr;
		SpaceTree* bottomBackLeft = nullptr;
		SpaceTree* bottomBackRight = nullptr;
		QVector<MDataPost::mPostMeshData1*> meshs;

		SpaceTree(AABB aabb);
		
		bool IsIntersect(AABB aabb);
		bool ContainPoint(QVector3D p);

	};

	void RENDDATA_EXPORT initSpaceTree(SpaceTree* root, int depth);
	void RENDDATA_EXPORT addMeshAABBToSpace(SpaceTree* root, MDataPost::mPostMeshData1 * mesh, MDataPost::mOneFrameData1 *fd, AABB aabb);
	void RENDDATA_EXPORT addMeshToSpace(SpaceTree* root, MDataPost::mPostMeshData1 *mesh, MDataPost::mOneFrameData1 *fd);
	void RENDDATA_EXPORT getPointToSpaceTree(SpaceTree* root, QVector3D p, SpaceTree *&require);
	void RENDDATA_EXPORT printSpace(SpaceTree* root);
	AABB RENDDATA_EXPORT getSpace(AABB aabb, SpaceKind mode);
	void RENDDATA_EXPORT getRayToSpaceData(SpaceTree* root, QVector3D origin, QVector3D dir, QVector3D &ping_pos, float &depth, SpaceTree* &reauire);

	void RENDDATA_EXPORT getRayToSpaceData(SpaceTree* root, QVector3D origin, QVector3D dir, QVector<MDataPost::mPostMeshData1*> &mesh);


	bool RENDDATA_EXPORT IntersectTriangle(const QVector3D& orig, const  QVector3D& dir, const QVector3D& v0, const QVector3D& v1, const QVector3D& v2,
		float &u, float &v, float &t, float& distance);
	
	bool RENDDATA_EXPORT rayTriangleIntersect(QVector3D orig, QVector3D dir,
		QVector3D a, QVector3D b, QVector3D c, float &t, float &b1, float &b2);

	void RENDDATA_EXPORT getAABBToMeshData(SpaceTree* root, AABB aabb, QVector<MDataPost::mPostMeshData1*> &mesh);
	//void RENDDATA_EXPORT getAABBInTreeToMeshData(SpaceTree* root, AABB aabb, QVector<MDataPost::mPostMeshData1*> &mesh);
	void RENDDATA_EXPORT getAABBToMeshData(SpaceTree * root, AABB aabb, QVector<MDataPost::mPostMeshData1*>& mesh,QVector<MDataPost::mPostMeshData1*>& mesh2);


	void RENDDATA_EXPORT getOBBToMeshData(SpaceTree * root, OBB aabb, QVector<MDataPost::mPostMeshData1*>& mesh);
	void RENDDATA_EXPORT getOBBToMeshData(SpaceTree * root, OBB aabb, QVector<MDataPost::mPostMeshData1*>& mesh, QVector<MDataPost::mPostMeshData1*>& mesh2);


	void RENDDATA_EXPORT destoryTree(SpaceTree* root);


}
