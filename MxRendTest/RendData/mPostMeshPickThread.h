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
		 * 设置位置
		 */
		void setLocation(const QPoint & pos, float depth);

		/*
		 * 设置位置
		 */
		void setLocation(int lastX, int lastY, int nowX, int nowY, QVector<QVector2D> pickQuad);

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
		void doQuadPick(QString partName, Space::SpaceTree* spaceTree);
		void doRoundPick(QString partName, Space::SpaceTree* spaceTree);
		void doPolygonPick(QString partName, Space::SpaceTree* spaceTree);

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
		* 框选
		*/
		void MultiplyPickNode(QString partName, Space::SpaceTree* spaceTree);
		void MultiplyPick1DMesh(QString partName, Space::SpaceTree* spaceTree);
		void MultiplyPick2DMesh(QString partName, Space::SpaceTree* spaceTree);
		void MultiplyPickAnyMesh(QString partName, Space::SpaceTree* spaceTree);
		void MultiplyPickMeshFace(QString partName, Space::SpaceTree* spaceTree);

		/*
		 * 判断单选是否拾取到该部件,并且返回他的深度值
		 */
		bool IsSoloPickMeshPart(MDataPost::mPostMeshPartData1 *meshPartData, float &depth);

		/*
		 * 判断框选是否拾取到该部件
		 */
		bool isMultiplyPickMeshPart(MDataPost::mPostMeshPartData1 *meshPartData);

		/*
		 * 将世界坐标转化为屏幕坐标
		 */
		QVector2D WorldvertexToScreenvertex(QVector3D Worldvertex);

		QVector3D ScreenvertexToWorldvertex(QVector3D vertex);
		/*
		 * 将世界坐标转化为屏幕坐标并且返回他的深度值
		 */
		QVector2D WorldvertexToScreenvertex(QVector3D Worldvertex, float &depth);

		/*
		* 判断节点是否被平面裁剪,即不能拾取,true代表被裁减了，不能拾取；false代表没有被裁剪，可以拾取
		*/
		bool isVertexCuttingByPlane(QVector3D vertex);

		/*
		* 判断一个单元的所有节点是否被平面裁剪，即只有全部节点都没有被裁减了才能拾取，true代表部分被裁减了，不能拾取；false代表全部没有被裁剪，可以拾取
		*/
		bool isVertexCuttingByPlane(QVector<QVector3D> vertexs);


	signals:
		/*
		 * 发送信号完成
		 */
		void finishedPickSig();


	public slots:


	private:
		//线程
		QThread *_thread;

		//拾取过滤器
		MViewBasic::PickFilter *_pickFilter;

		//拾取模式
		MViewBasic::PickMode _pickMode;

		//框选拾取模式
		MViewBasic::MultiplyPickMode _multiplyPickMode;

		//拾取的单元类型过滤器
		std::set<int> _pickElementTypeFilter;

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
		QPoint _pos;
		float _depth;
		/*
		 * 点选矩形
		 */
		QVector<QVector2D> soloQuad;

		//框选矩形
		QVector<QVector2D> multiQuad{};

		/*
		 * 框选位置
		 */
		int _centerX, _centerY, _boxW, _boxY;

		/*
		 * 是否完成
		 */
		bool _isfinished;


	};
}
