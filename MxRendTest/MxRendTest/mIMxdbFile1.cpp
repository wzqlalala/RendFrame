#include "mIMxdbFile1.h"

//MBasicFunction
#include "mCommonFunctions.h"
#include "mBasicEnum.h"

//MDataPost
#include "mDataPost1.h"
#include "mOneFrameData1.h"
#include "mPostMeshPartData1.h"
#include "mPostMeshData1.h"
#include "mPostMeshFaceData1.h"
#include "mPostMeshLineData1.h"
#include "mPostMeshNodeData1.h"
#include "mPostRigidWallData1.h"
#include "mPostOneFrameRendData.h"

//Qt
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QVector>
#include <QVector3D>
#include <QMutex>
#include <QtConcurrent/QtConcurrent>
#include <math.h>

#define optimazation false

using namespace MBasicFunction;
QMutex postmutex;
namespace MIOFile
{
	mIMxdbFile1::mIMxdbFile1():QRunnable()
	{
		_oneFrameRender = nullptr;
		//_animateRender = nullptr;
		setAutoDelete(true);
	}

	mIMxdbFile1::mIMxdbFile1(const QString& initfile, const QStringList& filenames):QRunnable()
	{
		_initFileName = initfile;
		_frameFiles = filenames;

		_oneFrameRender = nullptr;
		setAutoDelete(true);
		//_animateRender = nullptr;
	}

	mIMxdbFile1::~mIMxdbFile1()
	{
	}

	void mIMxdbFile1::setOneFrameRenderData(mPostOneFrameRendData * oneframe)
	{
		_oneFrameRender = oneframe;
	}

	mPostOneFrameRendData * mIMxdbFile1::getOneFrameRenderData()
	{
		return _oneFrameRender;
	}

	//void mIMxdbFile1::setAnimateRenderData(mPostAnimationRendData1 * aniframe)
	//{
	//	_animateRender = aniframe;
	//}

	void mIMxdbFile1::slot_importAllMxDbFile()
	{
		//后处理数据库
		//mDataPost1* datapost = new mDataPost1;
		
		//获取模型名
		QFileInfo inifinfo(_initFileName);
		QString inifname = inifinfo.baseName();
		if (inifname.isEmpty())
		{
			_dp->setModelName(QString("PostModel"));
			_postDataName = QString("PostModel");
		}
		else
		{
			_dp->setModelName(inifname);
			_postDataName = inifname;
		}

		//去除名称重复的后处理
		//mDatabaseSingleton::getInstance()->appendDataPost(datapost);

		//读取初始构型数据文件
		bool openinit = this->openFileStatus(_initFileName);
		if (openinit)
		{
			mOneFrameData1* initFrame = new mOneFrameData1;
			this->readInitModelDatas(_dp, initFrame);
			_readf.close();

			emit readFileSuccessSig(_initFileName);

			//存储
			_dp->appendFrameData(0, 0.0, initFrame);
		}
		else
		{
			//输出信息
			//mGlobalSignals::getInstance()->outputMessageSig(0, QString("没有初始帧文件..."));
			_readf.close();
			return;
		}

		//读取帧文件
		const int nframe = _frameFiles.size();
		for (int fi = 0; fi < nframe; ++fi)
		{
			//判断文件打开状态
			QString framefile = _frameFiles.at(fi);
			bool isopen = this->openFileStatus(framefile);
			if (!isopen)
			{
				//emit readFileStatusSig(FileNotOpen);
				continue;
			}

			FrameInfo frameInfo;
			frameInfo._frameFile = framefile;
			frameInfo._frameNum = fi + 1;

			//存储帧名称
			QFileInfo finfo(framefile);
			QString fname = finfo.baseName();
			QString caseinfo = fname.remove(inifname + "_");
			QString framename = QString("%1.%2").arg(fi + 1).arg(caseinfo);
			frameInfo._frameName = framename;

			//读取工况名
			char caseName[64];
			_readf.read(caseName, 64 * sizeof(char));
			//_byteAmount += 64*sizeof(char);
			frameInfo._frameCase = QString(caseName).simplified();

			//读取时间/频率类型，仅在0xA1B1C103及其之后的版本中
			if (_version >= 0xA1B1C103)
			{
				int typeval;
				_readf.read((char*)&typeval, sizeof(int));
				_dp->setResultType(typeval);
			}

			//读取并存储时间/频率
			double timeval;
			_readf.read((char*)&timeval, sizeof(double));
			//_byteAmount += sizeof(double);
			frameInfo._frameTime = timeval;

			//读取模型是否已存在的标识
			int sameframe;
			_readf.read((char*)&sameframe, sizeof(int));
			//_byteAmount += sizeof(int);
			frameInfo._frameModel = sameframe;
			_dp->appendSameFrame(fi + 1, sameframe);

			mOneFrameData1* framedata = nullptr;
			if (sameframe == -1)
			{
				//帧数据
				framedata = new mOneFrameData1;

				//输出信息
				//mGlobalSignals::getInstance()->outputMessageSig(0, QString("正在读取模型数据..."));
				this->readModelDatas(framedata);

				_dp->appendFrameData(fi + 1, timeval, framedata);
			}
			else
			{
				//获取相同的模型数据
				framedata = _dp->getOneFrameData(sameframe);
				_dp->appendFrameData(fi + 1, timeval, framedata);
			}
			if (framedata == nullptr)
			{
				continue;
			}

			//输出信息
			//mGlobalSignals::getInstance()->outputMessageSig(0, QString("正在读取变量信息..."));

			//读取变量个数
			int varCount;
			_readf.read((char*)&varCount, sizeof(int));
			//_byteAmount += sizeof(int);

			for (int i = 0; i < varCount; ++i)
			{
				//读取结果类型
				int restype;
				_readf.read((char*)&restype, sizeof(int));
				//_byteAmount += sizeof(int);

				if (restype == 0)
				{
					VariableInfo varinfo;
					varinfo._resultType = restype;

					//读取变量名
					char varname[64];
					_readf.read(varname, 64 * sizeof(char));
					//_byteAmount += 64 * sizeof(char);
					varinfo._varName = QString(varname);

					//读取类型
					int vartype;
					_readf.read((char*)&vartype, sizeof(int));
					//_byteAmount += sizeof(int);
					varinfo._varType = vartype;

					//读取变量个数
					int varcount;
					_readf.read((char*)&varcount, sizeof(int));
					//_byteAmount += sizeof(int);
					varinfo._varCount = varcount;

					//读取每个变量的层数及层名称
					QList<int> nlayerlist;
					for (int j = 0; j < varcount; ++j)
					{
						int nlayer;
						_readf.read((char*)&nlayer, sizeof(int));
						//_byteAmount += sizeof(int);
						nlayerlist.append(nlayer);

						QStringList lnames;
						for (int k = 0; k < nlayer; ++k)
						{
							char cstr[32];
							_readf.read(cstr, 32 * sizeof(char));
							//_byteAmount += 32 * sizeof(char);
							lnames.append(QString(cstr));
						}
						varinfo._varLayerNames.append(lnames);
					}
					varinfo._varLayer = nlayerlist;

					//读取分量数
					int ncompo;
					_readf.read((char*)&ncompo, sizeof(int));
					//_byteAmount += sizeof(int);
					varinfo._nCompo = ncompo;

					//读取是否计算矢量模
					bool ismag;
					_readf.read((char*)&ismag, sizeof(bool));
					//_byteAmount += sizeof(bool);
					varinfo._isMag = ismag;

					//读取分量名
					QStringList componames;
					QHash<QString, bool> compoplex;
					for (int j = 0; j < ncompo; ++j)
					{
						//分量名
						char cstr[32];
						_readf.read(cstr, 32 * sizeof(char));
						//_byteAmount += 32 * sizeof(char);
						componames.append(QString(cstr));

						//是否为复数
						bool bpara = false;
						_readf.read((char*)&bpara, sizeof(bool));
						//_byteAmount += sizeof(bool);
						compoplex.insert(QString(cstr), bpara);
					}
					varinfo._compoNames = componames;
					varinfo._compoComplex = compoplex;

					//读取该变量开始的字节位置
					long long int varbyte;
					_readf.read((char*)&varbyte, sizeof(long long int));
					//_byteAmount += sizeof(long long int);
					varinfo._varByte = varbyte;

					//存储
					frameInfo._frameVars.append(varinfo);

					//跳过具体数值
					for (int j = 0; j < varcount; ++j)
					{
						//读取节点或单元数
						int nloop;
						_readf.read((char*)&nloop, sizeof(int));

						//计算需要跳过的字节数并跳过
						int ndouble = ncompo;
						if (ncompo == 0)
						{
							ndouble = 1;
						}
						int nlayer = nlayerlist.at(j);
						if (nlayer != 0)
						{
							ndouble = ncompo * nlayer;
						}
						int skipbyte = nloop * (sizeof(int) + 2 * ndouble * sizeof(double));
						_readf.seekg(skipbyte, ios::cur);
						//_byteAmount += skipbyte;
					}
				}
				else if (restype == 1)
				{
					//读取变量名
					char namestr[64];
					_readf.read(namestr, 64 * sizeof(char));
					//_byteAmount += 64 * sizeof(char);
					QString name = QString(namestr);

					if (name == QString("special_frequency"))			//频率值
					{
						SpecialFrequency freq;

						double val[2];
						_readf.read((char*)val, 2 * sizeof(double));
						//_byteAmount += 2 * sizeof(double);
						freq._modalFreq = val[0];
						freq._modalEigen = val[1];

						//存储
						frameInfo._isFrequency = true;
						frameInfo._frequency = freq;
					}
					if (name == QString("complex_modes"))			//频率值
					{
						ComplexModes cmode;

						double val[4];
						_readf.read((char*)val, 4 * sizeof(double));
						//_byteAmount += 2 * sizeof(double);
						cmode._realVal = val[0];
						cmode._virtualVal = val[1];
						cmode._freqVal = val[2];
						cmode._dampVal = val[3];

						//存储
						frameInfo._isComplexMode = true;
						frameInfo._compMode = cmode;
					}
					else if (name == QString("EM_force"))
					{
						EMForce emf;

						//名称
						char fnamechar[64];
						_readf.read(fnamechar, 64 * sizeof(char));
						emf._name = QString(fnamechar);

						//值
						_readf.read((char*)(emf.val), 3 * sizeof(double));

						//存储
						frameInfo._isEMForce = true;
						frameInfo._emforce.append(emf);
					}
					else if (name == QString("EM_torque"))
					{
						EMTorque emt;

						//名称
						char fnamechar[64];
						_readf.read(fnamechar, 64 * sizeof(char));
						emt._name = QString(fnamechar);

						//值
						_readf.read((char*)(emt.val), 3 * sizeof(double));

						//存储
						frameInfo._isEMTorque = true;
						frameInfo._emtorque.append(emt);
					}
					else if (name == QString("EM_eddycurrentloss"))
					{
						EMEddyCurrentLoss emecl;

						//名称
						char fnamechar[64];
						_readf.read(fnamechar, 64 * sizeof(char));
						emecl._name = QString(fnamechar);

						//值
						_readf.read((char*)&(emecl.val), sizeof(double));

						//存储
						frameInfo._isEMEddyCurrentLoss = true;
						frameInfo._emeddyloss.append(emecl);
					}
					else if (name == QString("EM_energy"))
					{
						EMEnergy eme;

						//名称
						char fnamechar[64];
						_readf.read(fnamechar, 64 * sizeof(char));
						eme._name = QString(fnamechar);

						//值
						_readf.read((char*)&(eme.val), sizeof(double));

						//存储
						frameInfo._isEMEnergy = true;
						frameInfo._emenergy.append(eme);
					}
					else if (name == QString("EM_inductance"))
					{
						EMInductance emi;

						//名称
						char fnamechar[64];
						_readf.read(fnamechar, 64 * sizeof(char));
						emi._name = QString(fnamechar);

						//矩阵的行列数
						int nrow;
						_readf.read((char*)&nrow, sizeof(int));

						//矩阵行列名称
						for (int mi = 0; mi < nrow; ++mi)
						{
							char mchar[64];
							_readf.read(mchar, 64 * sizeof(char));
							emi._matrixtitle.append(QString(mchar));
						}

						//值
						double* vals = new double[nrow*nrow];
						_readf.read((char*)vals, nrow*nrow * sizeof(double));
						emi.val = vals;

						//存储
						frameInfo._isEMInductance = true;
						frameInfo._eminductance.append(emi);
					}
					else if (name == QString("EM_capacitance"))
					{
						EMCapacitance emc;

						//名称
						char fnamechar[64];
						_readf.read(fnamechar, 64 * sizeof(char));
						emc._name = QString(fnamechar);

						//矩阵的行列数
						int nrow;
						_readf.read((char*)&nrow, sizeof(int));

						//矩阵行列名称
						for (int mi = 0; mi < nrow; ++mi)
						{
							char mchar[64];
							_readf.read(mchar, 64 * sizeof(char));
							emc._matrixtitle.append(QString(mchar));
						}

						//值
						double* vals = new double[nrow*nrow];
						_readf.read((char*)vals, nrow*nrow * sizeof(double));
						emc.val = vals;

						//qDebug() << vals[0] << " " <<  vals[1] << " " << vals[2] << endl;
						//qDebug() << vals[3] << " " << vals[4] << " " << vals[5] << endl;
						//qDebug() << vals[6] << " " << vals[7] << " " << vals[8] << endl;

						//存储
						frameInfo._isEMCapacitance = true;
						frameInfo._emcapacitance.append(emc);
					}
					else if (name == QString("EM_ohmicloss"))
					{
						EMOhmicLoss eme;

						//名称
						char fnamechar[64];
						_readf.read(fnamechar, 64 * sizeof(char));
						eme._name = QString(fnamechar);

						//值
						_readf.read((char*)&(eme.val), sizeof(double));

						//存储
						frameInfo._isEMOhmicLoss = true;
						frameInfo._emohmicloss.append(eme);
					}
					else if (name == QString("EM_hysteresisloss"))
					{
						EMHysteresisLoss eme;

						//名称
						char fnamechar[64];
						_readf.read(fnamechar, 64 * sizeof(char));
						eme._name = QString(fnamechar);

						//值
						_readf.read((char*)&(eme.val), sizeof(double));

						//存储
						frameInfo._isEMHysteresisLoss = true;
						frameInfo._emhysteresisloss.append(eme);
					}
					else if (name == QString("RigidWallPos"))
					{
						int rigidWallNum = _dp->getRigidWallDataCount();
						for (int j = 0; j < rigidWallNum; j++)
						{
							//ID
							int id;
							_readf.read((char*)&id, sizeof(int));

							//当前位置
							double endpos[3];
							_readf.read((char*)endpos, 3 * sizeof(double));

							//framedata->setRigidWallEndPos(id, QVector3D(endpos[0], endpos[1], endpos[2]));
						}
					}
				}
				else if (restype == 2)
				{
					//读取载荷名
					char loadname[64];
					_readf.read(loadname, 64 * sizeof(char));
					QString namestr = QString(loadname);

					//读取字节位置
					long long int varbyte;
					_readf.read((char*)&varbyte, sizeof(long long int));

					//存储
					frameInfo._loadResults.insert(namestr, varbyte);

					//根据载荷名称跳过字节数
					if (namestr.compare("ShellStrain", Qt::CaseInsensitive) == 0 ||
						namestr.compare("ShellStress", Qt::CaseInsensitive) == 0)		//壳单元应变和应力
					{
						//获取壳单元个数
						int nshell = framedata->getNumByElementType(CPDSG) + framedata->getNumByElementType(MITC4)+
							framedata->getNumByElementType(TRIA3)+ framedata->getNumByElementType(QPH);

						int nlayer = 0;
						for (int ish = 0; ish < nshell; ++ish)
						{
							//跳过ID
							_readf.seekg(sizeof(int), ios::cur);

							//读取层数
							int shelllayer;
							_readf.read((char*)&shelllayer, sizeof(int));

							//取层数最大值（所有帧的壳层数均一致，因此只统计第一帧）
							if (fi == 0)
							{
								nlayer = max(nlayer, shelllayer);
							}

							//跳过值
							_readf.seekg(shelllayer* 9 * sizeof(double), ios::cur);
						}
						if (fi == 0)
						{
							_dp->appendShellLayer(namestr, nlayer);
						}
					}
					else if (namestr.compare("BarEndLoad", Qt::CaseInsensitive) == 0)		//杆轴力-梁单元和杆单元
					{
						//获取杆单元和梁单元个数
						int nbar = framedata->getNumByElementType(D3R2) + framedata->getNumByElementType(D3B2);

						//跳过字节数
						_readf.seekg(nbar * (sizeof(int) + 4 * sizeof(double)), ios::cur);
					}
					else if (namestr.compare("BarStress", Qt::CaseInsensitive) == 0)	//梁单元应力
					{
						//获取梁单元个数
						int nbar = framedata->getNumByElementType(D3B2);

						//跳过字节数
						_readf.seekg(nbar * (sizeof(int) + 13 * sizeof(double)), ios::cur);
					}
					else if (namestr.compare("ShearStress", Qt::CaseInsensitive) == 0)	//剪切板应力
					{
						//获取梁单元个数
						int nshear = framedata->getNumByElementType(SPQ4);

						//跳过字节数
						_readf.seekg(nshear * (sizeof(int) + 3 * sizeof(double)), ios::cur);
					}
					else if (namestr.compare("ShearLoad", Qt::CaseInsensitive) == 0)	//剪切板载荷
					{
						//获取梁单元个数
						int nshear = framedata->getNumByElementType(SPQ4);

						//跳过字节数
						_readf.seekg(nshear * (sizeof(int) + 4 * sizeof(double)), ios::cur);
					}
					else if (namestr.compare("ShellForceResultant", Qt::CaseInsensitive) == 0)	//壳单元载荷
					{
						//获取壳单元个数
						int nshell = framedata->getNumByElementType(CPDSG) + framedata->getNumByElementType(MITC4)+
							framedata->getNumByElementType(TRIA3) + framedata->getNumByElementType(QPH);

						//跳过字节数
						_readf.seekg(nshell * (sizeof(int) + 18 * sizeof(double)), ios::cur);
					}
					else if (namestr.compare("ShellForceMoment", Qt::CaseInsensitive) == 0)	//壳单元力矩
					{
						//获取壳单元个数
						int nshell = framedata->getNumByElementType(CPDSG) + framedata->getNumByElementType(MITC4)+
							framedata->getNumByElementType(TRIA3) + framedata->getNumByElementType(QPH);

						//跳过字节数
						_readf.seekg(nshell * (sizeof(int) + 18 * sizeof(double)), ios::cur);
					}
				}
			}

			//存储
			_dp->appendFrameInfo(frameInfo);

			//结束标记
			int endtag;
			_readf.read((char*)&endtag, sizeof(int));
			//_byteAmount += sizeof(int);
			if (endtag == 0xA1B1C1D2)
			{
				emit readFileSuccessSig(framefile);
			}

			//关闭文件
			_readf.close();
		}

		//更新数据库所有部件名称
		//datapost->updateAllPostPartNames();

		emit finishedImportAllMxDbFileSig();
	}

