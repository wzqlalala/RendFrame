#include "mPickToolClass.h"
#include<math.h>



#include <QVector2D>
#include <QVector4D>
#include <QMatrix4x4>

#define optimization true

using namespace MBasicFunction;
namespace MViewBasic
{
	QHash<MBasicFunction::MeshType, QVector<int>> mPickToolClass::LineIntersectionWithQuadVector = {
		{MeshBeam ,QVector<int>{0,1} },
		{MeshTri ,QVector<int>{0,1,1,2,2,0} },
		{MeshQuad ,QVector<int>{0,1,1,2,2,3,3,0 } },
		{MeshTet ,QVector<int>{ 0,1,0,2,0,3,1,2,1,3,2,3} },
		{MeshWedge ,QVector<int>{0,1,1,2,2,0,3,4,4,5,5,3,0,3,1,4,2,5} },
		{MeshHex ,QVector<int>{0,1,1,5,5,4,4,0,2,3,3,7,7,6,6,2,1,2,5,6,4,7,0,3 } },
		{MeshPyramid ,QVector<int>{0,1,1,2,2,3,3,0,0,4,1,4,2,4,3,4} },
	};

	QHash<MBasicFunction::MeshType, QVector<QVector<int>>> mPickToolClass::QuadPointInMeshVector = { 
	{MeshTri ,QVector<QVector<int>>{ QVector<int>{0,1,1,2,2,0} }},
	{MeshQuad ,QVector<QVector<int>>{ QVector<int>{ 0,1,1,2,2,0},QVector<int>{2,3,3,1,1,2},QVector<int>{3,0,0,2,2,3},QVector<int>{1,0,0,3,3,1} }},
	{MeshTet ,QVector<QVector<int>>{ QVector<int>{ 0,1,1,2,2,0}, QVector<int>{3,4,4,5,5,3},QVector<int>{0,2,2,5,5,3,3,0 },QVector<int>{1,4,4,5,5,2,2,1 },QVector<int>{0,3,3,4,4,1,1,0 } }},
	{MeshWedge ,QVector<QVector<int>>{ QVector<int>{ 0,1,1,2,2,0}, QVector<int>{3,4,4,5,5,3},QVector<int>{0,2,2,5,5,3,3,0 },QVector<int>{1,4,4,5,5,2,2,1 },QVector<int>{0,3,3,4,4,1,1,0 } }},
	{MeshHex ,QVector<QVector<int>>{ QVector<int>{ 0,1,1,5,5,4,4,0},QVector<int>{2,3,3,7,7,6,6,2},QVector<int>{1,2,2,6,6,5,5,1},QVector<int>{4,5,5,6,6,7,7,4},QVector<int>{0,4,4,7,7,3,3,0},QVector<int>{0,3,3,2,2,1,1,0} }},
	{MeshPyramid ,QVector<QVector<int>>{ QVector<int>{ 0,1,1,2,2,3,3,0},QVector<int>{0,1,1,4,4,0},QVector<int>{1,2,2,4,4,1},QVector<int>{2,3,3,4,4,2},QVector<int>{3,0,0,4,4,3} }},
	};

	#if optimization

	bool mPickToolClass::IsLineIntersectionWithQuad(const QVector<QVector2D> &Line, const QVector<QVector2D> &Quad, MeshType meshtype)
	{
		bool isintersection{ false };
		const QVector<int> &list = LineIntersectionWithQuadVector.value(meshtype);
		QVector2D ap1, ap2, bp1, bp2;
		for (int i = 0; i < list.size(); i += 2)
		{
			ap1 = Line.at(list.at(i));
			ap2 = Line.at(list.at(i + 1));
			if (!isintersection)//避免重复判断
			{
				for (int k = 0; k < Quad.size(); ++k)
				{
					if (k == Quad.size() - 1)
					{
						bp1 = Quad.at(k);
						bp2 = Quad.at(0);
					}
					else
					{
						bp1 = Quad.at(k);
						bp2 = Quad.at(k + 1);
					}
					QVector2D v = bp2 - bp1;
					QVector2D v1 = bp2 - ap1;
					QVector2D v2 = bp2 - ap2;
					float c1 = v.x()*v1.y() - v.y()*v1.x();
					float c2 = v.x()*v2.y() - v.y()*v2.x();

					v = ap2 - ap1;
					QVector2D v3 = ap2 - bp1;
					QVector2D v4 = ap2 - bp2;
					float c3 = v.x()*v3.y() - v.y()*v3.x();
					float c4 = v.x()*v4.y() - v.y()*v4.x();
					if ((c1 / c2 <= 0) && (c3 / c4 <= 0))
					{
						return  true;
					}
				}
			}
		}
		return false;
	}

