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
		 * �жϾ��ο�ĵ��Ƿ��������ڲ�
		 */
		static bool IsQuadPointInMesh(const QPoint & pos, QVector<QVector2D> Line1, MBasicFunction::MeshType meshtype);

		/*
		 * �ж�����ĵ��Ƿ���ʰȡ���ο��ڲ�
		 */
		static bool IsMeshPointInQuad(QVector<QVector2D> Line1, int centerX, int centerY, int boxW, int boxH);

		/*
		* �ж϶�����Ƿ��Բ��ѡ����ཻ
		*/
		static bool IsLineIntersectionWithCircle(QVector<QVector2D> Line1, QVector2D circleCenter, double radius);

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
