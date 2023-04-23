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
using namespace std;
namespace MDataGeo
{
	class mGeoPickData1;
	class mGeoPartData1;
	class mGeoModelData1;
	class mBasePick
	{
	public:

		mBasePick(QMatrix4x4 pvm, int w, int h) { _pvm = pvm; _width = w; _height = h; };

		void getAABBAndPickToMeshData(Space::SpaceTree * spaceTree, QVector<MDataPost::mPostMeshData1*>& meshAll, QVector<MDataPost::mPostMeshData1*>& meshContain);

		virtual bool getPickIsIntersectionWithAABB(Space::SpaceTree * spaceTree) = 0;

		virtual bool get2DAnd3DMeshCenterIsInPick(QVector3D pointCenter) = 0;//0ά��2ά��3ά

		virtual bool get1DMeshIsInPick(QVector<QVector3D> vertexs) = 0;//1ά

		virtual bool isIntersectionAABBAndPick(QVector<QVector2D> ap) = 0;//�ж��Ƿ��ཻ

		virtual bool isAABBPointIsAllInPick(QVector<QVector2D> ap) = 0;//�жϵ��Ƿ�ȫ��

		//��AABB�İ˸�����ת��Ϊ��ά��Ļ�ĵ�
		QVector<QVector2D> getAABBToScreenVertex(QVector3D minEdge, QVector3D maxEdge);
		//����Ļ����ת��Ϊ��������
		QVector3D ScreenvertexToWorldvertex(QVector3D vertex);
		//����������ת��Ϊ��Ļ����
		QVector2D WorldvertexToScreenvertex(QVector3D Worldvertex);
		QVector<QVector2D> WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs);
		//����������ת��Ϊ��Ļ���겢�ҷ����������ֵ
		QVector2D WorldvertexToScreenvertex(QVector3D Worldvertex, float &depth);	
		void WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs, QVector<QVector2D> &Screenvertexs, std::set<float> &depths);

	protected:
		QMatrix4x4 _pvm;
		int _width, _height;
		MViewBasic::MultiplyPickMode _multiplyPickMode;

	};


	class mQuadPick :public mBasePick
	{
	public:
		mQuadPick(QMatrix4x4 pvm, int w, int h, QVector<QVector2D> multiQuad) :mBasePick(pvm, w, h), _multiQuad(multiQuad)
		{
			_center = (_multiQuad.first() + _multiQuad.last()) / 2.0;
			_boxXY_2 = _multiQuad.first() - _center; _boxXY_2[0] = qAbs(_boxXY_2[0]); _boxXY_2[1] = qAbs(_boxXY_2[1]);
		};

		bool getPickIsIntersectionWithAABB(Space::SpaceTree * spaceTree) override;

		bool get2DAnd3DMeshCenterIsInPick(QVector3D pointCenter) override;//0ά��2ά��3ά

		bool get1DMeshIsInPick(QVector<QVector3D> vertexs) override;//1ά

		bool isIntersectionAABBAndPick(QVector<QVector2D> ap) override;

		bool isAABBPointIsAllInPick(QVector<QVector2D> ap) override;//�жϵ��Ƿ�ȫ��

	protected:
		QVector<QVector2D> _multiQuad;

		QVector2D _center, _boxXY_2;

	};

	class mPolygonPick :public mBasePick
	{
	public:
		mPolygonPick(QMatrix4x4 pvm, int w, int h, QVector<QVector2D> multiQuad):mBasePick(pvm, w, h),_multiQuad(multiQuad)
		{
			_center = (_multiQuad.first() + _multiQuad.last()) / 2.0;
		};

		bool getPickIsIntersectionWithAABB(Space::SpaceTree * spaceTree) override;

		bool get2DAnd3DMeshCenterIsInPick(QVector3D pointCenter) override;//0ά��2ά��3ά

		bool get1DMeshIsInPick(QVector<QVector3D> vertexs) override;//1ά

		bool isIntersectionAABBAndPick(QVector<QVector2D> ap) override;

		bool isAABBPointIsAllInPick(QVector<QVector2D> ap) override;//�жϵ��Ƿ�ȫ��

	protected:
		QVector<QVector2D> _multiQuad;

		QVector2D _center;

	};

	class mRoundPick :public mBasePick
	{
	public:
		mRoundPick(QMatrix4x4 pvm, int w, int h, QVector2D p1, QVector2D p2, QVector3D centerDirection)
			:mBasePick(pvm, w, h)
		{
			_screenCenter = (p1 + p2)/2.0;
			QVector3D Point = ScreenvertexToWorldvertex(p2);//���Բ��һ�������
			_centerPoint = ScreenvertexToWorldvertex(_screenCenter);//���Բ������
			_radius = _centerPoint.distanceToPoint(Point);
			_screenRadius = _screenCenter.distanceToPoint(p1);
			_centerDirection = centerDirection;
		};

		bool getPickIsIntersectionWithAABB(Space::SpaceTree * spaceTree) override;

		bool get2DAnd3DMeshCenterIsInPick(QVector3D pointCenter) override;//0ά��2ά��3ά

		bool get1DMeshIsInPick(QVector<QVector3D> vertexs) override;//1ά

		bool isIntersectionAABBAndPick(QVector<QVector2D> ap) override;

		bool isAABBPointIsAllInPick(QVector<QVector2D> ap) override;//�жϵ��Ƿ�ȫ��

	protected:
		QVector3D _centerPoint; QVector3D _centerDirection; double _radius; //�ռ�
		QVector2D _screenCenter; double _screenRadius;//��Ļ
	};
	class RENDDATA_EXPORT mPreGeoPickThread : public QObject
	{
		Q_OBJECT

	public:
		/*
		* ���ƹ��캯��
		*/
		mPreGeoPickThread(mGeoModelData1 *geoModelData, mGeoPickData1 *pickData);

		/*
		* ��������
		*/
		~mPreGeoPickThread();

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
		 * ���þ���
		 */
		void setMatrix(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model);

		/*
		* ���þ���
		*/
		void setMatrix(QMatrix4x4 pvm);

		/*
		 * ���õ�ѡλ��
		 */
		void setLocation(const QPoint & pos, float depth);

		/*
		* ����λ��(���λ��߶���ο�ѡ)
		*/
		void setLocation(QVector<QVector2D> pickQuad, QVector3D direction = QVector3D());

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
		void doSoloPick(mGeoPartData1 *partData);
		void doMultiplyPick(mGeoPartData1 *partData);

		/*
		 * ��ѡ
		 */
		void SoloPickGeoPoint(mGeoPartData1 *partData);
		void SoloPickGeoLine(mGeoPartData1 *partData);
		void SoloPickGeoFace(mGeoPartData1 *partData);
		void SoloPickGeoSolid(mGeoPartData1 *partData);
		void SoloPickGeoPart(mGeoPartData1 *partData);
		void SoloPickVertexOnGeoLine(mGeoPartData1 *partData);
		void SoloPickVertexOnGeoFace(mGeoPartData1 *partData);
		void SoloPickGeoPointByPart(mGeoPartData1 *partData);
		void SoloPickGeoLineByPart(mGeoPartData1 *partData);
		void SoloPickGeoFaceByPart(mGeoPartData1 *partData);
		void SoloPickGeoSolidByPart(mGeoPartData1 *partData);

		//���ο�ѡʰȡ
		void MultiplyPickGeoLine(mGeoPartData1 *partData);
		void MultiplyPickGeoFace(mGeoPartData1 *partData);
		void MultiplyPickGeoSolid(mGeoPartData1 *partData);
		void MultiplyPickGeoPoint(mGeoPartData1 *partData);
		void MultiplyPickGeoPart(mGeoPartData1 *partData);
		void MultiplyPickGeoPointByPart(mGeoPartData1 *partData);
		void MultiplyPickGeoLineByPart(mGeoPartData1 *partData);
		void MultiplyPickGeoFaceByPart(mGeoPartData1 *partData);
		void MultiplyPickGeoSolidByPart(mGeoPartData1 *partData);
		
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
		* �жϽڵ��Ƿ�ƽ��ü�,������ʰȡ,true�������ü��ˣ�����ʰȡ��false����û�б��ü�������ʰȡ
		*/
		bool isVertexCuttingByPlane(QVector3D vertex);

		/*
		* �ж�һ����Ԫ�����нڵ��Ƿ�ƽ��ü�����ֻ��ȫ���ڵ㶼û�б��ü��˲���ʰȡ��true�������ֱ��ü��ˣ�����ʰȡ��false����ȫ��û�б��ü�������ʰȡ
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

		//ʰȡ������
		MViewBasic::PickFilter *_pickFilter;

		//ʰȡģʽ
		MViewBasic::PickMode _pickMode;

		//��ѡʰȡģʽ
		MViewBasic::MultiplyPickMode _multiplyPickMode;

		//���ڴ�С
		int _Win_WIDTH, _Win_HEIGHT;

		//��ɫ����ͼ����
		QMatrix4x4 _projection, _view, _model, _pvm;

		/*
		* ʰȡ����
		*/
		mGeoPickData1 *_pickData;

		//��������
		mGeoModelData1 *_geoModelData;

		/*
		 * ��ѡλ��
		 */
		QPoint _pos;
		float _depth;

		/*********/
		std::shared_ptr<mBasePick> _pick;
		/*
		 * ��ѡ����
		 */
		QVector<QVector2D> soloQuad;

		/*
		 * �Ƿ����
		 */
		bool _isfinished;


	};
}