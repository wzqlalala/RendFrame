#include "Space.h"
namespace mxr
{
	AABB::AABB(QVector3D p1, QVector3D p2)
	{
		maxEdge.setX(std::max(p1.x(), p2.x()));
		maxEdge.setY(std::max(p1.y(), p2.y()));
		maxEdge.setZ(std::max(p1.z(), p2.z()));
		minEdge.setX(std::min(p1.x(), p2.x()));
		minEdge.setY(std::min(p1.y(), p2.y()));
		minEdge.setZ(std::min(p1.z(), p2.z()));
	}

	void AABB::init(QVector3D p)
	{
		maxEdge = minEdge = p;
	}

	AABB::AABB()
	{
		maxEdge = QVector3D(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		minEdge = QVector3D(FLT_MAX, FLT_MAX, FLT_MAX);
	}

	bool AABB::ContainPoint(QVector3D p1)
	{
		if (minEdge.x() < p1.x() && minEdge.y() < p1.y() && minEdge.z() < p1.z() &&
			maxEdge.x() > p1.x() && maxEdge.y() > p1.y() && maxEdge.z() > p1.z())
		{
			return true;
		}
		return false;
	}

	void AABB::push(AABB aabb)
	{
		minEdge.setX(std::min(aabb.minEdge.x(), minEdge.x()));
		minEdge.setY(std::min(aabb.minEdge.y(), minEdge.y()));
		minEdge.setZ(std::min(aabb.minEdge.z(), minEdge.z()));

		maxEdge.setX(std::max(aabb.maxEdge.x(), maxEdge.x()));
		maxEdge.setY(std::max(aabb.maxEdge.y(), maxEdge.y()));
		maxEdge.setZ(std::max(aabb.maxEdge.z(), maxEdge.z()));
	}

	void AABB::push(QVector3D p1, QVector3D p2)
	{
		minEdge.setX(std::min(p1.x(), minEdge.x()));
		minEdge.setY(std::min(p1.y(), minEdge.y()));
		minEdge.setZ(std::min(p1.z(), minEdge.z()));

		maxEdge.setX(std::max(p2.x(), maxEdge.x()));
		maxEdge.setY(std::max(p2.y(), maxEdge.y()));
		maxEdge.setZ(std::max(p2.z(), maxEdge.z()));
	}



	void AABB::push(QVector3D p1)
	{
		maxEdge.setX(std::max(p1.x(), maxEdge.x()));
		maxEdge.setY(std::max(p1.y(), maxEdge.y()));
		maxEdge.setZ(std::max(p1.z(), maxEdge.z()));
		minEdge.setX(std::min(p1.x(), minEdge.x()));
		minEdge.setY(std::min(p1.y(), minEdge.y()));
		minEdge.setZ(std::min(p1.z(), minEdge.z()));
	}

	bool AABB::empty()
	{
		if (maxEdge.x() == minEdge.x() && maxEdge.y() == minEdge.y() && maxEdge.z() == minEdge.z())
		{
			return true;
		}
		return false;
	}


	bool AABB::intersects(QVector3D origin, QVector3D dir, QVector3D &ptOnPlane)
	{
		//射线与包围盒某面的交点
		QVector3D min = minEdge; //aabb包围盒最小点坐标
		QVector3D max = maxEdge; //aabb包围盒最大点坐标

		float t;

		//分别判断射线与各面的相交情况

		//判断射线与包围盒x轴方向的面是否有交点
		if (dir.x() != 0.f) //射线x轴方向分量不为0 若射线方向矢量的x轴分量为0，射线不可能经过包围盒朝x轴方向的两个面
		{
			/*
			  使用射线与平面相交的公式求交点
			 */
			if (dir.x() > 0)//若射线沿x轴正方向偏移
				t = (min.x() - origin.x()) / dir.x();
			else  //射线沿x轴负方向偏移
				t = (max.x() - origin.x()) / dir.x();

			if (t > 0.f) //t>0时则射线与平面相交
			{
				ptOnPlane = origin + t * dir; //计算交点坐标
				//判断交点是否在当前面内
				if (min.y() < ptOnPlane.y() && ptOnPlane.y() < max.y() && min.z() < ptOnPlane.z() && ptOnPlane.z() < max.z())
				{
					return true; //射线与包围盒有交点
				}
			}
		}

		//若射线沿y轴方向有分量 判断是否与包围盒y轴方向有交点
		if (dir.y() != 0.f)
		{
			if (dir.y() > 0)
				t = (min.y() - origin.y()) / dir.y();
			else
				t = (max.y() - origin.y()) / dir.y();

			if (t > 0.f)
			{
				ptOnPlane = origin + t * dir;

				if (min.z() < ptOnPlane.z() && ptOnPlane.z() < max.z() && min.x() < ptOnPlane.x() && ptOnPlane.x() < max.x())
				{
					return true;
				}
			}
		}

		//若射线沿z轴方向有分量 判断是否与包围盒y轴方向有交点
		if (dir.z() != 0.f)
		{
			if (dir.z() > 0)
				t = (min.z() - origin.z()) / dir.z();
			else
				t = (max.z() - origin.z()) / dir.z();

			if (t > 0.f)
			{
				ptOnPlane = origin + t * dir;

				if (min.x() < ptOnPlane.x() && ptOnPlane.x() < max.x() && min.y() < ptOnPlane.y() && ptOnPlane.y() < max.y())
				{
					return true;
				}
			}
		}

		return false;
	}

	bool AABB::isContain(AABB aabb)
	{
		if (minEdge.x() < aabb.minEdge.x() && minEdge.y() < aabb.minEdge.y() && minEdge.z() < aabb.minEdge.z()
			&& maxEdge.x() > aabb.maxEdge.x() && maxEdge.y() > aabb.maxEdge.y() && maxEdge.z() > aabb.maxEdge.z()
			)
		{
			return true;
		}
		return false;
	}

	bool AABB::IsIntersect(AABB aabb)
	{
		if (maxEdge.x() < aabb.minEdge.x() || minEdge.x() > aabb.maxEdge.x())
		{
			return false;
		}
		if (maxEdge.y() < aabb.minEdge.y() || minEdge.y() > aabb.maxEdge.y())
		{
			return false;
		}
		if (maxEdge.z() < aabb.minEdge.z() || minEdge.z() > aabb.maxEdge.z())
		{
			return false;
		}
		return true;
	}
}

