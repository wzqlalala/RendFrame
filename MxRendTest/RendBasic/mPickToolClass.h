#pragma once
#include "rendbasic_global.h"

#include "mBasicEnum.h"

#include <QObject>

namespace MViewBasic
{
	class RENDBASIC_EXPORT mPickToolClass : public QObject
	{
	public:
		static QHash<MBasicFunction::MeshType, QVector<int>> LineIntersectionWithQuadVector;

		static QHash<MBasicFunction::MeshType, QVector<QVector<int>>> QuadPointInMeshVector;

		/*
		 * 判断一个线段和一个矩形选择框是否相交
		 */
		static bool IsLineIntersectionWithQuad(const QVector<QVector2D> &Line1, const QVector<QVector2D> &Quad, MBasicFunction::MeshType meshtype);

		/*
		* 判断一个线段和一个圆形选择框是否相交
		*/
		static bool IsLineIntersectionWithRound(QVector2D p1, QVector2D p2, QVector2D center, float radius);

		/*
		 * 判断点是否在网格内部
		 */
		static bool IsPointInMesh(const QVector2D & pos, const QVector<QVector2D> &Line1, MBasicFunction::MeshType meshtype);

		/*
		 * 判断网格的点是否在拾取矩形框内部
		 */
		static bool IsMeshPointInQuad(const QVector<QVector2D> &Line1, int centerX, int centerY, int boxW, int boxH);

		/*
		 * 判断点是否在拾取矩形框内部
		 */
		static bool IsPointInQuad(QVector2D point, QVector2D boxCenter, QVector2D boxXY_2);
		static bool IsPointInQuad(const QVector<QVector2D> &Line1, QVector2D boxCenter, QVector2D boxXY_2);//部分在内部即可
		static bool IsAllPointInQuad(const QVector<QVector2D> &Line1, QVector2D boxCenter, QVector2D boxXY_2);//全部在内部

		/*
		 * 判断点是否在拾取多边形框内部
		 */
		static bool IsPointInPolygon(QVector2D point, QVector2D boxCenter, const QVector<QVector2D> &polygons);
		static bool IsPointInPolygon(const QVector<QVector2D> &Line1, QVector2D boxCenter, const QVector<QVector2D> &polygons);//部分在内部即可
		static bool IsAllPointInPolygon(const QVector<QVector2D> &Line1, QVector2D boxCenter, const QVector<QVector2D> &polygons);//全部在内部

		/*
		* 判断网格的多边形是否和圆形选择框相交
		*/
		static bool IsTriIntersectionWithCircle(const QVector<QVector2D> &Line1, QVector2D circleCenter, double radius);

		/*判断包围盒是否与圆形选择框相交*/
		static bool IsAABBIntersectionWithCircle(const QVector<QVector2D> &Line1, QVector2D circleCenter, double radius);

		/*
		* 判断点是否全在圆形内部
		*/
		static bool IsPointInRound(QVector3D point, QVector3D center, QVector3D direction, float r);
		static bool IsPointInRound(QVector2D point, QVector2D center, float r);
		static bool IsPointInRound(const QVector<QVector2D> &Line1, QVector2D center, float r);//部分在内部即可
		static bool IsAllPointInRound(const QVector<QVector2D> &Line1, QVector2D center, float r);//全部在内部

		/*
		 * 判断网格的形心和点选矩形框的位置是否满足拾取要求
		 */
		static bool IsMeshCenterInPickQuad(const QPoint & pos, const QVector<QVector2D> &Line1);

		/*
		 * 计算三角形内部点的深度值
		 */
		static float CaculatePointInTriDepth(QVector2D ap1, QVector2D ap2, QVector2D ap3, QPoint pos, float depth1, float depth2, float depth3);

		/*
		 * 计算线内部点的深度值
		 */
		static float CaculatePointInLineDepth(QVector2D ap1, QVector2D ap2, QVector2D pos, float depth1, float depth2);

		//判断射线与三角形是否相交
		static bool rayTriangleIntersect(const QVector3D & orig, const QVector3D & dir, const QVector<QVector3D>& tri, float uv[2], float & t);

		//判断射线与四边形是否相交
		static bool rayQuadIntersect(const QVector3D & orig, const QVector3D & dir, const QVector<QVector3D>& tri, float uv[2], float & t);
	};
}
