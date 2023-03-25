#pragma once
//解决中文乱码
#pragma execution_character_set("utf-8")

//Qt
#include <QObject>
#include <QRunnable>
#include <QHash>
#include <iostream>
#include <fstream>
#include <QStringList>
#include <QVector>
#include <QString>
#include <set>

#include "mMeshViewEnum.h"

namespace MDataPost
{
	class mOneFrameData1;
	class mDataPost1;
	class mPostOneFrameRendData;
}

//namespace MPostView
//{
//	class mPostAnimationRendData1;
//}

using namespace std;
using namespace MDataPost;
namespace MIOFile
{
	class mIMxdbFile1 : public QObject, public QRunnable
	{
		Q_OBJECT
	public:
		/*
		* 默认构造函数
		*/
		mIMxdbFile1();

		/*
		* 重载构造函数
		* initfile表示初始构型的文件，仅有模型数据
		* filenames所有帧文件
		*/
		mIMxdbFile1(const QString& initfile, const QStringList& filenames);

		/*
		* 析构函数
		*/
		~mIMxdbFile1();

		void setDataPost(shared_ptr<mDataPost1> dp) { _dp = dp; };

		/*
		* 设置单帧结果数据
		*/
		void setOneFrameRenderData(mPostOneFrameRendData* oneframe);

		/*
		* 获取单帧结果数据
		*/
		mPostOneFrameRendData* getOneFrameRenderData();

		/*
		* 设置动画结果数据
		*/
		//void setAnimateRenderData(mPostAnimationRendData1* aniframe);

	public slots:

		/*
		* 导入全部文件（获取模型数据和帧信息）
		*/
		void slot_importAllMxDbFile();

		/*
		* 动画读取
		*/
		//void slot_importOneFrameData(mPostAnimationRendData1 *animationData, int f, int threadNum, const QString& pname, const QString& var, const QString& compo, const QString& layer, const QString& complex,bool hasstreamLine,QString streamLineVectorName, bool hasvectorArrowVector, QString vectorArrowVectorName);

		/*
		* 读取单帧变量数据（主要用于云图和动画）
		*/
		bool readCloudData();

		/*
		* 读取单帧位移矢量数据（主要用于变形图）
		*/
		void readDisplacementData();

		/*
		* 读取单帧矢量数据（主要用于流线图、矢量图等）
		*/
		void readVectorData(const QString& pname, int f, const QString& var);

		/*
		* 读取时间-变量曲线数据
		*/
		void readCurveData1(const QString& pname, const QString& var, const QStringList& compos, const QString& layer, const QString& complex, const set<int>& ids,
			QStringList& cnames, QVector<double>& xvalues, QHash<QString, QVector<double>>& yvalues);

		/*
		* 读取ID-变量曲线数据
		*/
		void readCurveData2(const QString& pname, const QString& var, const QStringList& compos, const QString& layer, const QString& complex, const QVector<int>& ids, const set<int>& frameIds,
			QStringList& cnames, QHash<QString, QHash<int, double>>& yvalues);

		/*
		* 读取变量-变量曲线数据
		*/
		void readCurveData3(const QString& pname, const QString& varx, const QString& compox, const QString& layerx, const QString& complexx,
			const QString& vary, const QStringList& composy, const QString& layery, const QString& complexy, 
			const set<int>& ids,QStringList& cnames, QVector<double>& xvalues, QHash<QString, QVector<double>>& yvalues);

		/*
		* 读取载荷
		*/
		void readLoadData(const QString& pname, const QStringList& casenames, std::set<int>& ids, const QString& type);

		/*
		* 读取所有变量最值信息
		*/
		void readAllResults(const QString& pname, QList<QStringList>& vals);

		/*
		* 读取指定标量变量的信息
		*/
		void readScalarData(const QString& pname, int f, const QString& var, QHash<int, double> &vals,MViewBasic::NodeOrElement &nodeOrElement);

		/*
		* 读取指定矢量变量的信息
		*/
		void readVectorData(const QString& pname, int f, const QString& var, QHash<int,QVector3D> &vals, MViewBasic::NodeOrElement &nodeOrElement);


	private:

		/*
		* 重载run函数
		*/
		void run() override;
		/*
		* 判断文件状态
		*/
		bool openFileStatus(const QString& fname);

		/*
		* 读取初始模型数据
		*/
		void readInitModelDatas(shared_ptr<mDataPost1> dataPost,mOneFrameData1* fd);

		/*
		* 读取模型数据
		*/
		void readModelDatas(mOneFrameData1* fd);

	signals:
		/*
		* 发送信号，反馈文件读取情况
		* 参数type表示错误的类型
		*/
		//void readFileStatusSig(IOFileErrorType type = UnknownError);

		/*
		* 发送信号，读取成功
		*/
		void readFileSuccessSig(const QString& filename);

		/*
		* 发送信号，读取全部帧文件成功
		*/
		void finishedImportAllMxDbFileSig();


	private:
		//版本号
		unsigned int _version;

		//初始构型文件
		QString _initFileName;

		//帧文件
		QStringList _frameFiles;

		//后处理数据库名称
		QString _postDataName;

		shared_ptr<mDataPost1> _dp;

		//所有帧信息
		//static QList<FrameInfo> _frameInfo;

		//文件数据流
		ifstream _readf;

		//单帧结果数据
		mPostOneFrameRendData* _oneFrameRender;

		//动画结果数据
		//mPostAnimationRendData1* _animateRender;
		//生成动画需要的参数
		int _frame;
		//mPostAnimationRendData1 *_animationData; int _threadNum; QString _pname, _var, _compo, _layer, _complex; bool _hasstreamLine; QString _streamLineVectorName; bool _hasvectorArrowVector; QString _vectorArrowVectorName;
		//long long int _byteAmount;
	};


}