	bool mPickToolClass::IsLineIntersectionWithRound(QVector2D p1, QVector2D p2, QVector2D center, float radius)
	{
		QVector2D direction = p2 - p1;
		float length = direction.length();
		direction /= length;
		QVector2D dist = center - p1;
		float proj = QVector2D::dotProduct(direction, dist);
		if (proj < 0 || proj > length)
		{
			return false;
		}
		QVector2D closestPoint = p1 + proj * direction;
		float d = (closestPoint - center).length();
		return (d < radius);
	}

	bool mPickToolClass::IsPointInMesh(const QPoint & pos, const QVector<QVector2D> &Line1, MeshType meshtype)
	{
		int Intersection_times = 0;
		QVector2D ap1, bp1, bp2;
		const QVector<QVector<int>> &list = QuadPointInMeshVector.value(meshtype);
		
		ap1 = QVector2D(pos.x(), pos.y());
		for (int j = 0; j < list.size(); j++)
		{
			for (int k = 0; k < list.at(j).size(); k += 2)
			{
				bp1 = Line1.at(list.at(j).at(k));
				bp2 = Line1.at(list.at(j).at(k + 1));
				//判断点是否在另一个多边形的内部
				double tempx = (bp1.x() - bp2.x()) / (bp1.y() - bp2.y())*(ap1.y() - bp2.y()) + bp2.x();
				if (((bp1.y() < ap1.y() && ap1.y() < bp2.y()) || (bp2.y() < ap1.y() && ap1.y() < bp1.y())) && tempx >= ap1.x())
				{
					Intersection_times++;//被裁减的点在裁剪多边形一条线段的左边
				}
			}
			if (Intersection_times % 2 != 0)
			{
				return true;
			}
		}
		return false;
	}

	#else

	bool mPickToolClass::IsLineIntersectionWithQuad(const QVector<QVector2D> &Line, const QVector<QVector2D> &Quad, MeshType meshtype)
	{
		bool isintersection{ false };
		QVector<int> list{};
		QVector2D ap1, ap2, bp1, bp2;

		if (meshtype == MeshBeam)
		{
			list = QVector<int>{ 0,1 };//取出线网格的线段的索引序号
		}
		else if (meshtype == MeshTri)
		{
			list = QVector<int>{ 0,1,1,2,2,0 };//取出四面体的线段的索引序号
		}
		else if (meshtype == MeshQuad)
		{
			list = QVector<int>{ 0,1,1,2,2,3,3,0 };//取出四边形的线段的索引序号
		}
		else if (meshtype == MeshTet)
		{
			list = QVector<int>{ 0,1,0,2,0,3,1,2,1,3,2,3 };//取出四面体的线段的索引序号
		}
		else if (meshtype == MeshWedge)
		{
			list = QVector<int>{ 0,1,1,2,2,0,3,4,4,5,5,3,0,3,1,4,2,5 };//取出三菱柱的线段的索引序号
		}
		else if (meshtype == MeshHex)
		{
			list = QVector<int>{ 0,1,1,5,5,4,4,0,2,3,3,7,7,6,6,2,1,2,5,6,4,7,0,3 };//取出六面体的线段的索引序号
		}
		else if (meshtype == MeshPyramid)
		{
			list = QVector<int>{ 0,1,1,2,2,3,3,0,0,4,1,4,2,4,3,4 };//取出三菱柱的线段的索引序号
		}
		for (int i = 0; i < list.size(); i += 2)
		{
			ap1 = Line.at(list.at(i));
			ap2 = Line.at(list.at(i + 1));
			if (!isintersection)//避免重复判断
			{
				for (int k = 0; k < Quad.size(); ++k)
				{
					if (k == Quad.size() - 1)
					{
						bp1 = Quad.at(k);
						bp2 = Quad.at(0);
					}
					else
					{
						bp1 = Quad.at(k);
						bp2 = Quad.at(k + 1);
					}
					QVector2D v = bp2 - bp1;
					QVector2D v1 = bp2 - ap1;
					QVector2D v2 = bp2 - ap2;
					float c1 = v.x()*v1.y() - v.y()*v1.x();
					float c2 = v.x()*v2.y() - v.y()*v2.x();

					v = ap2 - ap1;
					QVector2D v3 = ap2 - bp1;
					QVector2D v4 = ap2 - bp2;
					float c3 = v.x()*v3.y() - v.y()*v3.x();
					float c4 = v.x()*v4.y() - v.y()*v4.x();
					if ((c1 / c2 <= 0) && (c3 / c4 <= 0))
					{
						return  true;
					}
				}
			}
		}
		return false;
	}

