#include "mGeoModelData1.h"
#include "mGeoPartData1.h"
#include "mGeoSolidData1.h"
#include "mGeoFaceData1.h"
#include "mGeoLineData1.h"
#include "mGeoPointData1.h"

namespace MDataGeo
{

	set<int> mGeoModelData1::getAllGeoPointIDs()
	{
		set<int> ids;
		QHashIterator<QString, mGeoPartData1*> iter(_geoPartData);
		while (iter.hasNext())
		{
			iter.next();
			mGeoPartData1 *partData = iter.value();
			if (partData == nullptr)
			{
				continue;
			}
			if (!partData->getPartVisual())
			{
				continue;
			}
			set<int> geoPointIDs = partData->getGeoPointIDs();
			ids.insert(geoPointIDs.begin(), geoPointIDs.end());
		}
		return ids;
	}

	set<int> mGeoModelData1::getAllGeoPointIDsInPart(QString partName)
	{
		set<int> ids;
		mGeoPartData1 *partData = _geoPartData.value(partName);
		if (partData == nullptr)
		{
			return ids;
		}
		if (!partData->getPartVisual())
		{
			return ids;
		}
		ids = partData->getGeoPointIDs();
		return ids;
	}

	set<int> mGeoModelData1::getAllGeoLineIDs()
	{
		set<int> ids;
		QHashIterator<QString, mGeoPartData1*> iter(_geoPartData);
		while (iter.hasNext())
		{
			iter.next();
			mGeoPartData1 *partData = iter.value();
			if (partData == nullptr)
			{
				continue;
			}
			if (!partData->getPartVisual())
			{
				continue;
			}
			set<int> geoLineIDs = partData->getGeoLineIDs();
			ids.insert(geoLineIDs.begin(), geoLineIDs.end());
		}
		return ids;
	}

	set<int> mGeoModelData1::getAllGeoLineIDsInPart(QString partName)
	{
		set<int> ids;
		mGeoPartData1 *partData = _geoPartData.value(partName);
		if (partData == nullptr)
		{
			return ids;
		}
		if (!partData->getPartVisual())
		{
			return ids;
		}
		ids = partData->getGeoLineIDs();
		return ids;
	}

	set<int> mGeoModelData1::getAllGeoFaceIDs()
	{
		set<int> ids;
		QHashIterator<QString, mGeoPartData1*> iter(_geoPartData);
		while (iter.hasNext())
		{
			iter.next();
			mGeoPartData1 *partData = iter.value();
			if (partData == nullptr)
			{
				continue;
			}
			if (!partData->getPartVisual())
			{
				continue;
			}
			set<int> geoFaceIDs = partData->getGeoFaceIDs();
			ids.insert(geoFaceIDs.begin(), geoFaceIDs.end());
		}
		return ids;
	}

	set<int> mGeoModelData1::getAllGeoFaceIDsInPart(QString partName)
	{
		set<int> ids;
		mGeoPartData1 *partData = _geoPartData.value(partName);
		if (partData == nullptr)
		{
			return ids;
		}
		if (!partData->getPartVisual())
		{
			return ids;
		}
		ids = partData->getGeoFaceIDs();
		return ids;
	}

	set<int> mGeoModelData1::getAllGeoSolidIDs()
	{
		set<int> ids;
		QHashIterator<QString, mGeoPartData1*> iter(_geoPartData);
		while (iter.hasNext())
		{
			iter.next();
			mGeoPartData1 *partData = iter.value();
			if (partData == nullptr)
			{
				continue;
			}
			if (!partData->getPartVisual())
			{
				continue;
			}
			set<int> geoSolidIDs = partData->getGeoSolidIDs();
			ids.insert(geoSolidIDs.begin(), geoSolidIDs.end());
		}
		return ids;
	}

	set<int> mGeoModelData1::getAllGeoSolidIDsInPart(QString partName)
	{
		set<int> ids;
		mGeoPartData1 *partData = _geoPartData.value(partName);
		if (partData == nullptr)
		{
			return ids;
		}
		if (!partData->getPartVisual())
		{
			return ids;
		}
		ids = partData->getGeoSolidIDs();
		return ids;
	}