	//void mIMxdbFile1::slot_importOneFrameData(mPostAnimationRendData1 *animationData, int f, int threadNum, const QString & postname, const QString & var, const QString & compo, const QString & layer, const QString & complex, bool hasstreamLine, QString streamLineVectorName, bool hasvectorArrowVector, QString vectorArrowVectorName)
	//{
	//	_animationData = animationData; 
	//	_threadNum = threadNum; 
	//	_frame = f;
	//	_pname = postname, _var = var, _compo = compo, _layer = layer, _complex = complex; _hasstreamLine = hasstreamLine; _streamLineVectorName = streamLineVectorName;  _hasvectorArrowVector = hasvectorArrowVector; _vectorArrowVectorName = vectorArrowVectorName;
	//}

	bool mIMxdbFile1::readCloudData()
	{
		QString complex = "实部";
		int layer = 0;
		int f = 1;
		//mDataPost1* dp = mDatabaseSingleton::getInstance()->getDataPost(pname);
		if (_dp == nullptr)
		{
			return false;
		}
		QList<FrameInfo> frameInfo = _dp->getFrameInfos();
		if (f < 1 || f > frameInfo.size())
		{
			return false;
		}

		FrameInfo frameinfo = frameInfo.at(f - 1);
		const int nvar = frameinfo._frameVars.size();
		for (int i = 0; i < nvar; ++i)
		{
			VariableInfo varinfo = frameinfo._frameVars.at(i);
			//if (varinfo._varName == var)
			{
				//打开文件
				bool isopen = openFileStatus(frameinfo._frameFile);
				if (!isopen)
				{
					return false;
				}

				//获取变量的字节位置
				long long int varbyte = varinfo._varByte;
				_readf.seekg(varbyte, ios::beg);

				//获取分量数
				const int ncompo = varinfo._nCompo;

				//获取当前分量的索引
				const int compoi = 0;

				//存储值
				double maxval = DBL_MIN, minval = DBL_MAX;
				QHash<int, float> compovalues;

				//获取当前变量中的变量个数
				const int nresult = varinfo._varCount;
				for (int k = 0; k < nresult; ++k)
				{
					//获取层数及层索引
					const int nlayer = varinfo._varLayer.at(k);
					const int layerindex = 0;
					//计算分量值个数及所需值的索引
					int nval, valindex;
					if (nlayer == 0)
					{
						if (ncompo == 0)
						{
							nval = 1;
							valindex = 0;
						}
						else
						{
							nval = ncompo;
							valindex = compoi;
						}
					}
					else
					{
						if (ncompo == 0)
						{
							nval = nlayer;
							valindex = layerindex + compoi;
						}
						else
						{
							nval = ncompo * nlayer;
							valindex = ncompo * layerindex + compoi;
						}
					}

					//读取个数
					int loopcount;
					_readf.read((char*)&loopcount, sizeof(int));

					//读取变量值
					for (int j = 0; j < loopcount; ++j)
					{
						//ID
						int id;
						_readf.read((char*)&id, sizeof(int));

						//值（实部）
						double* val = new double[nval];
						_readf.read((char*)val, nval * sizeof(double));

						//值（虚部）
						double* val1 = new double[nval];
						_readf.read((char*)val1, nval * sizeof(double));

						double tempval;
						if (complex == QString("实部"))
						{
							tempval = val[valindex];
						}
						else if (complex == QString("虚部"))
						{
							tempval = val1[valindex];
						}
						else if (complex == QString("幅值"))
						{
							double v0 = val[valindex];
							double v1 = val1[valindex];
							tempval = sqrt(v0*v0 + v1 * v1);
						}
						else if (complex == QString("相位"))
						{
							double v0 = val[valindex];
							double v1 = val1[valindex];
							tempval = atan(v0 / v1) * 180 / 3.1415926 + 180;
						}
						else
						{
							if (valindex == -1 && nval == 3)
							{
								tempval = sqrt(val[0] * val[0] + val[1] * val[1] + val[2] * val[2] +
									val1[0] * val1[0] + val1[1] * val1[1] + val1[2] * val1[2]);
							}
							else
							{
								tempval = val[valindex];
							}
						}
						compovalues.insert(id, tempval);

						//取最值
						//if (j == 0)
						//{
							//maxval = tempval;
							//minval = tempval;
						//}
						//else
						//{
							maxval = (tempval > maxval) ? tempval : maxval;
							minval = (tempval < minval) ? tempval : minval;
						//}

						delete val;
						delete val1;
					}
				}

				//颜色条标题
				//QString titlestr = var;
				//if (ncompo > 0)
				//{
				//	if (compoi == -1)
				//	{
				//		titlestr = titlestr + "-Mag";
				//	}
				//	else
				//	{
				//		titlestr = titlestr + "-" + compo;
				//	}
				//}
				//if (!layer.isEmpty())
				//{
				//	titlestr = titlestr + ":" + layer;
				//}

				//compovalues.clear();

				//存储数据
				//if (isani)
				//{
				//	if (_animateRender != nullptr)
				//	{
				//		mPostOneFrameData* oneframe = new mPostOneFrameData;
				//		oneframe->setOrignalMaxMinData(maxval, minval);
				//		oneframe->setRendData(f, compovalues, titlestr);
				//		_animateRender->appendRendOneFrameData(f, oneframe);
				//	}
				//}
				//else
				//{
				if (_oneFrameRender != nullptr)
				{
					_oneFrameRender->setOrignalMaxMinData(maxval, minval);
					_oneFrameRender->setRendData(1, compovalues, "");
					_oneFrameRender->setNodeOrElement(MViewBasic::NodeOrElement(varinfo._varType));
				}
				//}

				compovalues.clear();

				_readf.close();

				//输出信息
				//mOutputMessage tepmsg(NormalMessage, QString("第%1帧文件数据读取成功！").arg(f));
				//mGlobalSignals::getInstance()->outputMessageSig(tepmsg);

				QList<FrameInfo>().swap(frameInfo);
				frameInfo.clear();
				return true;
			}
		}
		return false;
	}