	bool mPickToolClass::IsQuadPointInMesh(const QPoint & pos, const QVector<QVector2D> &Line1, MeshType meshtype)
	{
		int Intersection_times = 0;
		QVector2D ap1, bp1, bp2;
		QVector<QVector<int>> list;
		if (meshtype == MeshTri)
		{
			list = QVector<QVector<int>>{ QVector<int>{0,1,1,2,2,0} };//取出三角形的线段的索引序号
		}
		else if (meshtype == MeshQuad)
		{
			list = QVector<QVector<int>>{ QVector<int>{ 0,1,1,2,2,3,3,0 } };//取出四边形的线段的索引序号
		}
		else if (meshtype == MeshTet)
		{
			list = QVector<QVector<int>>{ QVector<int>{ 0,1,1,2,2,0},QVector<int>{2,3,3,1,1,2},QVector<int>{3,0,0,2,2,3},QVector<int>{1,0,0,3,3,1} };//取出四面体的线段的索引序号
		}
		else if (meshtype == MeshWedge)
		{
			list = QVector<QVector<int>>{ QVector<int>{ 0,1,1,2,2,0}, QVector<int>{3,4,4,5,5,3},QVector<int>{0,2,2,5,5,3,3,0 },QVector<int>{1,4,4,5,5,2,2,1 },QVector<int>{0,3,3,4,4,1,1,0 } };//取出三菱柱的线段的索引序号
		}
		else if (meshtype == MeshHex)
		{
			list = QVector<QVector<int>>{ QVector<int>{ 0,1,1,5,5,4,4,0},QVector<int>{2,3,3,7,7,6,6,2},QVector<int>{1,2,2,6,6,5,5,1},QVector<int>{4,5,5,6,6,7,7,4},QVector<int>{0,4,4,7,7,3,3,0},QVector<int>{0,3,3,2,2,1,1,0} };//取出六面体的线段的索引序号
		}
		else if (meshtype == MeshPyramid)
		{
			list = QVector<QVector<int>>{ QVector<int>{ 0,1,1,2,2,3,3,0},QVector<int>{0,1,1,4,4,0},QVector<int>{1,2,2,4,4,1},QVector<int>{2,3,3,4,4,2},QVector<int>{3,0,0,4,4,3} };//取出四棱锥的线段的索引序号
		}
		ap1 = QVector2D(pos.x(), pos.y());
		for (int j = 0; j < list.size(); j++)
		{
			for (int k = 0; k < list.at(j).size(); k += 2)
			{
				bp1 = Line1.at(list.at(j).at(k));
				bp2 = Line1.at(list.at(j).at(k + 1));
				//判断点是否在另一个多边形的内部
				double tempx = (bp1.x() - bp2.x()) / (bp1.y() - bp2.y())*(ap1.y() - bp2.y()) + bp2.x();
				if (((bp1.y() < ap1.y() && ap1.y() < bp2.y()) || (bp2.y() < ap1.y() && ap1.y() < bp1.y())) && tempx >= ap1.x())
				{
					Intersection_times++;//被裁减的点在裁剪多边形一条线段的左边
				}
			}
			if (Intersection_times % 2 != 0)
			{
				return true;
			}
		}
		return false;
	}

	#endif
	bool mPickToolClass::IsMeshPointInQuad(const QVector<QVector2D> &Line1, int centerX, int centerY, int boxW, int boxH)
	{
		for (int i = 0; i < Line1.size(); ++i)
		{
			if (fabs(Line1.at(i).x() - centerX) <= boxW && fabs(Line1.at(i).y() - centerY) <= boxH)
			{
				return true;
			}
		}
		return false;
	}

	bool mPickToolClass::IsPointInQuad(QVector2D point, QVector2D boxCenter, QVector2D boxXY_2)
	{
		if (qAbs(point.x() - boxCenter.x()) < boxXY_2.x() && qAbs(point.y() - boxCenter.y()) < boxXY_2.y())
		{
			return true;
		}
		return false;
	}

