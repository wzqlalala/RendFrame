#pragma once

//Qt
#include <QString>
#include <QStringList>
#include <QHash>
#include <QVector3D>

/*
* 单个变量信息
*/
struct VariableInfo
{
	//结果类型：常规变量/节点单元变量
	int _resultType;

	//变量名
	QString _varName;

	//变量类型：节点/单元
	int _varType;

	//变量个数
	int _varCount;

	//每个变量的层数
	QList<int> _varLayer;

	//每个变量的层名称
	QList<QStringList> _varLayerNames;

	//分量数
	int _nCompo;

	//是否计算矢量模
	bool _isMag;

	//分量名
	QStringList _compoNames;

	//该分量是否为复数
	QHash<QString, bool> _compoComplex;

	//字节开始位置
	long long int _varByte;
};

/*
* 频率结果
*/
struct SpecialFrequency
{
	//模态频率
	double _modalFreq;

	//模态特征值
	double _modalEigen;
};

/*
* 复模态结果
*/
struct ComplexModes
{
	//模态值实部
	double _realVal;

	//模态值虚部
	double _virtualVal;

	//频率
	double _freqVal;

	//阻尼
	double _dampVal;
};

/*
* 电磁-电磁力
*/
struct EMForce
{
	//名称
	QString _name;

	//值
	double val[3];
};

/*
* 电磁-电磁力矩
*/
struct EMTorque
{
	//名称
	QString _name;

	//值
	double val[3];
};

/*
* 电磁-涡流损耗
*/
struct EMEddyCurrentLoss
{
	//名称
	QString _name;

	//值
	double val;
};

/*
* 电磁-能量
*/
struct EMEnergy
{
	//名称
	QString _name;

	//值
	double val;
};

/*
* 电磁-电感
*/
struct EMInductance
{
	//名称
	QString _name;

	//矩阵标题
	QStringList _matrixtitle;

	//值
	double* val;
};

/*
* 电磁-电容
*/
struct EMCapacitance
{
	//名称
	QString _name;

	//矩阵标题
	QStringList _matrixtitle;

	//值
	double* val;
};

/*
* 电磁-欧姆损耗
*/
struct EMOhmicLoss
{
	//名称
	QString _name;

	//值
	double val;
};

/*
* 电磁-磁滞损耗
*/
struct EMHysteresisLoss
{
	//名称
	QString _name;

	//值
	double val;
};

/*
* 帧信息
*/
struct FrameInfo
{
	void setRigidWallEndPos(int ID, QVector3D pos)
	{
		_rigidWallEndPos.insert(QString("rigidwall_%1").arg(ID), pos);
	};
	QHash<QString, QVector3D> getRigidWallEndPos()
	{
		return _rigidWallEndPos;
	};

	//对应的文件名
	QString _frameFile;

	//帧号
	int _frameNum;

	//帧名称
	QString _frameName;

	//帧工况名称
	QString _frameCase;

	//帧时间
	double _frameTime;

	//帧模型信息
	int _frameModel;

	//帧结果信息
	QList<VariableInfo> _frameVars;

	//是否有特征值结果
	bool _isFrequency = false;

	//特征值结果
	SpecialFrequency _frequency;

	//是否有复模态结果
	bool _isComplexMode = false;

	//复模态结果
	ComplexModes _compMode;

	//是否有电磁-电磁力结果
	bool _isEMForce = false;

	//电磁-电磁力结果
	QList<EMForce> _emforce;

	//是否有电磁-电磁力矩结果
	bool _isEMTorque = false;

	//电磁-电磁力矩结果
	QList<EMTorque> _emtorque;

	//是否有电磁-涡流损耗结果
	bool _isEMEddyCurrentLoss = false;

	//电磁-涡流损耗结果
	QList<EMEddyCurrentLoss> _emeddyloss;

	//是否有电磁-能量
	bool _isEMEnergy = false;

	//电磁-能量结果
	QList<EMEnergy> _emenergy;

	//是否有电磁-电感结果
	bool _isEMInductance = false;

	//电磁-电感结果
	QList<EMInductance> _eminductance;

	//是否有电磁-电容结果
	bool _isEMCapacitance = false;

	//电磁-电容结果
	QList<EMCapacitance> _emcapacitance;

	//是否有电磁-欧姆损耗
	bool _isEMOhmicLoss = false;

	//电磁-欧姆损耗
	QList<EMOhmicLoss> _emohmicloss;

	//是否有电磁-磁滞损耗
	bool _isEMHysteresisLoss = false;

	//电磁-磁滞损耗
	QList<EMHysteresisLoss> _emhysteresisloss;

	//载荷结果
	QHash<QString, long long int> _loadResults;

	//刚性墙终止坐标
	QHash<QString, QVector3D> _rigidWallEndPos;
};