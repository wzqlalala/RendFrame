#include "mDataPost1.h"
#include "mOneFrameData1.h"
#include "mPostSetBase.h"
#include "mPostRigidWallData1.h"


namespace MDataPost
{

	mDataPost1::mDataPost1()
	{
		_modelName = QString("Post");
	}

	mDataPost1::~mDataPost1()
	{
		deleteAllFrameData();
		deleteSetData();
	}

	void mDataPost1::setModelName(const QString& name)
	{
		_modelName = name;
	}

	QString mDataPost1::getModelName()
	{
		return _modelName;
	}

	void mDataPost1::setResultType(int type)
	{
		_resultType = type;
	}

	int mDataPost1::getResultType()
	{
		return _resultType;
	}

	void mDataPost1::deleteAllFrameData()
	{
		for (auto iter = _allFrameids.begin();iter!=_allFrameids.end();++iter)
		{
			if (*iter == 0)
			{
				mOneFrameData1 *oneFrameData = _frameid_frameData.value(*iter);
				if (oneFrameData != nullptr)
				{
					//oneFrameData->deleteOneFrameData();
					delete oneFrameData;
					oneFrameData = nullptr;
				}
			}
			if (_frameSameFrame.value(*iter) == -1)
			{
				mOneFrameData1 *oneFrameData = _frameid_frameData.value(*iter);
				if (oneFrameData != nullptr)
				{
					//oneFrameData->deleteOneFrameData();
					delete oneFrameData;
					oneFrameData = nullptr;
				}
			}
		}
		_allFrameids.clear();
		_frameid_frameData.clear();
		_frameid_Time.clear();
		_frameSameFrame.clear();
		_allPartNames.clear();

	}

	void mDataPost1::appendFrameData(int frameid, double time, mOneFrameData1* frameData)
	{
		_frameid_frameData.insert(frameid, frameData);
		_frameid_Time.insert(frameid, time);
		_allFrameids.insert(frameid);
	}

	void mDataPost1::appendSameFrame(int frame1, int frame2)
	{
		_frameSameFrame.insert(frame1, frame2);
	}

	mOneFrameData1* mDataPost1::getFirstFrameData()
	{
		if (_allFrameids.size()!=0)
		{
			int id = *(_allFrameids.begin());
			return _frameid_frameData.value(id);
		}
		return nullptr;
	}

	mOneFrameData1* mDataPost1::getLastFrameData()
	{
		if (_allFrameids.size() != 0)
		{
			int id = *(_allFrameids.rbegin());
			return _frameid_frameData.value(id);
		}
		return nullptr;
	}

	void mDataPost1::getAllFrameId(std::set<int>& frameids)
	{
		frameids = _allFrameids;
	}

	mOneFrameData1* mDataPost1::getOneFrameData(int frameid)
	{
		if (_frameid_frameData.contains(frameid))
		{
			return _frameid_frameData.value(frameid);
		}
		return nullptr;
	}

	double mDataPost1::getOneFrameTime(int frameid)
	{
		if (_frameid_Time.contains(frameid))
		{
			return _frameid_Time.value(frameid);
		}
		return 0.00;
	}

	int mDataPost1::getTotalFrameCount()
	{
		return _allFrameids.size();
	}

	void mDataPost1::setMeshPartColor(QString partName, QVector3D color)
	{
		for (auto iter = _allFrameids.begin(); iter != _allFrameids.end(); ++iter)
		{
			mOneFrameData1 *oneFrameData = _frameid_frameData.value(*iter);
			if (oneFrameData != nullptr)
			{
				oneFrameData->setMeshPartColor(partName, color);
			}
		}
	}

	QVector3D mDataPost1::getMeshPartColor(QString partName)
	{
		for (auto iter = _allFrameids.begin(); iter != _allFrameids.end(); ++iter)
		{
			mOneFrameData1 *oneFrameData = _frameid_frameData.value(*iter);
			if (oneFrameData != nullptr)
			{
				if (oneFrameData->isExistMeshPart(partName))
				{
					return oneFrameData->getMeshPartColor(partName) *255;
				}
			}
		}
		return QVector3D();

	}