	bool mPickToolClass::IsPointInQuad(const QVector<QVector2D> &Line1, QVector2D boxCenter, QVector2D boxXY_2)
	{
		for (auto point : Line1)
		{
			if (IsPointInQuad(point, boxCenter, boxXY_2))
			{
				return true;
			}
		}
		return false;
	}

	bool mPickToolClass::IsAllPointInQuad(const QVector<QVector2D> &Line1, QVector2D boxCenter, QVector2D boxXY_2)
	{
		for (auto point : Line1)
		{
			if (!IsPointInQuad(point, boxCenter, boxXY_2))
			{
				return false;
			}
		}
		return true;
	}

	bool mPickToolClass::IsPointInPolygon(QVector2D point, QVector2D boxCenter, const QVector<QVector2D> &polygons)
	{
		int n = polygons.size();
		//QVector<QVector2D> sorted_polygons = polygons;
		//std::sort(sorted_polygons.begin(), sorted_polygons.end(), [&boxCenter](QVector2D a, QVector2D b) {
		//	return atan2(a.y() - boxCenter.y(), a.x() - boxCenter.x()) < atan2(b.y() - boxCenter.y(), b.x() - boxCenter.x());
		//});
		int cnt = 0;
		for (int i = 0; i < n; i++)
		{
			QVector2D p1 = polygons[i];
			QVector2D p2 = polygons[(i + 1) % n];
			if ((p1.y() > point.y()) != (p2.y() > point.y()))
			{
				float x = (point.y() - p1.y()) * (p2.x() - p1.x()) / (p2.y() - p1.y()) + p1.x();
				if (x >= point.x())
				{
					cnt++;
				}
			}
		}
		return (cnt % 2 == 1);
	}

	bool mPickToolClass::IsPointInPolygon(const QVector<QVector2D> &Line1, QVector2D boxCenter, const QVector<QVector2D> &polygons)
	{
		for (auto point : Line1)
		{
			if (IsPointInPolygon(point, boxCenter, polygons))
			{
				return true;
			}
		}
		return false;
	}

	bool mPickToolClass::IsAllPointInPolygon(const QVector<QVector2D> &Line1, QVector2D boxCenter, const QVector<QVector2D> &polygons)
	{
		for (auto point : Line1)
		{
			if (!IsPointInPolygon(point, boxCenter, polygons))
			{
				return false;
			}
		}
		return true;
	}

	bool mPickToolClass::IsLineIntersectionWithCircle(const QVector<QVector2D> &Line1, QVector2D circleCenter, double radius)
	{
		bool isintersection{ false };
		const QVector<QVector<int>> &list = QuadPointInMeshVector.value(MeshHex);
		QVector2D ap1, ap2, bp1, bp2;
		QVector<QVector2D> res;
		for (int j = 0; j < list.size(); j++)
		{
			ap1 = Line1.at(list.at(j).at(0));
			ap2 = Line1.at(list.at(j).at(1));

			QVector2D line = (ap2 - ap1).normalized();
			for (int k = 2; k < list.at(j).size(); k += 2)
			{
				bp1 = Line1.at(list.at(j).at(k));
				bp2 = Line1.at(list.at(j).at(k + 1));

				QVector2D line1 = (bp2 - bp1).normalized();
				float sintheta = line.x() * line1.y() - line1.x() * line.y();//叉乘

				res.append(bp1 + (line - line1) * radius / sintheta);
				line = (bp2 - bp1).normalized();
			}

			bp1 = Line1.at(list.at(j).at(0));
			bp2 = Line1.at(list.at(j).at(1));

			QVector2D line1 = (bp2 - bp1).normalized();
			float sintheta = line.x() * line1.y() - line1.x() * line.y();//叉乘

			res.append(bp1 + (line - line1) * radius / sintheta);
		}
		ap1 = circleCenter;
		int Intersection_times = 0;
		for (int i = 0; i < 6; i++)
		{
			for (int k = 0; k < 3; k ++)
			{
				bp1 = res.at(4 * i + k);
				bp2 = res.at(4 * i + k + 1);
				//判断点是否在另一个多边形的内部
				double tempx = (bp1.x() - bp2.x()) / (bp1.y() - bp2.y())*(ap1.y() - bp2.y()) + bp2.x();
				if (((bp1.y() < ap1.y() && ap1.y() < bp2.y()) || (bp2.y() < ap1.y() && ap1.y() < bp1.y())) && tempx >= ap1.x())
				{
					Intersection_times++;//被裁减的点在裁剪多边形一条线段的左边
				}
			}
			bp1 = res.at(4 * i + 3);
			bp2 = res.at(4 * i + 0);
			//判断点是否在另一个多边形的内部
			double tempx = (bp1.x() - bp2.x()) / (bp1.y() - bp2.y())*(ap1.y() - bp2.y()) + bp2.x();
			if (((bp1.y() < ap1.y() && ap1.y() < bp2.y()) || (bp2.y() < ap1.y() && ap1.y() < bp1.y())) && tempx >= ap1.x())
			{
				Intersection_times++;//被裁减的点在裁剪多边形一条线段的左边
			}
			if (Intersection_times % 2 != 0)
			{
				return true;
			}
		}
		

		return false;
	}

