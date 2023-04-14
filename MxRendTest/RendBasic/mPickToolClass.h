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
		 * �ж�һ���߶κ�һ������ѡ����Ƿ��ཻ
		 */
		static bool IsLineIntersectionWithQuad(QVector<QVector2D> Line1, QVector<QVector2D> Quad, MBasicFunction::MeshType meshtype);

		/*
		 * �жϵ��Ƿ��������ڲ�
		 */
		static bool IsPointInMesh(const QPoint & pos, QVector<QVector2D> Line1, MBasicFunction::MeshType meshtype);

		/*
		 * �ж�����ĵ��Ƿ���ʰȡ���ο��ڲ�
		 */
		static bool IsMeshPointInQuad(QVector<QVector2D> Line1, int centerX, int centerY, int boxW, int boxH);

		/*
		 * �жϵ��Ƿ���ʰȡ���ο��ڲ�
		 */
		static bool IsPointInQuad(QVector2D point, QVector2D boxCenter, QVector2D boxXY_2);
		static bool IsPointInQuad(QVector<QVector2D> Line1, QVector2D boxCenter, QVector2D boxXY_2);//�������ڲ�����
		static bool IsAllPointInQuad(QVector<QVector2D> Line1, QVector2D boxCenter, QVector2D boxXY_2);//ȫ�����ڲ�

		/*
		 * �жϵ��Ƿ���ʰȡ����ο��ڲ�
		 */
		static bool IsPointInPolygon(QVector2D point, QVector2D boxCenter, QVector<QVector2D> polygons);
		static bool IsPointInPolygon(QVector<QVector2D> Line1, QVector2D boxCenter, QVector<QVector2D> polygons);//�������ڲ�����
		static bool IsAllPointInPolygon(QVector<QVector2D> Line1, QVector2D boxCenter, QVector<QVector2D> polygons);//ȫ�����ڲ�

		/*
		* �ж϶�����Ƿ��Բ��ѡ����ཻ
		*/
		static bool IsLineIntersectionWithCircle(QVector<QVector2D> Line1, QVector2D circleCenter, double radius);

		/*
		* �жϵ��Ƿ�ȫ��Բ���ڲ�
		*/
		static bool IsPointInRound(QVector3D point, QVector3D center, QVector3D direction, float r);
		static bool IsPointInRound(QVector2D point, QVector2D center, float r);
		static bool IsPointInRound(QVector<QVector2D> Line1, QVector2D center, float r);//�������ڲ�����
		static bool IsAllPointInRound(QVector<QVector2D> Line1, QVector2D center, float r);//ȫ�����ڲ�

		/*
		 * �ж���������ĺ͵�ѡ���ο��λ���Ƿ�����ʰȡҪ��
		 */
		static bool IsMeshCenterInPickQuad(const QPoint & pos, QVector<QVector2D> Line1);

		/*
		 * �����������ڲ�������ֵ
		 */
		static float CaculatePointInTriDepth(QVector2D ap1, QVector2D ap2, QVector2D ap3, QPoint pos, float depth1, float depth2, float depth3);

		/*
		 * �������ڲ�������ֵ
		 */
		static float CaculatePointInLineDepth(QVector2D ap1, QVector2D ap2, QPoint pos, float depth1, float depth2);
	};
}