	set<QString> mGeoModelData1::getAllGeoPartNames()
	{
		set<QString> partNames;
		QHashIterator<QString, mGeoPartData1*> iter(_geoPartData);
		while (iter.hasNext())
		{
			iter.next();
			mGeoPartData1 *partData = iter.value();
			if (partData == nullptr)
			{
				continue;
			}
			if (!partData->getPartVisual())
			{
				continue;
			}
			partNames.insert(iter.key());
		}
		return partNames;
	}

	QString mGeoModelData1::getPartNameByGeoPointID(int PointID)
	{
		mGeoPointData1 *geoPointData = _geoPointData.value(PointID);
		if (geoPointData == nullptr)
		{
			return "";
		}
		return geoPointData->getPartName();
	}

	QString mGeoModelData1::getPartNameByGeoLineID(int LineID)
	{
		mGeoLineData1 *geoLineData = _geoLineData.value(LineID);
		if (geoLineData == nullptr)
		{
			return "";
		}
		return geoLineData->getPartName();
	}

	QString mGeoModelData1::getPartNameByGeoFaceID(int FaceID)
	{
		mGeoFaceData1 *geoFaceData = _geoFaceData.value(FaceID);
		if (geoFaceData == nullptr)
		{
			return "";
		}
		return geoFaceData->getPartName();
	}

	QString mGeoModelData1::getPartNameByGeoSolidID(int SolidID)
	{
		mGeoSolidData1 *geoSolidData = _geoSolidData.value(SolidID);
		if (geoSolidData == nullptr)
		{
			return "";
		}
		return geoSolidData->getPartName();
	}

	mGeoPartData1 * mGeoModelData1::getGeoPartDataByPartName(QString partName)
	{
		return _geoPartData.value(partName);
	}

	mGeoSolidData1 * mGeoModelData1::getGeoSolidDataByID(int ID)
	{
		return _geoSolidData.value(ID);
	}

	mGeoFaceData1 * mGeoModelData1::getGeoFaceDataByID(int ID)
	{
		return _geoFaceData.value(ID);
	}

	mGeoLineData1 * mGeoModelData1::getGeoLineDataByID(int ID)
	{
		return _geoLineData.value(ID);
	}

	mGeoPointData1 * mGeoModelData1::getGeoPointDataByID(int ID)
	{
		return _geoPointData.value(ID);
	}

	Space::AABB mGeoModelData1::getGeoPartAABBByPartName(QString partName)
	{
		return _geoPartData.value(partName)->getGeoPartAABB();
	}

	Space::AABB mGeoModelData1::getGeoSolidAABBByID(int ID)
	{
		return _geoSolidData.value(ID)->getGeoSolidAABB();
	}

	Space::AABB mGeoModelData1::getGeoFaceAABBByID(int ID)
	{
		return _geoFaceData.value(ID)->getGeoFaceAABB();
	}

	Space::AABB mGeoModelData1::getGeoLineAABBByID(int ID)
	{
		return _geoLineData.value(ID)->getGeoLineAABB();
	}

	Space::AABB mGeoModelData1::getGeoPointAABBByID(int ID)
	{
		return _geoPointData.value(ID)->getGeoPointAABB();
	}

	QList<QString> mGeoModelData1::getAllPartName()
	{
		return _geoPartData.keys();
	}

	QList<int> mGeoModelData1::getAllGeoSolidID()
	{
		return _geoSolidData.keys();
	}

	QList<int> mGeoModelData1::getAllGeoFaceID()
	{
		return _geoFaceData.keys();
	}

	QList<int> mGeoModelData1::getAllGeoLineID()
	{
		return _geoLineData.keys();
	}

	QList<int> mGeoModelData1::getAllGeoPointID()
	{
		return _geoPointData.keys();
	}

	QHashIterator<QString, mGeoPartData1*> mGeoModelData1::getPartIterator()
	{
		return QHashIterator<QString, mGeoPartData1*>(_geoPartData);
	}

