#pragma once
#include <qvector3d.h>
#include <qvector.h>
#include "export.h"
namespace mxr
{
	struct MXR_EXPORT AABB
	{
		QVector3D maxEdge;
		QVector3D minEdge;
		AABB();
		AABB(QVector3D p1, QVector3D p2);
		void init(QVector3D p);
		bool ContainPoint(QVector3D p1);
		void push(AABB aabb);
		void push(QVector3D p1, QVector3D p2);
		void push(QVector3D p1);
		bool empty();
		bool intersects(QVector3D origin, QVector3D dir, QVector3D &ptOnPlane);
		bool isContain(AABB aabb);
		bool IsIntersect(AABB aabb);

	};


	struct MXR_EXPORT OBB
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


			if (_inx < (a._size.x() / 2.0) && _iny < (a._size.y() / 2.0) && _inz < (a._size.z() / 2.0))
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
		}
	};
}