	void mIMxdbFile1::readDisplacementData()
	{
		//mDataPost1* dp = mDatabaseSingleton::getInstance()->getDataPost(pname);
		if (_dp == nullptr)
		{
			return;
		}
		QList<FrameInfo> frameInfo = _dp->getFrameInfos();
		int f = 1;
		QString complexstr = "实部";
		if (f < 1 || f > frameInfo.size())
		{
			return;
		}
		FrameInfo frameinfo = frameInfo.at(f - 1);
		const int nvar = frameinfo._frameVars.size();
		for (int i = 0; i < nvar; ++i)
		{
			VariableInfo varinfo = frameinfo._frameVars.at(i);
			if (varinfo._varName == QString("Displacement"))
			{
				//打开文件
				bool isopen = openFileStatus(frameinfo._frameFile);
				if (!isopen)
				{
					return;
				}

				//获取变量的字节位置
				long long int varbyte = varinfo._varByte;
				_readf.seekg(varbyte, ios::beg);

				//读取节点数
				int nodecount;
				_readf.read((char*)&nodecount, sizeof(int));
				
				QHash<int, QVector3D> dispvals;
				dispvals.reserve(nodecount);
				for (int j = 0; j < nodecount; ++j)
				{
					//节点ID
					int nid;
					_readf.read((char*)&nid, sizeof(int));

					//位移值（实部）
					double val[3];
					_readf.read((char*)val, 3 * sizeof(double));

					//位移值（虚部）
					double val1[3];
					_readf.read((char*)val1, 3 * sizeof(double));

					QVector3D valvec(val[0], val[1], val[2]);
					if (complexstr == QString("实部"))
					{
						valvec = QVector3D(val[0], val[1], val[2]);
					}
					else if (complexstr == QString("虚部"))
					{
						valvec = QVector3D(val1[0], val1[1], val1[2]);
					}
					else if (complexstr == QString("幅值"))
					{
						double magx = sqrt(val[0] * val[0] + val1[0] * val1[0]);
						double magy = sqrt(val[1] * val[1] + val1[1] * val1[1]);
						double magz = sqrt(val[2] * val[2] + val1[2] * val1[2]);
						valvec = QVector3D(magx, magy, magz);
					}
					else if (complexstr == QString("相位"))
					{
						double magx = atan(val[0]/val1[0]);
						double magy = atan(val[1]/val1[1]);
						double magz = atan(val[2]/val1[2]);
						valvec = QVector3D(magx, magy, magz);
					}

					dispvals.insert(nid, valvec);
				}

				//存储数据
				//if (isani)
				//{
				//	if (_animateRender != nullptr)
				//	{
				//		mPostOneFrameData* oneframe = _animateRender->getRendOneFrameData(f);
				//		if (oneframe != nullptr)
				//		{
				//			oneframe->setNodeDisplacementData(dispvals);
				//		}
				//	}
				//}
				//else
				{
					if (_oneFrameRender != nullptr)
					{
						_oneFrameRender->setNodeDisplacementData(dispvals);
					}
				}

				dispvals.clear();

				//输出信息
				//mOutputMessage tepmsg(NormalMessage, QString("第%1帧文件位移数据读取成功！").arg(f));
				//mGlobalSignals::getInstance()->outputMessageSig(tepmsg);

				_readf.close();
				return;
			}
		}
	}

	void mIMxdbFile1::readVectorData(const QString& pname, int f, const QString & var)
	{
		//mDataPost1* dp = mDatabaseSingleton::getInstance()->getDataPost(pname);
		if (_dp == nullptr)
		{
			return;
		}
		QList<FrameInfo> frameInfo = _dp->getFrameInfos();
		if (f < 1 || f > frameInfo.size())
		{
			return;
		}
		FrameInfo frameinfo = frameInfo.at(f - 1);
		const int nvar = frameinfo._frameVars.size();
		for (int i = 0; i < nvar; ++i)
		{
			VariableInfo varinfo = frameinfo._frameVars.at(i);
			if (varinfo._varName == var)
			{
				//获取分量数
				const int ncompo = varinfo._nCompo;
				if (ncompo != 3)
				{
					return;
				}

				//打开文件
				bool isopen = openFileStatus(frameinfo._frameFile);
				if (!isopen)
				{
					return;
				}

				//获取变量的字节位置
				long long int varbyte = varinfo._varByte;
				_readf.seekg(varbyte, ios::beg);

				//读取节点/单元数
				int nodecount;
				_readf.read((char*)&nodecount, sizeof(int));

				QHash<int, QVector3D> vecvals;
				vecvals.reserve(nodecount);
				for (int j = 0; j < nodecount; ++j)
				{
					//节点ID
					int nid;
					_readf.read((char*)&nid, sizeof(int));

					//矢量值（实部）
					double val[3];
					_readf.read((char*)val, 3 * sizeof(double));

					//矢量值（虚部）
					double val1[3];
					_readf.read((char*)val1, 3 * sizeof(double));

					vecvals.insert(nid, QVector3D(val[0], val[1], val[2]));
				}

				//获取变量类型
				MViewBasic::NodeOrElement vartype = MViewBasic::NodeOrElement(varinfo._varType);

				//存储
				if (_oneFrameRender != nullptr)
				{
					_oneFrameRender->setRendData(vecvals, vartype);
				}
				//else if (isani&&_animateRender != nullptr)
				//{
				//	mPostOneFrameData* oneframe = _animateRender->getRendOneFrameData(f);
				//	if (oneframe != nullptr)
				//	{
				//		oneframe->setRendData(vecvals, vartype);
				//	}
				//}

				//输出信息
				//mOutputMessage tepmsg(NormalMessage, QString("第%1帧文件矢量数据读取成功！").arg(f));
				//mGlobalSignals::getInstance()->outputMessageSig(tepmsg);

				_readf.close();
				return;
			}
		}
	}