	//void mGeoModelData1::setModelRadiusAndCenter(float modelRadius, QVector3D modelCenter)
	//{
	//	_modelRadius = modelRadius;
	//	_modelCenter = modelCenter;
	//}

	Space::AABB mGeoModelData1::getModelSize()
	{
		if (_isAddOrDeletePart)
		{
			caculateGeoModelRadiusAndCenter();
			_isAddOrDeletePart = false;
		}
		return _modelSize;
	}

	bool mGeoModelData1::isExistModel()
	{
		return !_geoPartData.isEmpty();
	}

	bool mGeoModelData1::isShowModel()
	{
		if (!isExistModel())
		{
			return false;
		}
		if (_isAddOrDeletePart)
		{
			caculateGeoModelRadiusAndCenter();
			_isAddOrDeletePart = false;
		}
		if (_modelSize.maxEdge.x() < _modelSize.minEdge.x())
		{
			return false;
		}
		return true;
	}

	void mGeoModelData1::importGeoPartData(QString partName, mGeoPartData1* geoPartData)
	{
		_isAddOrDeletePart = true;
		_geoPartData.insert(partName, geoPartData);
		ModelOperateEnum operate = ImportPart;
		_modelOperate.append({ operate, set<QString>{partName} });
	}

	void mGeoModelData1::replaceGeoPartData(QString partName, mGeoPartData1 * geoPartData)
	{
		this->deleteGeoPart(partName);
		_geoPartData.insert(partName, geoPartData);
		_isAddOrDeletePart = true;
		ModelOperateEnum operate = ReplaceOnePart;
		_modelOperate.append({ operate, set<QString>{partName} });
	}

	void mGeoModelData1::renameGeoPart(QString newName, QString oldName)
	{
		mGeoPartData1 *geoPartData = this->getGeoPartDataByPartName(oldName);
		if (geoPartData == nullptr)
		{
			return;
		}
		_geoPartData.remove(oldName);
		_geoPartData.insert(newName, geoPartData);
		ModelOperateEnum operate = RenameOnePart;
		_modelOperate.append({ operate, set<QString>{newName, oldName} });
		//重命名part的实体
		const set<int> &solidIDs = geoPartData->getGeoSolidIDs();
		for (int solidID : solidIDs)
		{
			mGeoSolidData1 *geoSolidData = _geoSolidData.value(solidID);
			if (geoSolidData == nullptr)
			{
				continue;
			}
			geoSolidData->setPartName(newName);
		}
		//删除part的面
		const set<int> &faceIDs = geoPartData->getGeoFaceIDs();
		for (int faceID : faceIDs)
		{
			mGeoFaceData1 *geoFaceData = _geoFaceData.value(faceID);
			if (geoFaceData == nullptr)
			{
				continue;
			}
			geoFaceData->setPartName(newName);
		}
		//删除part的线
		const set<int> &lineIDs = geoPartData->getGeoLineIDs();
		for (int lineID : lineIDs)
		{
			mGeoLineData1 *geoLineData = _geoLineData.value(lineID);
			if (geoLineData == nullptr)
			{
				continue;
			}
			geoLineData->setPartName(newName);
		}
		//删除part的点
		const set<int> &pointIDs = geoPartData->getGeoPointIDs();
		for (int pointID : pointIDs)
		{
			mGeoPointData1 *geoPointData = _geoPointData.value(pointID);
			if (geoPointData == nullptr)
			{
				continue;
			}
			geoPointData->setPartName(newName);
		}

	}

	void mGeoModelData1::appendGeoPartData(QString partName, mGeoPartData1* geoPartData)
	{
		_isAddOrDeletePart = true;
		ModelOperateEnum operate = AppendOnePart;
		if (_geoPartData.contains(partName))
		{
			deleteGeoPart(partName);
		}
		_geoPartData.insert(partName, geoPartData);
		_modelOperate.append({ operate, set<QString>{partName} });
	}

	void mGeoModelData1::appendGeoSolidData(int ID, mGeoSolidData1 * data)
	{
		_geoSolidData.insert(ID, data);
	}

	void mGeoModelData1::appendGeoFaceData(int ID, mGeoFaceData1 * data)
	{
		_geoFaceData.insert(ID, data);
	}

