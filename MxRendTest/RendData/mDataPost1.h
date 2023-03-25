#pragma once

#include "renddata_global.h"
#include "mResultInfo.h"

//MBasicFunction
#include "mPostEnum.h"

//Qt
#include <QList>
#include <QHash>
#include <set>
#include <QVector3D>

namespace MDataPost
{
	class mOneFrameData1;
	class mPostSetBase;
	class mPostRigidWallDataBase;
	class RENDDATA_EXPORT mDataPost1
	{
	public:
		/*
		 * 默认构造函数
		 */
		mDataPost1();

		/*
		 * 默认析构函数
		 */
		~mDataPost1();

		/*
		* 设置模型名
		*/
		void setModelName(const QString& name);

		/*
		* 获取模型名
		*/
		QString getModelName();

		/*
		* 存储结果类型（时间相关还是频率相关）
		*/
		void setResultType(int type);

		/*
		* 获取结果类型
		*/
		int getResultType();

		/*
		 * 删除所有帧的数据
		 */
		void deleteAllFrameData();

		/*
		 * 添加某帧的数据
		 */
		void appendFrameData(int frameid,double time, mOneFrameData1 *frameData);

		/*
		* 添加某帧与某帧模型数据一致
		*/
		void appendSameFrame(int frame1, int frame2);

		/*
		 * 获取第0帧的后处理数据
		 */
		mOneFrameData1 *getFirstFrameData();

		/*
		 * 获取最后一帧的后处理数据
		 */
		mOneFrameData1 *getLastFrameData();

		/*
		 * 获取所有帧的id
		 */
		void getAllFrameId(std::set<int> &frameids);

		/*
		 * 获取某一帧对应的后处理模型数据
		 */
		mOneFrameData1 *getOneFrameData(int frameid);

		/*
		 * 获取某一帧对应的时间
		 */
		double getOneFrameTime(int frameid);

		/*
		* 获取总帧数
		*/
		int getTotalFrameCount();

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

		//读取完文件后遍历所有的帧存储所有的零部件名称
		void updateAllPostPartNames();

		//获取所有帧的所有部件
		std::set<QString> getAllPostPartNames();

		/*
		* 添加帧信息
		*/
		void appendFrameInfo(FrameInfo fi);

		/*
		* 获取帧信息
		*/
		FrameInfo getFrameInfo(int f);

		/*
		* 获取帧信息
		*/
		FrameInfo getFrameInfo(const QString& cname);

		/*
		* 根据工况获取帧信息
		*/
		QList<FrameInfo> getFrameInfos(const QStringList& cnames);

		/*
		* 获取所有帧信息
		*/
		QList<FrameInfo> getFrameInfos();

		/*
		* 获取总帧数
		*/
		int getFrameAmount();

		/*
		* 获取所有工况名
		*/
		QStringList getAllCaseNames();

		/*
		* 获取某工况下的帧名称
		*/
		QStringList getCaseFrameNames(const QString& cname = QString("All"));

		/*
		* 获取某帧的变量名
		*/
		QStringList getFrameVariables(int f);

		/*
		* 获取某帧的矢量名
		*/
		QStringList getFrameVectors(int f);

		/*
		* 获取所有变量名
		*/
		QStringList getAllVariableNames();

		/*
		* 获取某变量的分量
		*/
		QStringList getVariableCompos(const QString& var);

		/*
		* 获取某变量的分量是否为复数
		*/
		bool isVarCompoComplex(const QString& var, const QString& compo);

		/*
		* 获取某变量的层
		*/
		QStringList getVariableLayer(const QString& var);

		/*
		 * 获取某帧的所有零件名
		 */
		QStringList getFrameParts(int f);

		/*
		 * 获取某帧的时间
		 */
		double getFrameTime(int f);

		/*
		 * 获取某帧所在的工况名
		 */
		QString getFrameCase(int f);

		/*
		* 获取某帧模态频率结果
		*/
		bool getFrameFreqValue(int f, double* val);

		/*
		* 获取某帧复模态结果
		*/
		bool getFrameComplexMode(int f, double* val);

		/*
		* 获取变量类型（0-节点或1-单元）
		*/
		int getVariableType(const QString& var);

		/*
		* 添加载荷提取壳应力/应变单元层数
		*/
		void appendShellLayer(const QString& name, int nlayer);

		/*
		* 获取载荷提取壳应力/应变层数
		*/
		int getShellLayerCount(const QString& name);

		/*********************************************************************************集合******************************************************************************************/
		/*
		* 添加数据
		*/
		void appendSetData(mPostSetBase* setData);

		/*
		* 获取所有个数
		*/
		int getSetDataCount();

		/*
		* 获取数据，根据索引
		*/
		mPostSetBase* getSetData(int i);

		/*
		* 获取数据，根据名称
		*/
		mPostSetBase* getSetData(const QString& name);

		/*
		* 获取所有通用集合的名称（不包括载荷和边界集合等特殊集合）
		*/
		QStringList getSetNames();

		/*
		* 替换集合数据，根据名称
		*/
		void replaceSetData(mPostSetBase* sData);

		/*
		* 删除集合数据
		*/
		void deleteSetData(const QString& name);

		/*
		* 删除集合数据
		*/
		void deleteSetData(int i);

		/*
		* 删除所有数据
		*/
		void deleteSetData();

		/*
		* 重命名集合
		*/
		void resetSetName(const QString& oldname, const QString& newname);

		/*
		* 判断名是否存在
		*/
		bool isSetNameExist(const QString& name);


		/*
		* 获取某种类型的数据（不包括载荷和边界集合等特殊集合）
		*/
		QList<mPostSetBase*> getTypeSetDatas(PostSetType type);

		/*
		* 获取某种类型通用集合的所有名称（不包括载荷和边界集合等特殊集合）
		*/
		QStringList getTypeSetNames(PostSetType type);


		/*********************************************************************************刚性墙******************************************************************************************/
		/*
		* 添加数据
		*/
		void appendRigidWallData(mPostRigidWallDataBase* data);

		/*
		* 获取所有个数
		*/
		int getRigidWallDataCount();

		/*
		* 获取数据，根据名称
		*/
		mPostRigidWallDataBase* getRigidWallData(QString name);

		/*
		* 获取所有的刚性墙ID
		*/
		QList<QString> getAllRigidWallNames();

	private:
		//名称
		QString _modelName;

		//结果是时间相关还是频率相关  0-时间，1-频率
		int _resultType;

		//每一帧对应的模型数据
		QHash<int, mOneFrameData1*> _frameid_frameData;

		//当前帧与哪一帧的模型一致
		QHash<int, int> _frameSameFrame;

		//每一帧对应的时间
		QHash<int, double> _frameid_Time;

		//所有帧的数据
		std::set<int> _allFrameids;

		//所有帧的所有部件
		std::set<QString> _allPartNames;

		//所有帧信息
		QList<FrameInfo> _frameInfo;

		//载荷提取数据：壳应力和应变的层数
		QHash<QString, int> _shellLayers;

		//某一帧的集合数据（切换帧会删除数据）
		QList<mPostSetBase*> _setData;

		//初始帧的刚性墙
		QHash<QString, mPostRigidWallDataBase*> _rigidWallDatas;//刚性墙名称_数据指针
	};
}