	void mDataPost1::updateAllPostPartNames()
	{
		for (auto iter = _allFrameids.begin(); iter != _allFrameids.end(); ++iter)
		{
			mOneFrameData1 *oneFrameData = _frameid_frameData.value(*iter);
			if (oneFrameData != nullptr)
			{
				QStringList partNames = oneFrameData->getOneFramePartNames();
				for (QString partName: partNames)
				{
					_allPartNames.insert(partName);
				}
			}
		}
	}

	std::set<QString> mDataPost1::getAllPostPartNames()
	{
		return _allPartNames;
	}

	void mDataPost1::appendFrameInfo(FrameInfo fi)
	{
		_frameInfo.append(fi);
	}

	FrameInfo mDataPost1::getFrameInfo(int f)
	{
		for (FrameInfo fi : _frameInfo)
		{
			if (fi._frameNum == f)
			{
				return fi;
			}
		}
		return FrameInfo();
	}

	FrameInfo mDataPost1::getFrameInfo(const QString & cname)
	{
		for (FrameInfo fi : _frameInfo)
		{
			if (fi._frameCase == cname)
			{
				return fi;
			}
		}
		return FrameInfo();
	}

	QList<FrameInfo> mDataPost1::getFrameInfos(const QStringList & cnames)
	{
		QList<FrameInfo> infos;
		for (FrameInfo fi : _frameInfo)
		{
			if (cnames.contains(fi._frameCase))
			{
				infos.append(fi);
			}
		}
		return infos;
	}

	QList<FrameInfo> mDataPost1::getFrameInfos()
	{
		return _frameInfo;
	}

	int mDataPost1::getFrameAmount()
	{
		return _frameInfo.size();
	}

	QStringList mDataPost1::getAllCaseNames()
	{
		QStringList cnames;
		const int nframe = _frameInfo.size();
		for (int i = 0; i < nframe; ++i)
		{
			FrameInfo finfo = _frameInfo.at(i);
			if (!cnames.contains(finfo._frameCase))
			{
				cnames.append(finfo._frameCase);
			}
		}
		return cnames;
	}

	QStringList mDataPost1::getCaseFrameNames(const QString & cname)
	{
		QStringList fnames;
		const int nframe = _frameInfo.size();
		for (int i = 0; i < nframe; ++i)
		{
			FrameInfo finfo = _frameInfo.at(i);
			if (cname == QString("All"))
			{
				fnames.append(finfo._frameName);
			}
			else if (cname == finfo._frameCase)
			{
				fnames.append(finfo._frameName);
			}
		}
		return fnames;
	}

	QStringList mDataPost1::getFrameVariables(int f)
	{
		QStringList vnames;
		int fi = f - 1;
		if (fi > -1 && fi < _frameInfo.size())
		{
			FrameInfo finfo = _frameInfo.at(fi);
			const int nvar = finfo._frameVars.size();
			for (int i = 0; i < nvar; ++i)
			{
				VariableInfo vinfo = finfo._frameVars.at(i);
				vnames.append(vinfo._varName);
			}
		}
		return vnames;
	}

	QStringList mDataPost1::getFrameVectors(int f)
	{
		QStringList vnames;
		int fi = f - 1;
		if (fi > -1 && fi < _frameInfo.size())
		{
			FrameInfo finfo = _frameInfo.at(fi);
			const int nvar = finfo._frameVars.size();
			for (int i = 0; i < nvar; ++i)
			{
				VariableInfo vinfo = finfo._frameVars.at(i);
				if (vinfo._nCompo == 3)
				{
					vnames.append(vinfo._varName);
				}
			}
		}
		return vnames;
	}