	void mGeoModelData1::appendGeoLineData(int ID, mGeoLineData1 * data)
	{
		_geoLineData.insert(ID, data);
	}

	void mGeoModelData1::appendGeoPointData(int ID, mGeoPointData1 * data)
	{
		_geoPointData.insert(ID, data);
	}

	void mGeoModelData1::setGeoPartVisual(QString partName, bool isShow)
	{
		ModelOperateEnum operate = ShowOnePart;
		if (!isShow)
		{
			operate = HideOnePart;
		}
		mGeoPartData1* geoPartData = _geoPartData.value(partName);
		if (geoPartData!=nullptr)
		{
			geoPartData->setPartVisual(isShow);
			_modelOperate.append({ operate, set<QString>{partName} });
		}
		caculateGeoModelRadiusAndCenter();
	}

	void mGeoModelData1::setGeoPartColor(QString partName, QVector3D color)
	{
		ModelOperateEnum operate = ColorOnePart;
		mGeoPartData1* geoPartData = _geoPartData.value(partName);
		if (geoPartData != nullptr)
		{
			geoPartData->setPartColor(color);
			_modelOperate.append({ operate, set<QString>{partName} });
		}
	}

	void mGeoModelData1::deleteGeoPart(QString partName)
	{
		mGeoPartData1* geoPartData = _geoPartData.value(partName);
		if (geoPartData == nullptr)
		{
			return;
		}

		_isAddOrDeletePart = true;
		//删除part的实体
		const set<int> &solidIDs = geoPartData->getGeoSolidIDs();
		for(int solidID : solidIDs)
		{
			mGeoSolidData1 *geoSolidData = _geoSolidData.value(solidID);
			if (geoSolidData != nullptr)
			{
				delete geoSolidData;
				geoSolidData = nullptr;
			}
			_geoSolidData.remove(solidID);
		}
		//删除part的面
		const set<int> &faceIDs = geoPartData->getGeoFaceIDs();
		for (int faceID : faceIDs)
		{
			mGeoFaceData1 *geoFaceData = _geoFaceData.value(faceID);
			if (geoFaceData != nullptr)
			{
				delete geoFaceData;
				geoFaceData = nullptr;
			}
			_geoFaceData.remove(faceID);
		}
		//删除part的线
		const set<int> &lineIDs = geoPartData->getGeoLineIDs();
		for (int lineID : lineIDs)
		{
			mGeoLineData1 *geoLineData = _geoLineData.value(lineID);
			if (geoLineData != nullptr)
			{
				delete geoLineData;
				geoLineData = nullptr;
			}
			_geoLineData.remove(lineID);
		}
		//删除part的点
		const set<int> &pointIDs = geoPartData->getGeoPointIDs();
		for (int pointID : pointIDs)
		{
			mGeoPointData1 *geoPointData = _geoPointData.value(pointID);
			if (geoPointData != nullptr)
			{
				delete geoPointData;
				geoPointData = nullptr;
			}
			_geoPointData.remove(pointID);
		}

		delete geoPartData;
		geoPartData = nullptr;
		_geoPartData.remove(partName);

		ModelOperateEnum operate = DeleteOnePart;
		_modelOperate.append({ operate, set<QString>{partName} });
		if (_geoPartData.size() == 0)
		{
			_modelSize = Space::AABB();
		}
		//caculateGeoModelRadiusAndCenter();
	}

	void mGeoModelData1::setAllGeoPartVisual(bool isShow)
	{
		ModelOperateEnum operate = ShowAllPart;
		if (!isShow)
		{
			operate = HideAllPart;
		}
		_modelOperate.append({ operate,set<QString>{} });
		QHashIterator<QString, mGeoPartData1*> iter(_geoPartData);
		while (iter.hasNext())
		{
			iter.next();
			iter.value()->setPartVisual(isShow);
		}

		caculateGeoModelRadiusAndCenter();
	}

