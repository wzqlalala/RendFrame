#pragma once
//解决中文乱码
#pragma execution_character_set("utf-8")

#include "renddata_global.h"

#include <QVector3D>
#include <QVector>
#include <QList>
#include <QHash>
#include <QSet>
#include <set>

#include "mBasicEnum.h"
#include "mBasicStructor.h"

using namespace std;
namespace MDataPost
{
	class mPostMeshPartData1;
	class mPostMeshData1;
	class mPostMeshNodeData1;
	class mPostMeshData1;
	class mPostMeshLineData1;
	class mPostMeshFaceData1;
	class RENDDATA_EXPORT mOneFrameData1
	{

	public:
		/*
		* 默认构造函数
		*/
		mOneFrameData1();

		/*
		 *默认析构函数
		 */
		~mOneFrameData1();

		/*
		 * 模型操作
		 */

		 //获取所有的节点ID（除去隐藏的网格）
		set<int> getAllNodeIDs();
		//获取一个部件的所有节点ID(除去隐藏的网格)
		set<int> getAllNodeIDsInPart(QString partName);

		//获取所有的网格ID（除去隐藏的网格）
		set<int> getAllMeshIDs();
		//获取一个部件的所有的网格ID(除去隐藏的网格)
		set<int> getAllMeshIDsInPart(QString partName);
		//获取所有的三维网格ID（除去隐藏的网格）
		QVector<mPostMeshData1*> getAll3DMeshIDs();
		//获取所有的二维网格ID（除去隐藏的网格）
		QVector<mPostMeshData1*> getAll2DMeshIDs();

		//获取所有的单元面ID（除去隐藏的网格）
		set<int> getAllMeshFaceIDs();
		//获取一个部件的所有的单元面ID(除去隐藏的网格)
		set<int> getAllMeshFaceIDsInPart(QString partName);
		//获取所有的单元面指针（除去隐藏的网格）
		QVector<mPostMeshFaceData1*> getAllMeshFaceDatas();

		//判断ID对应的节点或单元是否存在
		bool isIdExistInModel(int id, const QString& type);

		//通过ID(名称)获取相应的数据
		mPostMeshPartData1 *getMeshPartDataByPartName(QString partName);
		mPostMeshLineData1* getMeshLineDataByID(int ID);
		mPostMeshFaceData1* getMeshFaceDataByID(int ID);
		mPostMeshData1* getMeshDataByID(int ID);
		mPostMeshNodeData1* getNodeDataByID(int ID);

		//获取当前帧部件的迭代器
		QHashIterator<QString, mPostMeshPartData1*> getMeshPartIterator();

		//获取当前帧部件容器
		const QHash<QString, mPostMeshPartData1*> getMeshParts();

		//添加数据
		void appendMeshPartData(QString partName, mPostMeshPartData1 *data);
		void appendMeshFaceData(int ID, mPostMeshFaceData1 *data);
		void appendMeshLineData(int ID, mPostMeshLineData1 *data);
		void appendMeshData(int ID, mPostMeshData1 *data);
		void appendNodeData(int ID, mPostMeshNodeData1 *data);



		//获取模型的最大半径和中心
		void getModelSize(MBasicFunction::ModelSize & modelSize);

		//判断是否存在模型
		bool isExistModel();

		//获取模型的模型的单元大致尺寸
		float getModeMeshSize();

		/*
		* 判断是否有该部件通过名字
		*/
		bool isExistMeshPart(QString partName);

		/*
		 *部件操作
		 */

		//获取所有的部件名称(去掉隐藏的部件)
		std::set<QString> getAllPartNames();

		//获取所有的部件名称(去掉隐藏的部件)
		QStringList getAllPartNameList();

		//读文件添加数据，不自动更新渲染
		void importMeshPartData(QString partName, mPostMeshPartData1 *data);

		//替换部件
		void replaceMeshPartData(QString partName, mPostMeshPartData1* data);

		//设置网格部件的颜色
		void setMeshPartColor(QString partName, QVector3D color);

		//设置网格部件的显隐
		void setMeshPartVisual(QString partName, bool isShow);

		//获取网格部件的显隐
		bool getMeshPartVisual(QString partName);

		//设置全部网格部件的显隐
		void setAllMeshPartVisual(bool isShow);

		//设置全部网格部件的颜色
		void setAllMeshPartColor(QVector3D color);

		//获取网格部件的颜色
		QVector3D getMeshPartColor(QString partName);

		/*
		* 获取当前帧节点总数
		*/
		int getOneFrameNodeAmount();

		/*
		 * 添加当前帧的模型
		 */
		void appendPartData(QString partName, mPostMeshPartData1* onePartData);

		/*
		 * 单元操作
		 */

		 //创建网格
		void createMesh(int ID, MBasicFunction::MeshType meshType, MBasicFunction::ElementType elementType, 
			QVector<int> index, mPostMeshPartData1 *partData, QHash<QVector<int>, mPostMeshFaceData1*> &_meshFace);

		//创建网格
		void createMesh(int ID, MBasicFunction::MeshType meshType, MBasicFunction::ElementType elementType,
			QVector<int> index, mPostMeshPartData1 *partData, QVector<QVector<QPair<QPair<int, int>, mPostMeshFaceData1*>>> &_meshFace);

		//隐藏网格
		void maskMesh(set<int> meshIDs);

		//显示网格
		void showMesh(set<int> meshIDs);

		//反向隐藏网格
		void reverseMaskMesh();

		//显示所有网格
		void showAllMesh();

