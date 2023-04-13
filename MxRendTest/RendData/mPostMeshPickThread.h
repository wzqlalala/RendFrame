#pragma once
//�����������
#pragma execution_character_set("utf-8")

#include "renddata_global.h"

#include <QObject>
#include <QMatrix4x4>
#include <QVector>
#include <QVector2D>
#include <QThread>
#include <set>

#include "SpaceTree.h"

#include "mMeshViewEnum.h"

using namespace MViewBasic;
namespace MDataPost
{
	class mOneFrameData1;
	class mPostOneFrameRendData;
	class mPostMeshPickData;
	class mPostMeshPartData1;
	class RENDDATA_EXPORT mPostMeshPickThread : public QObject
	{
		Q_OBJECT

	public:
		/*
		* ���ƹ��캯��
		*/
		mPostMeshPickThread(mPostMeshPickData *postMeshPickData);

		/*
		* ��������
		*/
		~mPostMeshPickThread();

		/*
		* ���õ�ǰ��ʰȡģʽ
		*/
		void setPickMode(PickMode pickMode, MultiplyPickMode multiplyPickMode);

		/*
		* ���õ�ǰ��ʰȡ������
		*/
		void setPickFilter(MViewBasic::PickFilter *pickFilter);

		/*
		 * ��ʼʰȡ
		 */
		void startPick();

		/*
		* ��Ӳ�������
		*/
		void appendPartSpaceTree(QString partName, Space::SpaceTree *spaceTree);

		/*
		* ���õ�ǰ����Ⱦ����
		*/
		void setCurrentFrameRend(mOneFrameData1 * oneFrameData, mPostOneFrameRendData *postOneFrameRendData);

		/*
		* ���õ�ǰ����Ч������ƽ��
		*/
		void setCuttingPlaneNormalVertex(QVector<QPair<QVector3D, QVector3D>> postCuttingNormalVertex);

		/*
		* ����ʰȡ�ĵ�Ԫ������
		*/
		void setPickElementTypeFilter(std::set<int> pickElementTypeFilter);

		/*
		 * ���þ���
		 */
		void setMatrix(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model);

		/*
		* ���þ���
		*/
		void setMatrix(QMatrix4x4 pvm);

		/*
		* ����ͨ���Ƕ�ʰȡ�ĽǶ�
		*/
		void setPickAngleValue(double angle) { _pickAngleValue = angle; };

		/*
		 * ���õ�ѡλ��
		 */
		void setLocation(const QPoint & pos, float depth);

		/*
		* ����λ��(���λ��߶���ο�ѡ)
		*/
		void setLocation(QVector<QVector2D> pickQuad);

		/*
		 * ����Բ�ο�ѡλ��
		 */
		void setLocation(QVector3D centerPoint, QVector3D centerDirection, double radius, QVector2D centerScreenPoint, double screenRadius);

		/*
		 * ���ô��ڴ�С
		 */
		void setWidget(int w, int h);

		/*
		 *	�Ƿ����ʰȡ
		 */
		bool isFinished();

		/*
		 * ʰȡ�������Ϊfalse
		 */
		void setFinished();
	private:
		//ʰȡ������		
		void doSoloPick(QString partName, Space::SpaceTree* spaceTree);
		void doQuadPick(QString partName, Space::SpaceTree* spaceTree);
		void doRoundPick(QString partName, Space::SpaceTree* spaceTree);
		void doPolygonPick(QString partName, Space::SpaceTree* spaceTree);
		void doAnglePick();

		/*
		 * ��ѡ
		 */
		void SoloPickNode(QString partName);
		void SoloPick1DMesh(QString partName);
		void SoloPick2DMesh(QString partName);
		void SoloPickAnyMesh(QString partName);
		void SoloPickMeshFace(QString partName);
		void SoloPickNodeByLineAngle(QString partName);
		void SoloPickNodeByFaceAngle(QString partName);
		void SoloPick1DMeshByAngle(QString partName);
		void SoloPick2DMeshByAngle(QString partName);
		void SoloPickMeshFaceByAngle(QString partName);

		/*
		* ���ο�ѡ
		*/
		void MultiplyPickNode(QString partName, Space::SpaceTree* spaceTree);
		void MultiplyPick1DMesh(QString partName, Space::SpaceTree* spaceTree);
		void MultiplyPick2DMesh(QString partName, Space::SpaceTree* spaceTree);
		void MultiplyPickAnyMesh(QString partName, Space::SpaceTree* spaceTree);
		void MultiplyPickMeshFace(QString partName, Space::SpaceTree* spaceTree);

