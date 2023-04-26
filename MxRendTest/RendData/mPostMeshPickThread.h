#pragma once
//解决中文乱码
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
	class mBasePick
	{
	public:

		mBasePick(QMatrix4x4 pvm, int w, int h) { _pvm = pvm; _width = w; _height = h; };

		void getAABBAndPickToMeshData(Space::SpaceTree * spaceTree, QVector<MDataPost::mPostMeshData1*>& meshAll, QVector<MDataPost::mPostMeshData1*>& meshContain);

		virtual bool getPickIsIntersectionWithAABB(Space::SpaceTree * spaceTree) = 0;

		virtual bool get2DAnd3DMeshCenterIsInPick(QVector3D pointCenter) = 0;//0维和2维和3维

		virtual bool get1DMeshIsInPick(QVector<QVector3D> vertexs) = 0;//1维

		virtual bool isIntersectionAABBAndPick(QVector<QVector2D> ap) = 0;//判断是否相交

		virtual bool isAABBPointIsAllInPick(QVector<QVector2D> ap) = 0;//判断点是否全部

		//将AABB的八个顶点转化为二维屏幕的点
		QVector<QVector2D> getAABBToScreenVertex(QVector3D minEdge, QVector3D maxEdge);
		//将屏幕坐标转化为世界坐标
		QVector3D ScreenvertexToWorldvertex(QVector3D vertex);
		//将世界坐标转化为屏幕坐标
		QVector2D WorldvertexToScreenvertex(QVector3D Worldvertex);
		QVector<QVector2D> WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs);
		//将世界坐标转化为屏幕坐标并且返回他的深度值
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

		bool get2DAnd3DMeshCenterIsInPick(QVector3D pointCenter) override;//0维和2维和3维

		bool get1DMeshIsInPick(QVector<QVector3D> vertexs) override;//1维

		bool isIntersectionAABBAndPick(QVector<QVector2D> ap) override;

		bool isAABBPointIsAllInPick(QVector<QVector2D> ap) override;//判断点是否全部

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

		bool get2DAnd3DMeshCenterIsInPick(QVector3D pointCenter) override;//0维和2维和3维

		bool get1DMeshIsInPick(QVector<QVector3D> vertexs) override;//1维

		bool isIntersectionAABBAndPick(QVector<QVector2D> ap) override;

		bool isAABBPointIsAllInPick(QVector<QVector2D> ap) override;//判断点是否全部

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
			QVector3D Point = ScreenvertexToWorldvertex(p2);//算出圆上一点的坐标
			_centerPoint = ScreenvertexToWorldvertex(_screenCenter);//算出圆心坐标
			_radius = _centerPoint.distanceToPoint(Point);
			_screenRadius = _screenCenter.distanceToPoint(p1);
			_centerDirection = centerDirection;
		};

		bool getPickIsIntersectionWithAABB(Space::SpaceTree * spaceTree) override;

		bool get2DAnd3DMeshCenterIsInPick(QVector3D pointCenter) override;//0维和2维和3维

		bool get1DMeshIsInPick(QVector<QVector3D> vertexs) override;//1维

		bool isIntersectionAABBAndPick(QVector<QVector2D> ap) override;

		bool isAABBPointIsAllInPick(QVector<QVector2D> ap) override;//判断点是否全部

	protected:
		QVector3D _centerPoint; QVector3D _centerDirection; double _radius; //空间
		QVector2D _screenCenter; double _screenRadius;//屏幕
	};
	class RENDDATA_EXPORT mPostMeshPickThread : public QObject
	{
		Q_OBJECT

	public:
		/*
		* 复制构造函数
		*/
		mPostMeshPickThread(mPostMeshPickData *postMeshPickData);

		/*
		* 析构函数
		*/
		~mPostMeshPickThread();

		/*
		* 设置当前的拾取模式
		*/
		void setPickMode(PickMode pickMode, MultiplyPickMode multiplyPickMode);

		/*
		* 设置当前的拾取过滤器
		*/
		void setPickFilter(MViewBasic::PickFilter *pickFilter);

		/*
		 * 开始拾取
		 */
		void startPick();

		/*
		* 添加部件数据
		*/
		void appendPartSpaceTree(QString partName, Space::SpaceTree *spaceTree);

		/*
		* 设置当前的渲染数据
		*/
		void setCurrentFrameRend(mOneFrameData1 * oneFrameData, mPostOneFrameRendData *postOneFrameRendData);

		/*
		* 设置当前的生效的剖切平面
		*/
		void setCuttingPlaneNormalVertex(QVector<QPair<QVector3D, QVector3D>> postCuttingNormalVertex);

		/*
		* 设置拾取的单元过滤器
		*/
		void setPickElementTypeFilter(std::set<int> pickElementTypeFilter);

		/*
		 * 设置矩阵
		 */
		void setMatrix(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model);

		/*
		* 设置矩阵
		*/
		void setMatrix(QMatrix4x4 pvm);

		/*
		* 设置通过角度拾取的角度
		*/
		void setPickAngleValue(double angle) { _pickAngleValue = angle; };

		/*
		 * 设置单选位置
		 */
		void setLocation(const QVector2D & pos, float depth);

		/*
		* 设置位置(矩形或者多边形框选)
		*/
		void setLocation(QVector<QVector2D> pickQuad, QVector3D direction = QVector3D());

		/*
		 * 设置窗口大小
		 */
		void setWidget(int w, int h);

		/*
		 *	是否完成拾取
		 */
		bool isFinished();

		/*
		 * 拾取完成设置为false
		 */
		void setFinished();
	private:
		//拾取主程序		
		void doSoloPick(QString partName, Space::SpaceTree* spaceTree);
		void doMultiplyPick(QString partName, Space::SpaceTree* spaceTree);
		void doAnglePick();

		/*
		 * 单选
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
		* 矩形框选
		*/
		void MultiplyPickNode(QString partName, Space::SpaceTree* spaceTree);
		void MultiplyPick1DMesh(QString partName, Space::SpaceTree* spaceTree);
		void MultiplyPick2DMesh(QString partName, Space::SpaceTree* spaceTree);
		void MultiplyPickAnyMesh(QString partName, Space::SpaceTree* spaceTree);
		void MultiplyPickMeshFace(QString partName, Space::SpaceTree* spaceTree);

		/*
		void PolygonPickNode(QString partName, Space::SpaceTree* spaceTree);
		void PolygonPick1DMesh(QString partName, Space::SpaceTree* spaceTree);
		void PolygonPick2DMesh(QString partName, Space::SpaceTree* spaceTree);
		void PolygonPickAnyMesh(QString partName, Space::SpaceTree* spaceTree);
		void PolygonPickMeshFace(QString partName, Space::SpaceTree* spaceTree);
		void RoundPickNode(QString partName, Space::SpaceTree* spaceTree);
		void RoundPick1DMesh(QString partName, Space::SpaceTree* spaceTree);
		void RoundPick2DMesh(QString partName, Space::SpaceTree* spaceTree);
		void RoundPickAnyMesh(QString partName, Space::SpaceTree* spaceTree);
		void RoundPickMeshFace(QString partName, Space::SpaceTree* spaceTree);
		*/
		
		//通过角度拾取
		void SoloPickNodeByLineAngle();		
		void SoloPickNodeByFaceAngle();
		void SoloPick1DMeshByAngle();
		void SoloPickMeshLineByAngle();
		void SoloPick2DMeshByAngle();
		void SoloPickMeshFaceByAngle();

		
		//判断单选是否拾取到该部件,并且返回他的深度值
		
		bool IsSoloPickMeshPart(MDataPost::mPostMeshPartData1 *meshPartData, float &depth);

		
		//判断框选是否拾取到该部件
		bool isMultiplyPickMeshPart(MDataPost::mPostMeshPartData1 *meshPartData);

		
		//将世界坐标转化为屏幕坐标
		QVector2D WorldvertexToScreenvertex(QVector3D Worldvertex);

		QVector3D ScreenvertexToWorldvertex(QVector3D vertex);
		
		//将世界坐标转化为屏幕坐标并且返回他的深度值
		QVector2D WorldvertexToScreenvertex(QVector3D Worldvertex, float &depth);

		/*
		 * 将世界坐标转化为屏幕坐标并且返回他的深度值
		 */
		void WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs, QVector<QVector2D> &Screenvertexs, std::set<float> &depths);

		/*
		* 判断节点是否被平面裁剪,即不能拾取,true代表被裁减了，不能拾取；false代表没有被裁剪，可以拾取
		*/
		bool isVertexCuttingByPlane(QVector3D vertex);

		/*
		* 判断一个单元的所有节点是否被平面裁剪，即只有全部节点都没有被裁减了才能拾取，true代表部分被裁减了，不能拾取；false代表全部没有被裁剪，可以拾取
		*/
		bool isVertexCuttingByPlane(QVector<QVector3D> vertexs);

		//获取形心
		QVector3D getCenter(QVector<QVector3D> vertexs);


	signals:
		/*
		 * 发送信号完成
		 */
		void finishedPickSig();


	public slots:


	private:

		//拾取过滤器
		MViewBasic::PickFilter *_pickFilter;

		//拾取模式
		MViewBasic::PickMode _pickMode;

		//框选拾取模式
		MViewBasic::MultiplyPickMode _multiplyPickMode;

		//拾取的单元类型过滤器
		std::set<int> _pickElementTypeFilter;

		//拾取角度
		float _pickAngleValue = 60;

		//窗口大小
		int _Win_WIDTH, _Win_HEIGHT;

		//着色器视图矩阵
		QMatrix4x4 _projection, _view, _model, _pvm;

		/*
		* 当前帧的模型数据
		*/
		MDataPost::mOneFrameData1 *_oneFrameData;

		/*
		* 当前帧的变量数据
		*/
		mPostOneFrameRendData *_oneFrameRendData;

		/*
		* 当前帧的切面的法向量和点
		*/
		QVector<QPair<QVector3D,QVector3D>> _postCuttingNormalVertex;

		/*
		* 拾取数据
		*/
		mPostMeshPickData *_postMeshPickData;

		/*
		 * 模型名称
		 */
		QHash<QString, Space::SpaceTree*> _partSpaceTrees;

		/*
		 * 点选位置
		 */
		QVector2D _pos;
		float _depth;
		QVector3D _p;
		QVector3D _origin;
		QVector3D _dir;

		/*********/
		std::shared_ptr<mBasePick> _pick;
		/*
		 * 点选矩形
		 */
		QVector<QVector2D> soloQuad;

		////框选矩形

		///*
		// * 矩形和多边形框选位置
		// */
		//QVector<QVector2D> multiQuad{};

		///*
		//* 圆形框选位置
		//*/
		//QVector3D _centerPoint; QVector3D _centerDirection; double _radius; QVector2D _centerScreenPoint; double _screenRadius;

		////矩形框选中心和其半个宽度和半个高度
		//QVector2D _centerBox, _boxXY_2;

		/*
		 * 是否完成
		 */
		bool _isfinished;


	};
}