	bool mPickToolClass::IsPointInRound(QVector3D point, QVector3D center, QVector3D direction, float r)
	{
		if (point.distanceToLine(center, direction) <= r)
		{
			return true;
		}
		return false;
	}

	bool mPickToolClass::IsPointInRound(QVector2D point, QVector2D center, float r)
	{
		if (point.distanceToPoint(center) <= r)//判断包围盒的角点是否在圆内
		{
			return true;
		}
		return false;
	}

	bool mPickToolClass::IsPointInRound(const QVector<QVector2D> &Line1, QVector2D center, float r)
	{
		for (auto point : Line1)
		{
			if (IsPointInRound(point, center, r))//判断包围盒的角点是否在圆内
			{
				return true;
			}
		}
		return false;
	}

	bool mPickToolClass::IsAllPointInRound(const QVector<QVector2D> &Line1, QVector2D center, float r)
	{
		for (auto point : Line1)
		{
			if (!IsPointInRound(point, center, r))//判断包围盒的角点是否在圆内
			{
				return false;
			}
		}
		return true;
	}

	bool mPickToolClass::IsMeshCenterInPickQuad(const QPoint& pos, const QVector<QVector2D> &Line1)
	{
		QVector2D center;
		for (int i = 0; i < Line1.size(); ++i)
		{
			center += Line1.at(i);
		}
		center = center / Line1.size();

		if (fabs(center.x() - pos.x()) <= 3 && fabs(center.y() - pos.y()) <= 3)
		{
			return true;
		}
		return false;
	}

	float mPickToolClass::CaculatePointInTriDepth(QVector2D ap1, QVector2D ap2, QVector2D ap3, QPoint pos,float depth1, float depth2, float depth3)
	{
		//根据三角形面积计算被点击的点的深度值
		//float temp = (ap2.y() - ap3.y())*(ap1.x() - ap3.x()) + (ap3.x() - ap2.x())*(ap1.y() - ap3.y());
		//float a1 = ((ap2.y() - ap3.y())*(pos.x() - ap3.x()) + (ap3.x() - ap2.x())*(pos.y() - ap3.y()))/ temp;
		//float a2 = ((ap3.y() - ap1.y())*(pos.x() - ap3.x()) + (ap1.x() - ap3.x())*(pos.y() - ap3.y())) / temp;
		//float a3 = 1 - a1 - a2;
		//float depth = a1 * depth1 + a2 * depth2 + a3 * depth3;

		float A = ap1.x()*ap2.y() - ap1.x()*ap3.y() + ap2.x()*ap3.y() - ap2.x()*ap1.y() + ap3.x()*ap1.y() - ap3.x()*ap2.y();
		float A1 = ap1.x()*ap2.y() - ap1.x()*pos.y() + ap2.x()*pos.y() - ap2.x()*ap1.y() + pos.x()*ap1.y() - pos.x()*ap2.y();
		float A2 = ap1.x()*pos.y() - ap1.x()*ap3.y() + pos.x()*ap3.y() - pos.x()*ap1.y() + ap3.x()*ap1.y() - ap3.x()*pos.y();
		float A3 = pos.x()*ap2.y() - pos.x()*ap3.y() + ap2.x()*ap3.y() - ap2.x()*pos.y() + ap3.x()*pos.y() - ap3.x()*ap2.y();
		float depth = (A1 * depth3 + A2 * depth2 + A3 * depth1) / A;
		return depth;
	}

	float mPickToolClass::CaculatePointInLineDepth(QVector2D ap1, QVector2D ap2, QPoint pos, float depth1, float depth2)
	{
		float distance = ap1.distanceToPoint(ap2);
		float distance1 = ap1.distanceToPoint(QVector2D(pos));
		float distance2 = ap2.distanceToPoint(QVector2D(pos));
		float depth = (depth1 * distance2 + depth2 * distance1)/distance;
		return depth;
	}
}