		/*
		* Բ�ο�ѡ
		*/
		void RoundPickNode(QString partName, Space::SpaceTree* spaceTree);
		void RoundPick1DMesh(QString partName, Space::SpaceTree* spaceTree);
		void RoundPick2DMesh(QString partName, Space::SpaceTree* spaceTree);
		void RoundPickAnyMesh(QString partName, Space::SpaceTree* spaceTree);
		void RoundPickMeshFace(QString partName, Space::SpaceTree* spaceTree);

		
		//ͨ���Ƕ�ʰȡ
		void SoloPickNodeByLineAngle();		
		void SoloPickNodeByFaceAngle();
		void SoloPick1DMeshByAngle();
		void SoloPickMeshLineByAngle();
		void SoloPick2DMeshByAngle();
		void SoloPickMeshFaceByAngle();

		//�ж�һ����Χ�к�һ������ο���ɵ��Ƿ��ཻ
		void getAABBAndQuadToMeshData(Space::SpaceTree *root, QVector<MDataPost::mPostMeshData1*>& meshAll, QVector<MDataPost::mPostMeshData1*>& meshContain);
		//�ж��Ƿ��ཻ
		bool isIntersectionAABBAndQuad(QVector<QVector2D> ap);

		
		//�жϵ�ѡ�Ƿ�ʰȡ���ò���,���ҷ����������ֵ
		
		bool IsSoloPickMeshPart(MDataPost::mPostMeshPartData1 *meshPartData, float &depth);

		
		//�жϿ�ѡ�Ƿ�ʰȡ���ò���
		bool isMultiplyPickMeshPart(MDataPost::mPostMeshPartData1 *meshPartData);

		
		//����������ת��Ϊ��Ļ����
		QVector2D WorldvertexToScreenvertex(QVector3D Worldvertex);

		QVector3D ScreenvertexToWorldvertex(QVector3D vertex);
		
		//����������ת��Ϊ��Ļ���겢�ҷ����������ֵ
		QVector2D WorldvertexToScreenvertex(QVector3D Worldvertex, float &depth);

		/*
		 * ����������ת��Ϊ��Ļ���겢�ҷ����������ֵ
		 */
		void WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs, QVector<QVector2D> &Screenvertexs, std::set<float> &depths);

		/*
		* �жϽڵ��Ƿ�ƽ��ü�,������ʰȡ,true�����ü��ˣ�����ʰȡ��false����û�б��ü�������ʰȡ
		*/
		bool isVertexCuttingByPlane(QVector3D vertex);

		/*
		* �ж�һ����Ԫ�����нڵ��Ƿ�ƽ��ü�����ֻ��ȫ���ڵ㶼û�б��ü��˲���ʰȡ��true�����ֱ��ü��ˣ�����ʰȡ��false����ȫ��û�б��ü�������ʰȡ
		*/
		bool isVertexCuttingByPlane(QVector<QVector3D> vertexs);

		//��ȡ����
		QVector3D getCenter(QVector<QVector3D> vertexs);


	signals:
		/*
		 * �����ź����
		 */
		void finishedPickSig();


	public slots:


	private:
		//�߳�
		QThread *_thread;

		//ʰȡ������
		MViewBasic::PickFilter *_pickFilter;

		//ʰȡģʽ
		MViewBasic::PickMode _pickMode;

		//��ѡʰȡģʽ
		MViewBasic::MultiplyPickMode _multiplyPickMode;

		//ʰȡ�ĵ�Ԫ���͹�����
		std::set<int> _pickElementTypeFilter;

		//ʰȡ�Ƕ�
		float _pickAngleValue = 60;

		//���ڴ�С
		int _Win_WIDTH, _Win_HEIGHT;

		//��ɫ����ͼ����
		QMatrix4x4 _projection, _view, _model, _pvm;

		/*
		* ��ǰ֡��ģ������
		*/
		MDataPost::mOneFrameData1 *_oneFrameData;

		/*
		* ��ǰ֡�ı�������
		*/
		mPostOneFrameRendData *_oneFrameRendData;

		/*
		* ��ǰ֡������ķ������͵�
		*/
		QVector<QPair<QVector3D,QVector3D>> _postCuttingNormalVertex;

		/*
		* ʰȡ����
		*/
		mPostMeshPickData *_postMeshPickData;

		/*
		 * ģ������
		 */
		QHash<QString, Space::SpaceTree*> _partSpaceTrees;

		/*
		 * ��ѡλ��
		 */
		QPoint _pos;
		float _depth;
		/*
		 * ��ѡ����
		 */
		QVector<QVector2D> soloQuad;

		//��ѡ����

		/*
		 * ���κͶ���ο�ѡλ��
		 */
		QVector<QVector2D> multiQuad{};

		/*
		* Բ�ο�ѡλ��
		*/
		QVector3D _centerPoint; QVector3D _centerDirection; double _radius; QVector2D _centerScreenPoint; double _screenRadius;

		//���ο�ѡ���ĺ�������ȺͰ���߶�
		QVector2D _centerBox, _boxXY_2;

		/*
		 * �Ƿ����
		 */
		bool _isfinished;


	};
}