	void mIMxdbFile1::readCurveData1(const QString& pname, const QString & var, const QStringList & compos, const QString & layer, const QString & complex,
		const set<int>& ids, QStringList & cnames, QVector<double>& xvalues, QHash<QString, QVector<double>>& yvalues)
	{
		//mDataPost1* dp = mDatabaseSingleton::getInstance()->getDataPost(pname);
		if (_dp == nullptr)
		{
			return;
		}
		QList<FrameInfo> frameInfo = _dp->getFrameInfos();
		const int nframe = frameInfo.size();
		for (int i = 0; i < nframe; ++i)
		{
			FrameInfo finfo = frameInfo.at(i);
			const int nvar = finfo._frameVars.size();
			for (int ii = 0; ii < nvar; ++ii)
			{
				VariableInfo varinfo = finfo._frameVars.at(ii);
				if (varinfo._varName == var)
				{
					//打开文件
					bool isopen = openFileStatus(finfo._frameFile);
					if (!isopen)
					{
						return;
					}

					//获取变量的字节位置
					long long int varbyte = varinfo._varByte;
					_readf.seekg(varbyte, ios::beg);

					//获取分量数
					const int ncompo = varinfo._nCompo;

					//生成X数据
					xvalues.append(finfo._frameTime);

					//获取当前变量中的变量个数
					const int nresult = varinfo._varCount;
					for (int k = 0; k < nresult; ++k)
					{
						//获取层数及层索引
						const int nlayer = varinfo._varLayer.at(k);
						const int layerindex = varinfo._varLayerNames.at(k).indexOf(layer);
						//计算分量值个数
						int nval;
						if (nlayer == 0)
						{
							if (ncompo == 0)
							{
								nval = 1;
							}
							else
							{
								nval = ncompo;
							}
						}
						else
						{
							if (ncompo == 0)
							{
								nval = nlayer;
							}
							else
							{
								nval = ncompo * nlayer;
							}
						}

						//读取个数
						int loopcount;
						_readf.read((char*)&loopcount, sizeof(int));

						//读取变量值
						for (int j = 0; j < loopcount; ++j)
						{
							//ID
							int id;
							_readf.read((char*)&id, sizeof(int));

							if (ids.find(id) != ids.end())
							{
								//值（实部）
								double* val = new double[nval];
								_readf.read((char*)val, nval * sizeof(double));

								//值（虚部）
								double* val1 = new double[nval];
								_readf.read((char*)val1, nval * sizeof(double));

								for (QString compo : compos)
								{
									//曲线名称
									QString cname = QString("Time-%1_%2_%3_%4-ID%5").arg(var).arg(compo).arg(layer).arg(complex).arg(id);
									if (!cnames.contains(cname))
									{
										cnames.append(cname);
									}

									//获取当前分量的索引
									const int compoi = varinfo._compoNames.indexOf(compo);
									//计算分量的索引
									int valindex;
									if (nlayer == 0)
									{
										if (ncompo == 0)
										{
											valindex = 0;
										}
										else
										{
											valindex = compoi;
										}
									}
									else
									{
										if (ncompo == 0)
										{
											valindex = layerindex + compoi;
										}
										else
										{
											valindex = ncompo * layerindex + compoi;
										}
									}
									//取值
									double tempval;
									if (complex == QString("实部"))
									{
										tempval = val[valindex];
									}
									else if (complex == QString("虚部"))
									{
										tempval = val1[valindex];
									}
									else if (complex == QString("幅值"))
									{
										double v0 = val[valindex];
										double v1 = val1[valindex];
										tempval = sqrt(v0*v0 + v1 * v1);
									}
									else if (complex == QString("相位"))
									{
										double v0 = val[valindex];
										double v1 = val1[valindex];
										tempval = atan(v0 / v1) * 180 / 3.1415926 + 180;
									}
									else
									{
										if (valindex == -1 && nval == 3)
										{
											tempval = sqrt(val[0] * val[0] + val[1] * val[1] + val[2] * val[2] +
												val1[0] * val1[0] + val1[1] * val1[1] + val1[2] * val1[2]);
										}
										else
										{
											tempval = val[valindex];
										}
									}
									//存储
									QVector<double> tempvec;
									if (yvalues.contains(cname))
									{
										tempvec = yvalues.value(cname);
									}
									tempvec.append(tempval);
									yvalues.insert(cname, tempvec);
								}
							}
							else
							{
								_readf.seekg(2 * nval * sizeof(double), ios::cur);
							}
						}
					}

					//关闭文件
					_readf.close();
				}
			}
		}
	}

	void mIMxdbFile1::readCurveData2(const QString& pname, const QString & var, const QStringList & compos, const QString & layer, const QString & complex, const QVector<int>& ids,
		const set<int>& frameIds, QStringList & cnames, QHash<QString, QHash<int, double>>& yvalues)
	{
		//mDataPost1* dp = mDatabaseSingleton::getInstance()->getDataPost(pname);
		if (_dp == nullptr)
		{
			return;
		}
		QList<FrameInfo> frameInfo = _dp->getFrameInfos();
		const int nframe = frameInfo.size();
		for (int i = 0; i < nframe; ++i)
		{
			FrameInfo finfo = frameInfo.at(i);
			if (frameIds.find(finfo._frameNum) != frameIds.end())
			{
				const int nvar = finfo._frameVars.size();
				for (int ii = 0; ii < nvar; ++ii)
				{
					VariableInfo varinfo = finfo._frameVars.at(ii);
					if (varinfo._varName == var)
					{
						//打开文件
						bool isopen = openFileStatus(finfo._frameFile);
						if (!isopen)
						{
							return;
						}

						//获取变量的字节位置
						long long int varbyte = varinfo._varByte;
						_readf.seekg(varbyte, ios::beg);

						//获取分量数
						const int ncompo = varinfo._nCompo;

						//获取当前变量中的变量个数
						const int nresult = varinfo._varCount;
						for (int k = 0; k < nresult; ++k)
						{
							//获取层数及层索引
							const int nlayer = varinfo._varLayer.at(k);
							const int layerindex = varinfo._varLayerNames.at(k).indexOf(layer);
							//计算分量值个数及所需值的索引
							int nval;
							if (nlayer == 0)
							{
								if (ncompo == 0)
								{
									nval = 1;
								}
								else
								{
									nval = ncompo;
								}
							}
							else
							{
								if (ncompo == 0)
								{
									nval = nlayer;
								}
								else
								{
									nval = ncompo * nlayer;
								}
							}

							//读取个数
							int loopcount;
							_readf.read((char*)&loopcount, sizeof(int));

							//读取变量值
							for (int j = 0; j < loopcount; ++j)
							{
								//ID
								int id;
								_readf.read((char*)&id, sizeof(int));

								if (ids.contains(id))
								{
									//值（实部）
									double* val = new double[nval];
									_readf.read((char*)val, nval * sizeof(double));

									//值（虚部）
									double* val1 = new double[nval];
									_readf.read((char*)val1, nval * sizeof(double));

									for (QString compo : compos)
									{
										//曲线名称
										QString cname = QString("ID-%1_%2_%3_%4-f%5").arg(var).arg(compo).arg(layer).arg(complex).arg(finfo._frameNum);
										if (!cnames.contains(cname))
										{
											cnames.append(cname);
										}

										//获取当前分量的索引
										const int compoi = varinfo._compoNames.indexOf(compo);
										//计算分量的索引
										int valindex;
										if (nlayer == 0)
										{
											if (ncompo == 0)
											{
												valindex = 0;
											}
											else
											{
												valindex = compoi;
											}
										}
										else
										{
											if (ncompo == 0)
											{
												valindex = layerindex + compoi;
											}
											else
											{
												valindex = ncompo * layerindex + compoi;
											}
										}
										//取值
										double tempval;
										if (complex == QString("实部"))
										{
											tempval = val[valindex];
										}
										else if (complex == QString("虚部"))
										{
											tempval = val1[valindex];
										}
										else if (complex == QString("幅值"))
										{
											double v0 = val[valindex];
											double v1 = val1[valindex];
											tempval = sqrt(v0*v0 + v1 * v1);
										}
										else if (complex == QString("相位"))
										{
											double v0 = val[valindex];
											double v1 = val1[valindex];
											tempval = atan(v0 / v1) * 180 / 3.1415926 + 180;
										}
										else
										{
											if (valindex == -1 && nval == 3)
											{
												tempval = sqrt(val[0] * val[0] + val[1] * val[1] + val[2] * val[2] +
													val1[0] * val1[0] + val1[1] * val1[1] + val1[2] * val1[2]);
											}
											else
											{
												tempval = val[valindex];
											}
										}
										//存储
										QHash<int, double> tempvec;
										if (yvalues.contains(cname))
										{
											tempvec = yvalues.value(cname);
										}
										tempvec.insert(id, tempval);
										yvalues.insert(cname, tempvec);
									}
								}
								else
								{
									_readf.seekg(2 * nval * sizeof(double), ios::cur);
								}
							}
						}

						//关闭文件
						_readf.close();
					}
				}
			}
		}
	}