		//根据单元类型获取个数
		int getNumByElementType(MBasicFunction::ElementType elementType);

		//获取网格的形心
		QVector3D getMeshCenter(mPostMeshData1 *meshData);

		//删除网格
		//void deleteMesh(set<int> meshIDs);

		/*
		 * 单元面操作
		 */

		 //仅仅判断是否是表面，对于原始文件来说
		bool MeshFaceIsSurfaceIDPointer(mPostMeshFaceData1* mMeshFaceData1);

		//判断这个单元面是不是表面，对于原始文件来说,并且返回这个单元面所属的单元的指针,边界线可能存在于两个网格中（两个部件），如果是两个网格中，则也返回另一个单元的指针
		bool MeshFaceIsSurface(mPostMeshFaceData1* mMeshFaceData1, mPostMeshData1 **meshData, int isInTwoMesh, mPostMeshData1 **meshData1);

		//判断这个单元面是不是表面，对于一般情况来说
		bool MeshFaceIsSurface(mPostMeshFaceData1 *mMeshFaceData1);

		//判断这个单元面是不是表面，对于一般情况来说,返回这个单元面现在所属的单元ID
		int MeshFaceIsSurface1(mPostMeshFaceData1 *mMeshFaceData1);

		 //创建单元面
		void createMeshFace(QVector<int>  set, QVector<int> index, mPostMeshData1* meshData, int order,
			QString partName, mPostMeshPartData1* partData, QHash<QVector<int>, mPostMeshFaceData1*> &_meshFace);

		//创建单元面
		void createMeshFace(QVector<int> set,  mPostMeshData1* meshData, int order,
			QString partName, QVector<QVector<QPair<QPair<int, int>, mPostMeshFaceData1*>>> &_meshFace);

		//移除掉不是表面的单元面
		void eraseNotMeshFace();

		//判断单元面是否渲染
		void judgeMeshFaceIsShow();

		//返回是否添加到表面,根据单元面判断的单元
		bool getMeshFaceShow(mPostMeshFaceData1 *mMeshFaceData1,mPostMeshData1* meshData);

		//将表面存储到容器中
		void setMeshFaceShow();

		//判断单元面是否渲染
		void judgeMeshFaceIsShow(set<int> meshIDs);

		/*
		 * 单元线操作
		 */

		 //计算网格的边界线
		void caculateMeshLine();

		 //判断边界线属于单元的第几个线
		int judgeMeshLineOrder(mPostMeshData1 *meshData, std::set<int> index);

		/*
		 * 模型操作
		 */

		/*
		 * 获取当前帧的所有模型
		 */
		QHash<QString, mPostMeshPartData1*> getAllPartData();

		/*
		* 获取当前帧所有零件名
		*/
		QStringList getOneFramePartNames();

		/*
		 * 通过名称获取当前帧的一个模型
		 */
		mPostMeshPartData1 *getOnePartDataByName(QString partName);

		/*
		 * 删除这一帧的所有数据
		 */
		void deleteOneFrameData();

		/*
		* 获取当前帧单元总数
		*/
		int getOneFrameElementAmount();

		/*
		* 设置频率数据
		*/
		void setOneFrameModalValue(double freq, double eig);

		/*
		* 获取是否有模态数据
		*/
		bool isOneFrameModalValue();

		/*
		* 获取模态频率值
		*/
		double getOneFrameModalFrequency();

		/*
		* 获取模态特征值
		*/
		double getOneFrameModalEigen();

		/*
		 * 获取当前帧的节点迭代器
		 */
		QHashIterator<int, mPostMeshNodeData1*> getMeshNodeIterator();

		///*
		//* 存储刚性墙的终止坐标
		//*/
		//void setRigidWallEndPos(int ID, QVector3D pos);

		///*
		//* 获取刚性墙的终止坐标
		//*/
		//QVector3D getRigidWallEndPos(QString name);
		
	public:
		//模型操作枚举
		MBasicFunction::ModelOperateEnum _meshOperateEnum;

		//渲染需要实时判断
		//单个模型操作的部件，渲染后置为空
		std::set<QString> _oneMeshOperatePartName;

		//是否进行全部模型操作，渲染后置为false
		bool _allMeshOperatePart;

		void calculatePointCell();

		void resizeNodeSize(int size);
		void resizeCopyCellSize(int size);
		void calculateCellData();
	protected:

		//计算几何中心和半径
		void caculateMeshModelRadiusAndCenter();

		//计算网格的尺寸
		void caculateMeshSize();

		//生成边界线
		void generateMeshLine(mPostMeshFaceData1* surfaceID1, mPostMeshFaceData1* surfaceID2, mPostMeshData1 *meshData, QString partName, mPostMeshPartData1* partData);

	private:

		//是否有频率数据
		bool _hasModalValue;

		//频率数据
		double _modalValue[2];

	protected:

		QVector<mPostMeshNodeData1*> _nodeData2;
		QHash<int, mPostMeshNodeData1*> _nodeData1;

		QVector<mPostMeshData1*> _meshData2;
		QHash<int, mPostMeshData1*> _meshData1;


		QHash<int, mPostMeshLineData1*> _meshLineData1;

		std::vector<mPostMeshFaceData1*> _meshFaceData1;

		QHash<QString, mPostMeshPartData1*> _partData1;
		QHash<MBasicFunction::ElementType, int> _eleNum;

		//模型尺寸
		MBasicFunction::ModelSize _modelSize;

		//是否对模型进行删除或者添加
		bool _isAddOrDeletePart_Mesh;

		//模型的单元大致尺寸
		float _meshSize;
		
	};
}