	QStringList mDataPost1::getAllVariableNames()
	{
		QStringList vnames;
		const int nf = _frameInfo.size();
		for (int i = 0; i < nf; ++i)
		{
			FrameInfo finfo = _frameInfo.at(i);
			const int nvar = finfo._frameVars.size();
			for (int i = 0; i < nvar; ++i)
			{
				VariableInfo vinfo = finfo._frameVars.at(i);
				vnames.append(vinfo._varName);
			}
		}
		return vnames.toSet().toList();
	}

	QStringList mDataPost1::getVariableCompos(const QString & var)
	{
		QStringList cnames;
		const int nf = _frameInfo.size();
		for (int i = 0; i < nf; ++i)
		{
			FrameInfo finfo = _frameInfo.at(i);
			const int nvar = finfo._frameVars.size();
			for (int i = 0; i < nvar; ++i)
			{
				VariableInfo vinfo = finfo._frameVars.at(i);
				if (var == vinfo._varName)
				{
					cnames = vinfo._compoNames;

					//判断是否计算矢量模
					if (vinfo._isMag)
					{
						cnames.append(QString("Mag"));
					}

					return cnames;
				}
			}
		}
		return cnames;
	}

	bool mDataPost1::isVarCompoComplex(const QString & var, const QString & compo)
	{
		const int nf = _frameInfo.size();
		for (int i = 0; i < nf; ++i)
		{
			FrameInfo finfo = _frameInfo.at(i);
			const int nvar = finfo._frameVars.size();
			for (int i = 0; i < nvar; ++i)
			{
				VariableInfo vinfo = finfo._frameVars.at(i);
				if (var == vinfo._varName)
				{
					return vinfo._compoComplex.value(compo);
				}
			}
		}
		return false;
	}

	QStringList mDataPost1::getVariableLayer(const QString & var)
	{
		QStringList lnames;
		const int nf = _frameInfo.size();
		for (int i = 0; i < nf; ++i)
		{
			FrameInfo finfo = _frameInfo.at(i);
			const int nvar = finfo._frameVars.size();
			for (int i = 0; i < nvar; ++i)
			{
				VariableInfo vinfo = finfo._frameVars.at(i);
				if (var == vinfo._varName)
				{
					const int nlayer = vinfo._varLayerNames.size();
					for (int j = 0; j < nlayer; ++j)
					{
						lnames.append(vinfo._varLayerNames.at(j));
					}
					//去除重复项
					lnames = lnames.toSet().toList();
					return lnames;
				}
			}
		}
		return lnames;
	}

	QStringList mDataPost1::getFrameParts(int f)
	{
		QStringList pnames;

		mOneFrameData1 *oneFrameData = this->getOneFrameData(f);
		if (oneFrameData != nullptr)
		{
			pnames = oneFrameData->getOneFramePartNames();
		}

		return pnames;
	}

	double mDataPost1::getFrameTime(int f)
	{
		double t = 0.0;
		int fi = f - 1;
		if (fi > -1 && fi < _frameInfo.size())
		{
			FrameInfo finfo = _frameInfo.at(fi);
			t = finfo._frameTime;
		}
		return t;
	}

	QString mDataPost1::getFrameCase(int f)
	{
		QString name;
		int fi = f - 1;
		if (fi > -1 && fi < _frameInfo.size())
		{
			FrameInfo finfo = _frameInfo.at(fi);
			name = finfo._frameCase;
		}
		return name;
	}

	bool mDataPost1::getFrameFreqValue(int f, double * val)
	{
		bool hasval = false;
		int fi = f - 1;
		if (fi > -1 && fi < _frameInfo.size())
		{
			FrameInfo finfo = _frameInfo.at(fi);
			hasval = finfo._isFrequency;
			if (hasval)
			{
				SpecialFrequency sf = finfo._frequency;
				val[0] = sf._modalFreq;
				val[1] = sf._modalEigen;
			}
		}
		return hasval;
	}