	void mIMxdbFile1::readCurveData3(const QString& pname, const QString & varx, const QString & compox, const QString & layerx, const QString & complexx,
		const QString & vary, const QStringList & composy, const QString & layery, const QString & complexy, 
		const set<int>& ids, QStringList & cnames, QVector<double>& xvalues, QHash<QString, QVector<double>>& yvalues)
	{
		//mDataPost1* dp = mDatabaseSingleton::getInstance()->getDataPost(pname);
		if (_dp == nullptr)
		{
			return;
		}
		QList<FrameInfo> frameInfo = _dp->getFrameInfos();
		const int nframe = frameInfo.size();
		for (int i = 0; i < nframe; ++i)
		{
			FrameInfo finfo = frameInfo.at(i);
			const int nvar = finfo._frameVars.size();
			for (int ii = 0; ii < nvar; ++ii)
			{
				VariableInfo varinfo = finfo._frameVars.at(ii);

				//X值
				if (varinfo._varName == varx)
				{
					//打开文件
					bool isopen = openFileStatus(finfo._frameFile);
					if (!isopen)
					{
						return;
					}

					//获取变量的字节位置
					long long int varbyte = varinfo._varByte;
					_readf.seekg(varbyte, ios::beg);

					//获取分量数
					const int ncompo = varinfo._nCompo;

					//获取当前变量中的变量个数
					const int nresult = varinfo._varCount;
					for (int k = 0; k < nresult; ++k)
					{
						//获取层数及层索引
						const int nlayer = varinfo._varLayer.at(k);
						const int layerindex = varinfo._varLayerNames.at(k).indexOf(layerx);
						//计算分量值个数及所需值的索引
						int nval;
						if (nlayer == 0)
						{
							if (ncompo == 0)
							{
								nval = 1;
							}
							else
							{
								nval = ncompo;
							}
						}
						else
						{
							if (ncompo == 0)
							{
								nval = nlayer;
							}
							else
							{
								nval = ncompo * nlayer;
							}
						}

						//读取个数
						int loopcount;
						_readf.read((char*)&loopcount, sizeof(int));

						//读取变量值
						for (int j = 0; j < loopcount; ++j)
						{
							//ID
							int id;
							_readf.read((char*)&id, sizeof(int));

							if (ids.find(id) != ids.end())
							{
								//值（实部）
								double* val = new double[nval];
								_readf.read((char*)val, nval * sizeof(double));

								//值（虚部）
								double* val1 = new double[nval];
								_readf.read((char*)val1, nval * sizeof(double));

								//获取当前分量的索引
								const int compoi = varinfo._compoNames.indexOf(compox);
								//计算分量的索引
								int valindex;
								if (nlayer == 0)
								{
									if (ncompo == 0)
									{
										valindex = 0;
									}
									else
									{
										valindex = compoi;
									}
								}
								else
								{
									if (ncompo == 0)
									{
										valindex = layerindex + compoi;
									}
									else
									{
										valindex = ncompo * layerindex + compoi;
									}
								}
								//取值
								double tempval;
								if (complexx == QString("实部"))
								{
									tempval = val[valindex];
								}
								else if (complexx == QString("虚部"))
								{
									tempval = val1[valindex];
								}
								else if (complexx == QString("幅值"))
								{
									double v0 = val[valindex];
									double v1 = val1[valindex];
									tempval = sqrt(v0*v0 + v1 * v1);
								}
								else if (complexx == QString("相位"))
								{
									double v0 = val[valindex];
									double v1 = val1[valindex];
									tempval = atan(v0 / v1) * 180 / 3.1415926 + 180;
								}
								else
								{
									if (valindex == -1 && nval == 3)
									{
										tempval = sqrt(val[0] * val[0] + val[1] * val[1] + val[2] * val[2] +
											val1[0] * val1[0] + val1[1] * val1[1] + val1[2] * val1[2]);
									}
									else
									{
										tempval = val[valindex];
									}
								}
								//存储
								xvalues.append(tempval);
							}
							else
							{
								_readf.seekg(2 * nval * sizeof(double), ios::cur);
							}
						}
					}

					//关闭文件
					_readf.close();
				}

				//Y值
				if (varinfo._varName == vary)
				{
					//打开文件
					bool isopen = openFileStatus(finfo._frameFile);
					if (!isopen)
					{
						return;
					}

					//获取变量的字节位置
					long long int varbyte = varinfo._varByte;
					_readf.seekg(varbyte, ios::beg);

					//获取分量数
					const int ncompo = varinfo._nCompo;

					//获取当前变量中的变量个数
					const int nresult = varinfo._varCount;
					for (int k = 0; k < nresult; ++k)
					{
						//获取层数及层索引
						const int nlayer = varinfo._varLayer.at(k);
						const int layerindex = varinfo._varLayerNames.at(k).indexOf(layery);
						//计算分量值个数及所需值的索引
						int nval;
						if (nlayer == 0)
						{
							if (ncompo == 0)
							{
								nval = 1;
							}
							else
							{
								nval = ncompo;
							}
						}
						else
						{
							if (ncompo == 0)
							{
								nval = nlayer;
							}
							else
							{
								nval = ncompo * nlayer;
							}
						}

						//读取个数
						int loopcount;
						_readf.read((char*)&loopcount, sizeof(int));

						//读取变量值
						for (int j = 0; j < loopcount; ++j)
						{
							//ID
							int id;
							_readf.read((char*)&id, sizeof(int));

							if (ids.find(id) != ids.end())
							{
								//值（实部）
								double* val = new double[nval];
								_readf.read((char*)val, nval * sizeof(double));

								//值（虚部）
								double* val1 = new double[nval];
								_readf.read((char*)val1, nval * sizeof(double));

								for (QString compo : composy)
								{
									//曲线名称
									QString cname = QString("%1_%2_%3_%4-%5_%6_%7_%8-ID%9").arg(varx).arg(compox).arg(layerx).arg(complexx)
										.arg(vary).arg(compo).arg(layery).arg(complexy).arg(id);
									if (!cnames.contains(cname))
									{
										cnames.append(cname);
									}

									//获取当前分量的索引
									const int compoi = varinfo._compoNames.indexOf(compo);
									//计算分量的索引
									int valindex;
									if (nlayer == 0)
									{
										if (ncompo == 0)
										{
											valindex = 0;
										}
										else
										{
											valindex = compoi;
										}
									}
									else
									{
										if (ncompo == 0)
										{
											valindex = layerindex + compoi;
										}
										else
										{
											valindex = ncompo * layerindex + compoi;
										}
									}
									//取值
									double tempval;
									if (complexy == QString("实部"))
									{
										tempval = val[valindex];
									}
									else if (complexy == QString("虚部"))
									{
										tempval = val1[valindex];
									}
									else if (complexy == QString("幅值"))
									{
										double v0 = val[valindex];
										double v1 = val1[valindex];
										tempval = sqrt(v0*v0 + v1 * v1);
									}
									else if (complexy == QString("相位"))
									{
										double v0 = val[valindex];
										double v1 = val1[valindex];
										tempval = atan(v0 / v1) * 180 / 3.1415926 + 180;
									}
									else
									{
										if (valindex == -1 && nval == 3)
										{
											tempval = sqrt(val[0] * val[0] + val[1] * val[1] + val[2] * val[2] +
												val1[0] * val1[0] + val1[1] * val1[1] + val1[2] * val1[2]);
										}
										else
										{
											tempval = val[valindex];
										}
									}
									//存储
									QVector<double> tempvec;
									if (yvalues.contains(cname))
									{
										tempvec = yvalues.value(cname);
									}
									tempvec.append(tempval);
									yvalues.insert(cname, tempvec);
								}
							}
							else
							{
								_readf.seekg(2 * nval * sizeof(double), ios::cur);
							}
						}
					}

					//关闭文件
					_readf.close();
				}
			}
		}
	}

	void mIMxdbFile1::readLoadData(const QString& pname, const QStringList & casenames, std::set<int>& ids, const QString& type)
	{
		//mDataPost1* dp = mDatabaseSingleton::getInstance()->getDataPost(pname);
		if (_dp == nullptr)
		{
			return;
		}
		//根据工况名获取帧信息
		for (QString cname : casenames)
		{
			int firstbar = cname.indexOf("-");
			QString tempname = cname.right(cname.size() - firstbar - 1);
			FrameInfo fi = _dp->getFrameInfo(tempname);

			//打开文件
			bool isopen = openFileStatus(fi._frameFile);
			if (isopen)
			{
				//获取帧数据
				mOneFrameData1* framedata = _dp->getOneFrameData(fi._frameNum);
				if (framedata == nullptr)
				{
					_readf.close();
					continue;
				}

				//获取载荷数据的字节位置并跳转
				if (!fi._loadResults.contains(type))
				{
					_readf.close();
					continue;
				}
				long long int loadbyte = fi._loadResults.value(type);
				_readf.seekg(loadbyte, ios::beg);

				//读取数据
				std::set<int> eleIds = ids;
				if (type.compare("BarEndLoad", Qt::CaseInsensitive) == 0)
				{
					//存储数据
					//mBarEndLoad* bel = new mBarEndLoad;
					//mSourceData::getInstance()->appendBarEndLoad(cname, bel);

					//读取数据
					const int nbar = framedata->getNumByElementType(D3R2) + framedata->getNumByElementType(D3B2);
					for (int i = 0; i < nbar; ++i)
					{
						//单元ID
						int barid;
						_readf.read((char*)&barid, sizeof(int));

						//判断是否为所需单元
						if (eleIds.count(barid) == 1)
						{
							//值
							double endload[4];
							_readf.read((char*)endload, 4 * sizeof(double));

							//bel->appendBarId(barid);
							//bel->appendEndLoad(barid, endload[0], endload[1]);
							//bel->appendEndLoad1(barid, endload[2], endload[3]);

							//已存储的单元从列表中移除
							eleIds.erase(barid);
						}
						else
						{
							_readf.seekg(4 * sizeof(double), ios::cur);
						}

						//判断所需要的ID是否已全部读取，是则终止读取
						if (eleIds.size() == 0)
						{
							break;
						}
					}
				}
				else if (type.compare("BarStress", Qt::CaseInsensitive) == 0)
				{
					//存储数据
					//mSrcBarS* sbs = new mSrcBarS;
					//mSourceData::getInstance()->appendBarStress(cname, sbs);

					//读取数据
					const int nbar = framedata->getNumByElementType(D3B2);
					for (int i = 0; i < nbar; ++i)
					{
						//单元ID
						int barid;
						_readf.read((char*)&barid, sizeof(int));

						//判断是否为所需单元
						if (eleIds.count(barid) == 1)
						{
							//值
							double vals[13];
							_readf.read((char*)vals, 13 * sizeof(double));

							//sbs->appendBarId(barid);
							//sbs->appendPointC(barid, vals[0], vals[1]);
							//sbs->appendPointD(barid, vals[2], vals[3]);
							//sbs->appendPointE(barid, vals[4], vals[5]);
							//sbs->appendPointF(barid, vals[6], vals[7]);
							//sbs->appendAxial(barid, vals[8]);
							//sbs->appendMaximum(barid, vals[9], vals[10]);
							//sbs->appendMinimum(barid, vals[11], vals[12]);

							//已存储的单元从列表中移除
							eleIds.erase(barid);
						}
						else
						{
							_readf.seekg(13 * sizeof(double), ios::cur);
						}

						//判断所需要的ID是否已全部读取，是则终止读取
						if (eleIds.size() == 0)
						{
							break;
						}
					}
				}
				else if (type.compare("ShearStress", Qt::CaseInsensitive) == 0)
				{
					//存储数据
					//mSrcShearS* sss = new mSrcShearS;
					//mSourceData::getInstance()->appendShearStress(cname, sss);

					//读取数据
					const int nshear = framedata->getNumByElementType(SPQ4);
					for (int i = 0; i < nshear; ++i)
					{
						//单元ID
						int shearid;
						_readf.read((char*)&shearid, sizeof(int));

						//判断是否为所需单元
						if (eleIds.count(shearid) == 1)
						{
							//值
							double vals[3];
							_readf.read((char*)vals, 3 * sizeof(double));

							//sss->appendShearId(shearid);
							//sss->appendShearMax(shearid, vals[0]);
							//sss->appendShearAvg(shearid, vals[1]);
							//sss->appendShearThickness(shearid, vals[2]);

							//已存储的单元从列表中移除
							eleIds.erase(shearid);
						}
						else
						{
							_readf.seekg(3 * sizeof(double), ios::cur);
						}

						//判断所需要的ID是否已全部读取，是则终止读取
						if (eleIds.size() == 0)
						{
							break;
						}
					}
				}
				else if (type.compare("ShearLoad", Qt::CaseInsensitive) == 0)
				{
					//存储数据
					//mSrcShearF* ssf = new mSrcShearF;
					//mSourceData::getInstance()->appendShearForce(cname, ssf);

					//读取数据
					const int nshear = framedata->getNumByElementType(SPQ4);
					for (int i = 0; i < nshear; ++i)
					{
						//单元ID
						int shearid;
						_readf.read((char*)&shearid, sizeof(int));

						//判断是否为所需单元
						if (eleIds.count(shearid) == 1)
						{
							//值
							double vals[4];
							_readf.read((char*)vals, 4 * sizeof(double));

							//ssf->appendShearId(shearid);
							//ssf->appendShearValues(shearid, vals[0], vals[1], vals[2], vals[3]);

							//已存储的单元从列表中移除
							eleIds.erase(shearid);
						}
						else
						{
							_readf.seekg(4 * sizeof(double), ios::cur);
						}

						//判断所需要的ID是否已全部读取，是则终止读取
						if (eleIds.size() == 0)
						{
							break;
						}
					}
				}
				else if (type.compare("ShellStrain", Qt::CaseInsensitive) == 0)
				{
					//存储数据
					//mSrcShellSE* ssse = new mSrcShellSE;
					//mSourceData::getInstance()->appendShellStrain(cname, ssse);

					//读取数据
					const int nshell = framedata->getNumByElementType(CPDSG) + framedata->getNumByElementType(MITC4) +
						framedata->getNumByElementType(TRIA3) + framedata->getNumByElementType(QPH);
					for (int i = 0; i < nshell; ++i)
					{
						//单元ID
						int shellid;
						_readf.read((char*)&shellid, sizeof(int));

						//读取层数
						int nlayer;
						_readf.read((char*)&nlayer, sizeof(int));

						//判断是否为所需单元
						if (eleIds.count(shellid) == 1)
						{
							//值
							for (int j = 0; j < nlayer; ++j)
							{
								double vals[9];
								_readf.read((char*)vals, 9 * sizeof(double));

								QString layername;
								if (j == 0)
								{
									layername = QString("At Center");
								}
								else
								{
									layername = QString("Layer %1").arg(j);
								}
								//ssse->appendShellId(shellid);
								//ssse->appendShellPly(shellid, layername, vals);
							}

							//已存储的单元从列表中移除
							eleIds.erase(shellid);
						}
						else
						{
							_readf.seekg(nlayer * 9 * sizeof(double), ios::cur);
						}

						//判断所需要的ID是否已全部读取，是则终止读取
						if (eleIds.size() == 0)
						{
							break;
						}
					}
				}
				else if (type.compare("ShellStress", Qt::CaseInsensitive) == 0)
				{
					//存储数据
					//mSrcShellSE* ssse = new mSrcShellSE;
					//mSourceData::getInstance()->appendShellStress(cname, ssse);

					//读取数据
					const int nshell = framedata->getNumByElementType(CPDSG) + framedata->getNumByElementType(MITC4) +
						framedata->getNumByElementType(TRIA3) + framedata->getNumByElementType(QPH);
					for (int i = 0; i < nshell; ++i)
					{
						//单元ID
						int shellid;
						_readf.read((char*)&shellid, sizeof(int));

						//读取层数
						int nlayer;
						_readf.read((char*)&nlayer, sizeof(int));

						//判断是否为所需单元
						if (eleIds.count(shellid) == 1)
						{
							//ssse->appendShellNPly(shellid, nlayer);
							//值
							for (int j = 0; j < nlayer; ++j)
							{
								double vals[9];
								_readf.read((char*)vals, 9 * sizeof(double));

								QString layername;
								if (j == 0)
								{
									layername = QString("At Z1");
								}
								else if (j == 1)
								{
									layername = QString("At Center");
								}
								else if (j == 2)
								{
									layername = QString("At Z2");
								}
								else
								{
									layername = QString("Layer %1").arg(j - 2);
								}
								//ssse->appendShellId(shellid);
								//ssse->appendShellPly(shellid, layername, vals);
							}

							//已存储的单元从列表中移除
							eleIds.erase(shellid);
						}
						else
						{
							_readf.seekg(nlayer * 9 * sizeof(double), ios::cur);
						}

						//判断所需要的ID是否已全部读取，是则终止读取
						if (eleIds.size() == 0)
						{
							break;
						}
					}
				}
				else if (type.compare("ShellForceResultant", Qt::CaseInsensitive) == 0)
				{
					//存储数据
					//mSrcShellRF* ssrf = new mSrcShellRF;
					//mSourceData::getInstance()->appendShellRForce(cname, ssrf);

					//读取数据
					const int nshell = framedata->getNumByElementType(CPDSG) + framedata->getNumByElementType(MITC4) +
						framedata->getNumByElementType(TRIA3) + framedata->getNumByElementType(QPH);
					for (int i = 0; i < nshell; ++i)
					{
						//单元ID
						int shellid;
						_readf.read((char*)&shellid, sizeof(int));

						//判断是否为所需单元
						if (eleIds.count(shellid) == 1)
						{
							//值
							double* vals = new double[18];
							_readf.read((char*)vals, 18 * sizeof(double));

							//ssrf->appendShellId(shellid);
							//ssrf->appendShellValues(shellid, vals);

							//已存储的单元从列表中移除
							eleIds.erase(shellid);
						}
						else
						{
							_readf.seekg(18 * sizeof(double), ios::cur);
						}

						//判断所需要的ID是否已全部读取，是则终止读取
						if (eleIds.size() == 0)
						{
							break;
						}
					}
				}
				else if (type.compare("ShellForceMoment", Qt::CaseInsensitive) == 0)
				{
					//存储数据
					//mSrcShellMF* ssmf = new mSrcShellMF;
					//mSourceData::getInstance()->appendShellMForce(cname, ssmf);

					//读取数据
					const int nshell = framedata->getNumByElementType(CPDSG) + framedata->getNumByElementType(MITC4) +
						framedata->getNumByElementType(TRIA3) + framedata->getNumByElementType(QPH);
					for (int i = 0; i < nshell; ++i)
					{
						//单元ID
						int shellid;
						_readf.read((char*)&shellid, sizeof(int));

						//判断是否为所需单元
						if (eleIds.count(shellid) == 1)
						{
							//值
							double* vals = new double[18];
							_readf.read((char*)vals, 18 * sizeof(double));

							//ssmf->appendShellId(shellid);
							//ssmf->appendShellValues(shellid, vals);

							//已存储的单元从列表中移除
							eleIds.erase(shellid);
						}
						else
						{
							_readf.seekg(18 * sizeof(double), ios::cur);
						}

						//判断所需要的ID是否已全部读取，是则终止读取
						if (eleIds.size() == 0)
						{
							break;
						}
					}
				}
			}
			_readf.close();
		}
	}