	void mGeoModelData1::setAllGeoPartColor(QVector3D color)
	{
		ModelOperateEnum operate = ColorAllPart;
		_modelOperate.append({ operate,set<QString>{} });
		QHashIterator<QString, mGeoPartData1*> iter(_geoPartData);
		while (iter.hasNext())
		{
			iter.next();
			iter.value()->setPartColor(color);
		}
	}

	void mGeoModelData1::deleteAllGeoPart()
	{
		ModelOperateEnum operate = DeleteAllPart;
		_modelOperate.append({ operate,set<QString>{} });

		QList<int> allPointIDs = _geoPointData.keys();
		for_each(allPointIDs.begin(), allPointIDs.end(), [this](int id) {mGeoPointData1* pointData = _geoPointData.value(id); delete pointData; pointData = nullptr; });
		QList<int> allGeoLineIDs = _geoLineData.keys();
		for_each(allGeoLineIDs.begin(), allGeoLineIDs.end(), [this](int id) {mGeoLineData1* geoLineData = _geoLineData.value(id); delete geoLineData; geoLineData = nullptr; });
		QList<int> allGeoFaceIDs = _geoFaceData.keys();
		for_each(allGeoFaceIDs.begin(), allGeoFaceIDs.end(), [this](int id) {mGeoFaceData1* geoFaceData = _geoFaceData.value(id); delete geoFaceData; geoFaceData = nullptr; });
		QList<int> allGeoSolidIDs = _geoSolidData.keys();
		for_each(allGeoSolidIDs.begin(), allGeoSolidIDs.end(), [this](int id) {mGeoSolidData1* geoSolidData = _geoSolidData.value(id); delete geoSolidData; geoSolidData = nullptr; });
		QList<QString> allPartNames = _geoPartData.keys();
		for_each(allPartNames.begin(), allPartNames.end(), [this](QString name) {mGeoPartData1* partData = _geoPartData.value(name); delete partData; partData = nullptr; });

		_modelSize = Space::AABB();

		_geoPointData.clear();
		_geoLineData.clear();
		_geoFaceData.clear();
		_geoSolidData.clear();
		_geoPartData.clear();
	}

	QVector3D mGeoModelData1::getGeoPartColor(QString partName)
	{
		mGeoPartData1* geoPartData = _geoPartData.value(partName);
		if (geoPartData != nullptr)
		{
			return	geoPartData->getPartColor()*255;
		}
		else
		{
			return QVector3D();
		}
	}

	bool mGeoModelData1::getGeoPartVisual(QString partName)
	{
		mGeoPartData1* geoPartData = _geoPartData.value(partName);
		if (geoPartData != nullptr)
		{
			return  geoPartData->getPartVisual();
		}
	}

	bool mGeoModelData1::isExistGeoPart(QString partName)
	{
		return _geoPartData.contains(partName);
	}

	void mGeoModelData1::setGeoLineProperty(set<int> geoLineIDs, GeoLineProperty geoLineProperty)
	{
		set<QString> partNames;
		for (int geoLineID : geoLineIDs)
		{
			mGeoLineData1 *geoLineData = _geoLineData.value(geoLineID);
			if (geoLineData == nullptr)
			{
				return;
			}
			geoLineData->setGeoLineProperty(geoLineProperty);
			QString partName = geoLineData->getPartName();

			partNames.insert(partName);
		}
		ModelOperateEnum operate = ReplaceOnePart;
		_modelOperate.append({ operate, partNames });
	}

	mGeoModelData1::mGeoModelData1()
	{
		_modelSize = Space::AABB();
		_isAddOrDeletePart = false;
	}
	mGeoModelData1::~mGeoModelData1()
	{
		deleteAllGeoPart();
	}
	void mGeoModelData1::caculateGeoModelRadiusAndCenter()
	{
		Space::AABB aabb;
		if (!_geoPartData.isEmpty())
		{
			QHashIterator<QString, mGeoPartData1*> iter(_geoPartData);
			while (iter.hasNext())
			{
				iter.next();
				if (!iter.value()->getPartVisual())
				{
					continue;
				}
				aabb.push(iter.value()->getGeoPartAABB());
			}
		}
		
		_modelSize = aabb;
	}
}