	bool mDataPost1::getFrameComplexMode(int f, double * val)
	{
		bool hasval = false;
		int fi = f - 1;
		if (fi > -1 && fi < _frameInfo.size())
		{
			FrameInfo finfo = _frameInfo.at(fi);
			hasval = finfo._isComplexMode;
			if (hasval)
			{
				ComplexModes sf = finfo._compMode;
				val[0] = sf._realVal;
				val[1] = sf._virtualVal;
				val[2] = sf._freqVal;
				val[3] = sf._dampVal;
			}
		}
		return hasval;
	}

	int mDataPost1::getVariableType(const QString & var)
	{
		int type = 0;
		const int nf = _frameInfo.size();
		for (int i = 0; i < nf; ++i)
		{
			FrameInfo finfo = _frameInfo.at(i);
			const int nvar = finfo._frameVars.size();
			for (int i = 0; i < nvar; ++i)
			{
				VariableInfo vinfo = finfo._frameVars.at(i);
				if (var == vinfo._varName)
				{
					type = vinfo._varType;
					return type;
				}
			}
		}
		return type;
	}

	void mDataPost1::appendShellLayer(const QString & name, int nlayer)
	{
		_shellLayers.insert(name, nlayer);
	}

	int mDataPost1::getShellLayerCount(const QString & name)
	{
		if (_shellLayers.contains(name))
		{
			return _shellLayers.value(name);
		}
		return 0;
	}

	void mDataPost1::setMeshPartVisual(QString partName, bool isShow)
	{
		for (auto iter = _allFrameids.begin(); iter != _allFrameids.end(); ++iter)
		{
			mOneFrameData1 *oneFrameData = _frameid_frameData.value(*iter);
			if (oneFrameData != nullptr)
			{
				oneFrameData->setMeshPartVisual(partName, isShow);
			}
		}
	}

	bool mDataPost1::getMeshPartVisual(QString partName)
	{
		for (auto iter = _allFrameids.begin(); iter != _allFrameids.end(); ++iter)
		{
			mOneFrameData1 *oneFrameData = _frameid_frameData.value(*iter);
			if (oneFrameData != nullptr)
			{
				if (oneFrameData->isExistMeshPart(partName))
				{
					return oneFrameData->getMeshPartVisual(partName);
				}
			}
		}
		return false;
	}

	void mDataPost1::setAllMeshPartVisual(bool isShow)
	{
		for (auto iter = _allFrameids.begin(); iter != _allFrameids.end(); ++iter)
		{
			mOneFrameData1 *oneFrameData = _frameid_frameData.value(*iter);
			if (oneFrameData != nullptr)
			{
				oneFrameData->setAllMeshPartVisual(isShow);
			}
		}
	}

	void mDataPost1::setAllMeshPartColor(QVector3D color)
	{
		for (auto iter = _allFrameids.begin(); iter != _allFrameids.end(); ++iter)
		{
			mOneFrameData1 *oneFrameData = _frameid_frameData.value(*iter);
			if (oneFrameData != nullptr)
			{
				oneFrameData->setAllMeshPartColor(color);
			}
		}
	}

	void mDataPost1::appendSetData(mPostSetBase* setData)
	{
		_setData.append(setData);
	}
	int mDataPost1::getSetDataCount()
	{
		return _setData.size();
	}
	mPostSetBase* mDataPost1::getSetData(int i)
	{
		if (i > -1 && i < _setData.size())
		{
			return _setData.at(i);
		}
		else
		{
			return nullptr;
		}
	}
	mPostSetBase* mDataPost1::getSetData(const QString& name)
	{
		const int scount = _setData.size();
		for (int i = 0; i < scount; ++i)
		{
			mPostSetBase* tepSet = _setData.at(i);
			if (tepSet != nullptr)
			{
				QString cName = tepSet->getSetName();
				if (cName == name)
				{
					return tepSet;
				}
			}
		}
		return nullptr;
	}
	QStringList mDataPost1::getSetNames()
	{
		QStringList names;
		const int scount = _setData.size();
		for (int i = 0; i < scount; ++i)
		{
			mPostSetBase* tepSet = _setData.at(i);
			if (tepSet != nullptr)
			{
				QString setName = tepSet->getSetName();
				names.append(setName);
			}
		}
		return names;
	}