	void mIMxdbFile1::readAllResults(const QString & pname, QList<QStringList>& vals)
	{
		//mDataPost1* dp = mDatabaseSingleton::getInstance()->getDataPost(pname);
		if (_dp == nullptr)
		{
			return;
		}
		QList<FrameInfo> frameInfo = _dp->getFrameInfos();

		QHash<QString, QStringList> maxinfo;	//key-变量名-分量，value-最大值、最大值帧、最大值位置
		QHash<QString, QStringList> mininfo;	//key-变量名-分量，value-最大值、最大值帧、最大值位置
		for (FrameInfo finfo : frameInfo)
		{
			QList<VariableInfo> varInfo = finfo._frameVars;
			for (VariableInfo vinfo : varInfo)
			{
				//打开文件
				bool isopen = openFileStatus(finfo._frameFile);
				if (!isopen)
				{
					return;
				}

				//获取变量的字节位置
				long long int varbyte = vinfo._varByte;
				_readf.seekg(varbyte, ios::beg);

				//获取分量数
				const int ncompo = vinfo._nCompo;

				QStringList groupnames;
				if (vinfo._compoNames.size() > 0)
				{
					for (QString coname : vinfo._compoNames)
					{
						QString name = vinfo._varName + "-" + coname;
						groupnames.append(name);
					}
					if (vinfo._compoNames.size() == 3 && vinfo._isMag)
					{
						groupnames.append(vinfo._varName + "-Mag");
					}
				}
				else
				{
					groupnames.append(vinfo._varName);
				}

				//获取当前变量中的变量个数
				const int nresult = vinfo._varCount;
				for (int k = 0; k < nresult; ++k)
				{
					//获取层数及层索引
					const int nlayer = vinfo._varLayer.at(k);
					//计算分量值个数及所需值的索引
					int nval;
					if (nlayer == 0)
					{
						if (ncompo == 0)
						{
							nval = 1;
						}
						else
						{
							nval = ncompo;
						}
					}
					else
					{
						if (ncompo == 0)
						{
							nval = nlayer;
						}
						else
						{
							nval = ncompo * nlayer;
						}
					}

					//读取个数
					int loopcount;
					_readf.read((char*)&loopcount, sizeof(int));

					//读取变量值
					for (int j = 0; j < loopcount; ++j)
					{
						//ID
						int id;
						_readf.read((char*)&id, sizeof(int));

						//值（实部）
						double* val = new double[nval];
						_readf.read((char*)val, nval * sizeof(double));

						//值（虚部）
						double* val1 = new double[nval];
						_readf.read((char*)val1, nval * sizeof(double));

						//取最值
						for (int jj = 0; jj < nval; ++jj)
						{
							if (j == 0)
							{
								maxinfo.insert(groupnames.at(jj), { QString::number(val[jj]),QString::number(finfo._frameNum),QString::number(id) });
								mininfo.insert(groupnames.at(jj), { QString::number(val[jj]),QString::number(finfo._frameNum),QString::number(id) });
							}
							else
							{
								double tempval1 = maxinfo.value(groupnames.at(jj)).at(0).toDouble();
								if (val[jj] > tempval1)
								{
									maxinfo.insert(groupnames.at(jj), { QString::number(val[jj]),QString::number(finfo._frameNum),QString::number(id) });
								}
								double tempval2 = mininfo.value(groupnames.at(jj)).at(0).toDouble();
								if (val[jj] < tempval2)
								{
									mininfo.insert(groupnames.at(jj), { QString::number(val[jj]),QString::number(finfo._frameNum),QString::number(id) });
								}
							}
						}

						//计算合量
						if (nval == 3 && vinfo._isMag)
						{
							double magval = sqrt(val[0] * val[0] + val[1] * val[1] + val[2] * val[2] +
								val1[0] * val1[0] + val1[1] * val1[1] + val1[2] * val1[2]);
							if (j == 0)
							{
								maxinfo.insert(groupnames.at(3), { QString::number(magval),QString::number(finfo._frameNum),QString::number(id) });
								mininfo.insert(groupnames.at(3), { QString::number(magval),QString::number(finfo._frameNum),QString::number(id) });
							}
							else
							{
								double tempval1 = maxinfo.value(groupnames.at(3)).at(0).toDouble();
								if (magval > tempval1)
								{
									maxinfo.insert(groupnames.at(3), { QString::number(magval),QString::number(finfo._frameNum),QString::number(id) });
								}
								double tempval2 = mininfo.value(groupnames.at(3)).at(0).toDouble();
								if (magval < tempval2)
								{
									mininfo.insert(groupnames.at(3), { QString::number(magval),QString::number(finfo._frameNum),QString::number(id) });
								}
							}
						}

						//删除
						delete val;
						delete val1;
					}
				}

				_readf.close();
			}
		}

		//解析
		for (FrameInfo finfo : frameInfo)
		{
			QList<VariableInfo> varInfo = finfo._frameVars;
			for (VariableInfo vinfo : varInfo)
			{
				if (vinfo._compoNames.size() > 0)
				{
					for (QString cname : vinfo._compoNames)
					{
						QString gname = vinfo._varName + "-" + cname;
						QStringList templist = { gname };
						templist.append(maxinfo.value(gname));
						templist.append(mininfo.value(gname));
						if (!vals.contains(templist))
						{
							vals.append(templist);
						}
					}
					if (vinfo._compoNames.size() == 3 && vinfo._isMag)
					{
						QString gname = vinfo._varName + "-Mag";
						QStringList templist = { gname };
						templist.append(maxinfo.value(gname));
						templist.append(mininfo.value(gname));
						if (!vals.contains(templist))
						{
							vals.append(templist);
						}
					}
				}
				else
				{
					QStringList templist = { vinfo._varName };
					templist.append(maxinfo.value(vinfo._varName));
					templist.append(mininfo.value(vinfo._varName));
					if (!vals.contains(templist))
					{
						vals.append(templist);
					}
				}
			}
		}
	}

