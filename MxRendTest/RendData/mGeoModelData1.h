#pragma once
#include "renddata_global.h"

#include <QHash>
#include <set>
#include <QVector3D>

#include "mBasicEnum.h"
#include "mBasicStructor.h"

#include "SpaceTree.h"

using namespace std;
using namespace MBasicFunction;
namespace MDataGeo
{
	class mGeoPartData1;
	class mGeoPointData1;
	class mGeoLineData1;
	class mGeoFaceData1;
	class mGeoSolidData1;
	class RENDDATA_EXPORT mGeoModelData1
	{
	public:
		//默认构造函数
		mGeoModelData1();

		//析构函数
		~mGeoModelData1();

		/*
		 * 模型操作
		 */

		//获取所有的几何点(除去隐藏的几何部件)
		set<int> getAllGeoPointIDs();
		//获取一个部件的所有几何点(除去隐藏的几何部件)
		set<int> getAllGeoPointIDsInPart(QString partName);

		//获取所有的几何线(除去隐藏的几何部件)
		set<int> getAllGeoLineIDs();
		//获取一个部件的所有几何线(除去隐藏的几何部件)
		set<int> getAllGeoLineIDsInPart(QString partName);

		//获取所有的几何面(除去隐藏的几何部件)
		set<int> getAllGeoFaceIDs();
		//获取一个部件的所有几何面(除去隐藏的几何部件)
		set<int> getAllGeoFaceIDsInPart(QString partName);

		//获取所有的几何体(除去隐藏的几何部件)
		set<int> getAllGeoSolidIDs();
		//获取一个部件的所有几何体(除去隐藏的几何部件)
		set<int> getAllGeoSolidIDsInPart(QString partName);

		//获取全部的零件(除去隐藏的几何部件)
		set<QString> getAllGeoPartNames();

		//通过几何点获取零件名称
		QString getPartNameByGeoPointID(int PointID);
		//通过几何线获取零件名称
		QString getPartNameByGeoLineID(int LineID);
		//通过几何面获取零件名称
		QString getPartNameByGeoFaceID(int FaceID);
		//通过几何实体获取零件名称
		QString getPartNameByGeoSolidID(int SolidID);

		 //通过ID(名称)获取相应的数据
		mGeoPartData1 *getGeoPartDataByPartName(QString partName);
		mGeoSolidData1 *getGeoSolidDataByID(int ID);
		mGeoFaceData1 *getGeoFaceDataByID(int ID);
		mGeoLineData1* getGeoLineDataByID(int ID);
		mGeoPointData1* getGeoPointDataByID(int ID);

		//通过ID(名称)获取相应的包围盒
		Space::AABB getGeoPartAABBByPartName(QString partName);
		Space::AABB getGeoSolidAABBByID(int ID);
		Space::AABB getGeoFaceAABBByID(int ID);
		Space::AABB getGeoLineAABBByID(int ID);
		Space::AABB getGeoPointAABBByID(int ID);

		//获取相应数据所有的ID(名称)
		QList<QString> getAllPartName();
		QList<int> getAllGeoSolidID();
		QList<int> getAllGeoFaceID();
		QList<int> getAllGeoLineID();
		QList<int> getAllGeoPointID();

		//获取模型的所有部件迭代器
		QHashIterator<QString, mGeoPartData1*> getPartIterator();

		////设置模型的最大半径和中心
		//void setModelRadiusAndCenter(float modelRadius, QVector3D modelCenter);

		//获取模型的尺寸
		Space::AABB getModelSize();

		//判断是否存在模型
		bool isExistModel();

		//判断是否有模型显示
		bool isShowModel();

		/*
		 * 部件操作
		 */

		//读文件添加数据，不自动更新渲染
		void importGeoPartData(QString partName, mGeoPartData1 *geoPartData);

		//几何清理时替换部件
		void replaceGeoPartData(QString partName, mGeoPartData1* geoPartData);

		//重命名几何部件
		void renameGeoPart(QString newName, QString oldName);

		//添加几何数据
		void appendGeoPartData(QString partName, mGeoPartData1 *geoPartData);
		void appendGeoSolidData(int ID, mGeoSolidData1 *data);
		void appendGeoFaceData(int ID, mGeoFaceData1 *data);
		void appendGeoLineData(int ID, mGeoLineData1 *data);
		void appendGeoPointData(int ID, mGeoPointData1 *data);

		//设置单个几何部件的显隐
		void setGeoPartVisual(QString partName,bool isShow);

		//设置单个几何部件的颜色
		void setGeoPartColor(QString partName, QVector3D color);

		//删除单个几何部件
		void deleteGeoPart(QString partName);

		//设置全部几何部件的显隐
		void setAllGeoPartVisual(bool isShow);

		//设置全部几何部件的颜色
		void setAllGeoPartColor(QVector3D color);

		//删除全部几何部件
		void deleteAllGeoPart();

		//获取几何部件的颜色
		QVector3D getGeoPartColor(QString partName);

		//获取单个几何部件的显隐
		bool getGeoPartVisual(QString partName);
		/*
		 * 判断是否有该部件通过名字
		 */
		bool isExistGeoPart(QString partName);

		//几何线操作
		void setGeoLineProperty(set<int> geoLineIDs, GeoLineProperty geoLineProperty);
	private:

		//计算几何中心和半径
		void caculateGeoModelRadiusAndCenter();
	public:

		//模型操作枚举
		QList<QPair<MBasicFunction::ModelOperateEnum, std::set<QString>>> _modelOperate;

	private:
		QHash<int, mGeoPointData1*> _geoPointData;
		QHash<int, mGeoLineData1*> _geoLineData;
		QHash<int, mGeoFaceData1*> _geoFaceData;
		QHash<int, mGeoSolidData1*> _geoSolidData;
		QHash<QString, mGeoPartData1*> _geoPartData;

		//模型中心
		//QVector3D _modelCenter;

		//模型半径
		//float _modelRadius;

		//模型的尺寸
		Space::AABB _modelSize;

		//是否对模型进行删除或者添加,用于重新计算几何模型的半径和中心
		bool _isAddOrDeletePart;
	};
}