	void mDataPost1::replaceSetData(mPostSetBase* sData)
	{
		if (sData == nullptr)
		{
			return;
		}
		QString cName = sData->getSetName();
		const int nprop = _setData.size();
		for (int i = 0; i < nprop; ++i)
		{
			mPostSetBase* tepSet = _setData.at(i);
			if (tepSet != nullptr)
			{
				QString name = tepSet->getSetName();
				if (name == cName)
				{
					_setData.removeAt(i);
					delete tepSet;
					_setData.insert(i, sData);
					return;
				}
			}
		}
	}
	void mDataPost1::deleteSetData(const QString& name)
	{
		const int nset = _setData.size();
		for (int i = 0; i < nset; ++i)
		{
			mPostSetBase* tepSet = _setData.at(i);
			if (tepSet != nullptr)
			{
				QString setname = tepSet->getSetName();
				if (setname == name)
				{
					delete tepSet;
					tepSet = nullptr;
					_setData.removeAt(i);
					return;
				}
			}
		}
	}

	void mDataPost1::deleteSetData(int i)
	{
		if (i < _setData.size())
		{
			mPostSetBase* tepSet = _setData.at(i);
			if (tepSet != nullptr)
			{
				delete tepSet;
				tepSet = nullptr;
			}
			_setData.removeAt(i);
		}
	}

	void mDataPost1::deleteSetData()
	{
		const int scount = _setData.size();
		for (int i = 0; i < scount; ++i)
		{
			mPostSetBase* tepSet = _setData.at(i);
			if (tepSet != nullptr)
			{
				delete tepSet;
				tepSet = nullptr;
			}
		}
		_setData.clear();
	}
	void mDataPost1::resetSetName(const QString& oldname, const QString& newname)
	{
		mPostSetBase *base = getSetData(oldname);
		if (base == nullptr)
		{
			return;
		}
		base->setSetName(newname);
	}
	bool mDataPost1::isSetNameExist(const QString& name)
	{
		QStringList setNames = this->getSetNames();
		if (setNames.contains(name))
		{
			return true;
		}
		return false;

	}
	QList<mPostSetBase*> mDataPost1::getTypeSetDatas(PostSetType type)
	{
		QList<mPostSetBase*> typeLBs;
		const int nlb = _setData.size();
		for (int i = 0; i < nlb; ++i)
		{
			mPostSetBase* tepLB = _setData.at(i);
			if (tepLB != nullptr)
			{
				PostSetType lbType = tepLB->getSetType();
				if (lbType == type)
				{
					typeLBs.append(tepLB);
				}
			}
		}
		return typeLBs;
	}

	QStringList mDataPost1::getTypeSetNames(PostSetType type)
	{
		QStringList names;
		const int nlb = _setData.size();
		for (int i = 0; i < nlb; ++i)
		{
			mPostSetBase* tepLB = _setData.at(i);
			if (tepLB != nullptr)
			{
				PostSetType lbType = tepLB->getSetType();
				if (lbType == type)
				{
					QString lbName = tepLB->getSetName();
					names.append(lbName);
				}
			}
		}
		return names;
	}
	void mDataPost1::appendRigidWallData(mPostRigidWallDataBase * data)
	{
		_rigidWallDatas.insert(QString("rigidwall_%1").arg(data->getRigidWallID()), data);
	}
	int mDataPost1::getRigidWallDataCount()
	{
		return _rigidWallDatas.size();
	}
	mPostRigidWallDataBase *mDataPost1::getRigidWallData(QString name)
	{
		return _rigidWallDatas.value(name);
	}
	QList<QString> mDataPost1::getAllRigidWallNames()
	{
		QList<QString> names = _rigidWallDatas.keys();
		sort(names.begin(), names.end());
		return names;
	}
}