	void mIMxdbFile1::readScalarData(const QString & pname, int f, const QString & var, QHash<int, double>& vals, MViewBasic::NodeOrElement &nodeOrElement)
	{
		if (var == "")
		{
			return;
		}
		QString compo, layer, complex;

		//mDataPost1* dp = mDatabaseSingleton::getInstance()->getDataPost(pname);
		if (_dp == nullptr)
		{
			return;
		}
		QList<FrameInfo> frameInfo = _dp->getFrameInfos();
		if (f < 1 || f > frameInfo.size())
		{
			return;
		}

		FrameInfo frameinfo = frameInfo.at(f - 1);
		const int nvar = frameinfo._frameVars.size();
		for (int i = 0; i < nvar; ++i)
		{
			VariableInfo varinfo = frameinfo._frameVars.at(i);
			if (varinfo._varName == var)
			{
				//打开文件
				bool isopen = openFileStatus(frameinfo._frameFile);
				if (!isopen)
				{
					return;
				}

				//获取变量的字节位置
				long long int varbyte = varinfo._varByte;
				_readf.seekg(varbyte, ios::beg);

				//获取分量数
				const int ncompo = varinfo._nCompo;

				//获取当前分量的索引
				const int compoi = varinfo._compoNames.indexOf(compo);

				//存储值
				double maxval = 0, minval = 0;
				QHash<int, float> compovalues;

				//获取当前变量中的变量个数
				const int nresult = varinfo._varCount;
				for (int k = 0; k < nresult; ++k)
				{
					//获取层数及层索引
					const int nlayer = varinfo._varLayer.at(k);
					const int layerindex = varinfo._varLayerNames.at(k).indexOf(layer);
					//计算分量值个数及所需值的索引
					int nval, valindex;
					if (nlayer == 0)
					{
						if (ncompo == 0)
						{
							nval = 1;
							valindex = 0;
						}
						else
						{
							nval = ncompo;
							valindex = compoi;
						}
					}
					else
					{
						if (ncompo == 0)
						{
							nval = nlayer;
							valindex = layerindex + compoi;
						}
						else
						{
							nval = ncompo * nlayer;
							valindex = ncompo * layerindex + compoi;
						}
					}

					//读取个数
					int loopcount;
					_readf.read((char*)&loopcount, sizeof(int));

					//读取变量值
					for (int j = 0; j < loopcount; ++j)
					{
						//ID
						int id;
						_readf.read((char*)&id, sizeof(int));

						//值（实部）
						double* val = new double[nval];
						_readf.read((char*)val, nval * sizeof(double));

						//值（虚部）
						double* val1 = new double[nval];
						_readf.read((char*)val1, nval * sizeof(double));

						double tempval;
						if (complex == QString("实部"))
						{
							tempval = val[valindex];
						}
						else if (complex == QString("虚部"))
						{
							tempval = val1[valindex];
						}
						else if (complex == QString("幅值"))
						{
							double v0 = val[valindex];
							double v1 = val1[valindex];
							tempval = sqrt(v0*v0 + v1 * v1);
						}
						else if (complex == QString("相位"))
						{
							double v0 = val[valindex];
							double v1 = val1[valindex];
							tempval = atan(v0 / v1) * 180 / 3.1415926 + 180;
						}
						else
						{
							if (valindex == -1 && nval == 3)
							{
								tempval = sqrt(val[0] * val[0] + val[1] * val[1] + val[2] * val[2] +
									val1[0] * val1[0] + val1[1] * val1[1] + val1[2] * val1[2]);
							}
							else
							{
								tempval = val[valindex];
							}
						}
						vals.insert(id, tempval);

						//取最值
						if (j == 0)
						{
							maxval = tempval;
							minval = tempval;
						}
						else
						{
							maxval = (tempval > maxval) ? tempval : maxval;
							minval = (tempval < minval) ? tempval : minval;
						}

						delete val;
						delete val1;
					}
				}

				compovalues.clear();

				_readf.close();

				//获取变量类型
				nodeOrElement = MViewBasic::NodeOrElement(varinfo._varType);

				//输出信息
				//mOutputMessage tepmsg(NormalMessage, QString("第%1帧文件数据读取成功！").arg(f));
				//mGlobalSignals::getInstance()->outputMessageSig(tepmsg);

				QList<FrameInfo>().swap(frameInfo);
				frameInfo.clear();
				return;
			}
		}
	}

	void mIMxdbFile1::readVectorData(const QString & pname, int f, const QString & var, QHash<int, QVector3D> &vals, MViewBasic::NodeOrElement &nodeOrElement)
	{
		if (var == "")
		{
			return;
		}
		//mDataPost1* dp = mDatabaseSingleton::getInstance()->getDataPost(pname);

		if (_dp == nullptr)
		{
			return;
		}
		QList<FrameInfo> frameInfo = _dp->getFrameInfos();
		if (f < 1 || f > frameInfo.size())
		{
			return;
		}
		FrameInfo frameinfo = frameInfo.at(f - 1);
		const int nvar = frameinfo._frameVars.size();
		for (int i = 0; i < nvar; ++i)
		{
			VariableInfo varinfo = frameinfo._frameVars.at(i);
			if (varinfo._varName == var)
			{
				//获取分量数
				const int ncompo = varinfo._nCompo;
				if (ncompo != 3)
				{
					return;
				}

				//打开文件
				bool isopen = openFileStatus(frameinfo._frameFile);
				if (!isopen)
				{
					return;
				}

				//获取变量的字节位置
				long long int varbyte = varinfo._varByte;
				_readf.seekg(varbyte, ios::beg);

				//读取节点/单元数
				int nodecount;
				_readf.read((char*)&nodecount, sizeof(int));

				//QHash<int, QVector3D> vecvals;

				vals.reserve(nodecount);

				for (int j = 0; j < nodecount; ++j)
				{
					//节点ID
					int nid;
					_readf.read((char*)&nid, sizeof(int));

					//矢量值（实部）
					double val[3];
					_readf.read((char*)val, 3 * sizeof(double));

					//矢量值（虚部）
					double val1[3];
					_readf.read((char*)val1, 3 * sizeof(double));

					vals.insert(nid, QVector3D(val[0], val[1], val[2]));
				}

				//获取变量类型
				nodeOrElement = MViewBasic::NodeOrElement(varinfo._varType);

				//输出信息
				//mOutputMessage tepmsg(NormalMessage, QString("第%1帧文件矢量数据读取成功！").arg(f));
				//mGlobalSignals::getInstance()->outputMessageSig(tepmsg);

				_readf.close();
				return;

			}
		}
	}

	void mIMxdbFile1::run()
	{
		//bool isread = this->readCloudData(_pname, _frame, _var, _compo, _layer, _complex);
		//if (isread)
		//{
		//	//读取位移
		//	this->readDisplacementData(_pname, _frame, _complex);

		//	//判断是否读取流线图矢量
		//	if (_hasstreamLine)
		//	{
		//		this->readVectorData(_pname, _frame, _streamLineVectorName);
		//	}
		//	else
		//	{
		//		//判断是否读取矢量图矢量
		//		if (_hasvectorArrowVector)
		//		{
		//			this->readVectorData(_pname, _frame, _vectorArrowVectorName);
		//		}
		//	}
		//}
		////postmutex.lock();
		//QMutexLocker locker(&postmutex);
		//_animationData->appendRendOneFrameData(_frame, _oneFrameRender);
		//if (_animationData->getRendAnimationIds().size() == _threadNum)
		//{
		//	emit finishedImportAllMxDbFileSig();
		//}
		//postmutex.unlock();
	}

	bool mIMxdbFile1::openFileStatus(const QString & fname)
	{
		//_byteAmount = 0;

		//判断文件是否存在
		QFile dbFile(fname);
		if (!dbFile.exists())
		{
			//emit readFileStatusSig(FileNotExist);
			//mGlobalSignals::getInstance()->outputMessageSig(1, QString("文件%1不存在！").arg(fname));
			return false;
		}
		//判断文件是否打开
		_readf.open(fname.toLocal8Bit(), ios::in | ios::binary);
		if (!_readf)
		{
			//emit readFileStatusSig(FileNotOpen);
			//mGlobalSignals::getInstance()->outputMessageSig(2, QString("文件%1打开失败！").arg(fname));
			return false;
		}

		//读取版本号
		int v = 0;
		_readf.read((char*)&v, sizeof(int));
		//_byteAmount += sizeof(int);
		_version = (unsigned int)v;
		if (_version > 0xA1B1C104)			//-1582186239 == 0xA1B1C102、-1582186238 == 0xA1B1C103
		{
			//emit readFileStatusSig(FileVersionFalse);
			//mGlobalSignals::getInstance()->outputMessageSig(2, QString("文件%1版本错误！").arg(fname));
			return false;
		}

		return true;
	}

	void mIMxdbFile1::readInitModelDatas(shared_ptr<mDataPost1> dataPost, mOneFrameData1 * fd)
	{
		/*
		//读取节点总数
		int nodeamount;
		_readf.read((char*)&nodeamount, sizeof(int));
		//_byteAmount += sizeof(int);
		fd->resizeNodeSize(nodeamount);

		//存储节点总数
		for (int i = 0; i < nodeamount; ++i)
		{
			//读取节点ID
			int nodeid;
			_readf.read((char*)&nodeid, sizeof(int));
			//_byteAmount += sizeof(int);

			//读取节点坐标
			double nodecoord[3];
			_readf.read((char*)nodecoord, 3 * sizeof(double));
			//_byteAmount += 3* sizeof(double);

			//存储节点ID和坐标
			mPostMeshNodeData1 *nodeData = new mPostMeshNodeData1(nodeid, QVector3D(nodecoord[0], nodecoord[1], nodecoord[2]));
			fd->appendNodeData(nodeid, nodeData);
		}

		//输出信息
		mGlobalSignals::getInstance()->outputMessageSig(0, QString("模型节点数据读取成功..."));

		//读取Part数
		int partcount;
		_readf.read((char*)&partcount, sizeof(int));

		//读取part信息
		for (int i = 0; i < partcount; ++i)
		{
			//读取Part名
			char partname[64];
			_readf.read(partname, 64 * sizeof(char));
			//_byteAmount += 64*sizeof(char);

			mPostMeshPartData1 *partData = new mPostMeshPartData1(QString(partname), i + 1);

			//读取单元数量
			int elecount;
			_readf.read((char*)&elecount, sizeof(int));
			//_byteAmount += sizeof(int);

			QHash<QVector<int>, mPostMeshFaceData1*> _meshFace;

			for (int j = 0; j < elecount; ++j)
			{
				//读取单元ID
				int eleid;
				_readf.read((char*)&eleid, sizeof(int));
				//_byteAmount += sizeof(int);

				//读取单元类型
				char eletype[32];
				_readf.read(eletype, 32 * sizeof(char));
				//_byteAmount += 32* sizeof(char);

				//存储
				ElementType elementtype = mCommonFunctions::getElementType(QString(eletype).toUpper());
				MeshType meshtype = mCommonFunctions::transformTypeStrToEnum(elementtype);
				//根据类型获取单元包含的节点个数
				int elenode = mCommonFunctions::elementNodeCount(elementtype);


				//读取单元包含的节点ID
				int *ids = new int[elenode];
				_readf.read((char*)ids, elenode * sizeof(int));
				//_byteAmount += elenode*sizeof(int);				

				QVector<int> nodeids;
				nodeids.reserve(elenode);
				for (int k = 0; k < elenode; ++k)
				{
					nodeids.append(ids[k]);
				}
				delete ids;
				postmutex.lock();

				fd->createMesh(eleid, meshtype, elementtype, nodeids, partData, _meshFace);
				postmutex.unlock();
			}

			fd->appendMeshPartData(partname, partData);
			//存储显示的面
			partData->setMeshFaceData(_meshFace.values().toVector());
			//输出信息
			mGlobalSignals::getInstance()->outputMessageSig(0, QString("%1部件数据读取成功！").arg(QString(partname)));
			_meshFace.clear();
			QHash<QVector<int>, mPostMeshFaceData1*>().swap(_meshFace);
		}
		//将单元数据分配到hash和vector
		fd->calculateCellData();
		fd->calculatePointCell();
		//生成边界线
		fd->caculateMeshLine();

		//输出信息
		mGlobalSignals::getInstance()->outputMessageSig(0, QString("边界线生成成功！"));

		*/

		this->readModelDatas(fd);

		//读取特殊零件（刚性墙）数据
		if (_version >= 0xA1B1C104)
		{
			//读取个数
			int rwcount;
			_readf.read((char*)&rwcount, sizeof(int));

			for (int i = 0; i < rwcount; ++i)
			{
				//读取ID
				int ID;
				_readf.read((char*)&ID, sizeof(int));

				//读取类型
				int rwtype;
				_readf.read((char*)&rwtype, sizeof(int));

				//读取法线起始、终止坐标
				double normaldirect[6];
				_readf.read((char*)normaldirect, 6 * sizeof(double));
				if (rwtype == 0)//读无限大平面刚性墙
				{
					mPostInfinitelyLargePlaneRigidWall *rigidWall = new mPostInfinitelyLargePlaneRigidWall(ID, normaldirect);
					dataPost->appendRigidWallData(rigidWall);
				}
				else if (rwtype == 1)					//读有限大平面刚性墙
				{
					//边向量终止坐标
					double edgecoord[3];
					_readf.read((char*)edgecoord, 3 * sizeof(double));

					//边长L
					double edgeL;
					_readf.read((char*)&edgeL, sizeof(double));

					//边长M
					double edgeM;
					_readf.read((char*)&edgeM, sizeof(double));

					mPostLimitedLargePlaneRigidWall *rigidWall = new mPostLimitedLargePlaneRigidWall(ID, normaldirect);
					memcpy(rigidWall->_lineEndVector, edgecoord, 3 * sizeof(double));
					rigidWall->_L = edgeL;
					rigidWall->_M = edgeM;
					dataPost->appendRigidWallData(rigidWall);
				}
				else if (rwtype == 2)				//读长方体刚性墙
				{
					//边向量终止坐标
					double edgecoord[3];
					_readf.read((char*)edgecoord, 3 * sizeof(double));

					//边长L
					double edgeL;
					_readf.read((char*)&edgeL, sizeof(double));

					//边长M
					double edgeM;
					_readf.read((char*)&edgeM, sizeof(double));

					//边长P
					double edgeP;
					_readf.read((char*)&edgeP, sizeof(double));

					mPostRectangularRigidWall *rigidWall = new mPostRectangularRigidWall(ID, normaldirect);
					memcpy(rigidWall->_lineEndVector, edgecoord, 3 * sizeof(double));
					rigidWall->_L = edgeL;
					rigidWall->_M = edgeM;
					rigidWall->_P = edgeP;
					dataPost->appendRigidWallData(rigidWall);
				}
				else if (rwtype == 3)					//读圆柱状刚性墙
				{
					//半径
					double radius;
					_readf.read((char*)&radius, sizeof(double));

					//轴向长度
					double length;
					_readf.read((char*)&length, sizeof(double));

					mPostCylindricalRigidWall *rigidWall = new mPostCylindricalRigidWall(ID, normaldirect);
					rigidWall->_radius = radius;
					rigidWall->_length = length;
					dataPost->appendRigidWallData(rigidWall);
				}
				else if (rwtype == 4)//读球刚性墙
				{
					//半径
					double radius;
					_readf.read((char*)&radius, sizeof(double));

					mPostSphericalRigidWall *rigidWall = new mPostSphericalRigidWall(ID, normaldirect);
					rigidWall->_radius = radius;
					dataPost->appendRigidWallData(rigidWall);
				}
			}
		}
	}

	void mIMxdbFile1::readModelDatas(mOneFrameData1* fd)
	{
		//读取节点总数
		int nodeamount;
		_readf.read((char*)&nodeamount, sizeof(int));
		//_byteAmount += sizeof(int);
		fd->resizeNodeSize(nodeamount);

		int maxNodeID = 0;
		//存储节点总数
		for (int i = 0; i < nodeamount; ++i)
		{
			//读取节点ID
			int nodeid;
			_readf.read((char*)&nodeid, sizeof(int));
			//_byteAmount += sizeof(int);

			//读取节点坐标
			double nodecoord[3];
			_readf.read((char*)nodecoord, 3 * sizeof(double));
			//_byteAmount += 3* sizeof(double);

			maxNodeID = max(maxNodeID, nodeid);

			//存储节点ID和坐标
			mPostMeshNodeData1 *nodeData = new mPostMeshNodeData1(nodeid, QVector3D(nodecoord[0], nodecoord[1], nodecoord[2]));
			fd->appendNodeData(nodeid, nodeData);
		}

		qDebug() << "节点数量为" << nodeamount;

		//输出信息
		//mGlobalSignals::getInstance()->outputMessageSig(0, QString("模型节点数据读取成功..."));

		//读取Part数
		int partcount;
		_readf.read((char*)&partcount, sizeof(int));
		
		//读取part信息
		for (int i = 0; i < partcount; ++i)
		{
			//读取Part名
			char partname[64];
			_readf.read(partname, 64 * sizeof(char));
			//_byteAmount += 64*sizeof(char);

			mPostMeshPartData1 *partData = new mPostMeshPartData1(QString(partname), i + 1);

			//读取单元数量
			int elecount;
			_readf.read((char*)&elecount, sizeof(int));
			//_byteAmount += sizeof(int);

			qDebug() << "单元数量为" << elecount;
	
			//QHash<QVector<int>, mPostMeshFaceData1*> _meshFace;
			QVector<QVector<QPair<QPair<int, int>, mPostMeshFaceData1*>>> meshFaces(maxNodeID);
#if optimazation

			int minnodeidpart = maxNodeID;
			int maxnodeidpart = 0;
#endif // optimazation
			for (int j = 0; j < elecount; ++j)
			{
				//读取单元ID
				int eleid;
				_readf.read((char*)&eleid, sizeof(int));
				//_byteAmount += sizeof(int);

				//读取单元类型
				char eletype[32];
				_readf.read(eletype, 32 * sizeof(char));
				//_byteAmount += 32* sizeof(char);

				//存储
				ElementType elementtype = mCommonFunctions::getElementType(QString(eletype).toUpper());
				MeshType meshtype = mCommonFunctions::transformTypeStrToEnum(elementtype);
				//根据类型获取单元包含的节点个数
				int elenode = mCommonFunctions::elementNodeCount(elementtype);


				//读取单元包含的节点ID
				int *ids = new int[elenode];
				_readf.read((char*)ids, elenode * sizeof(int));
				//_byteAmount += elenode*sizeof(int);				
				
				QVector<int> nodeids;
				nodeids.reserve(elenode);
				for (int k = 0; k < elenode; ++k)
				{
					nodeids.append(ids[k]);
#if optimazation
					minnodeidpart = min(minnodeidpart, ids[k]);
					maxnodeidpart = max(maxnodeidpart, ids[k]);
#endif // optimazation
				}
				delete ids;
				postmutex.lock();
		
				fd->createMesh(eleid, meshtype, elementtype, nodeids, partData, meshFaces);
				postmutex.unlock();
			}

			fd->appendMeshPartData(partname, partData);
			//存储显示的面
			QVector<mPostMeshFaceData1*> faceData;
#if optimazation
			for (int j = minnodeidpart;j <= maxnodeidpart;++j)
			{
				for (auto iter : meshFaces.at(j))
				{
					faceData.append(iter.second);
				}
			}
#else
			for (auto face : meshFaces)
			{
				for (auto iter : face)
				{
					faceData.append(iter.second);
				}
			}
#endif // optimazation
			partData->setMeshFaceData(faceData);
			//partData->setMeshFaceData(_meshFace.values().toVector());
			//输出信息
			//mGlobalSignals::getInstance()->outputMessageSig(0, QString("%1部件数据读取成功！").arg(QString(partname)));
			meshFaces.clear();
			QVector<QVector<QPair<QPair<int, int>, mPostMeshFaceData1*>>>().swap(meshFaces);
			//QHash<QVector<int>, mPostMeshFaceData1*>().swap(_meshFace);
		}

		//将单元数据分配到hash和vector
		fd->calculateCellData();
		//fd->calculatePointCell();
		//mGlobalSignals::getInstance()->outputMessageSig(0, QString("整理数据！"));
		//生成边界线
		fd->caculateMeshLine();

		//输出信息
		//mGlobalSignals::getInstance()->outputMessageSig(0, QString("边界线生成成功！"));

		//QtConcurrent::run(fd, &mOneFrameData1::calculatePointCell);
	}
}