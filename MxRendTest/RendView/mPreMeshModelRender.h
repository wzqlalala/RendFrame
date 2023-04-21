#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")
#include "mBaseRender.h"
#include "app.h"
//#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <QObject>
#include <QHash>
#include <set>
#include <QString>

#include "SpaceTree.h"

#include "mBasicEnum.h"
#include "mMeshViewEnum.h"
#include "mBasicStructor.h"

#include "mGroupRender.h"
namespace mxr
{
	class Shader;
	class Texture;
	class Uniform;
	class Drawable;
	class StateSet;
	class Group;
	class Geode;
	class Texture;
}
namespace MViewBasic
{
	class mModelView;
	class mViewBase;
}
namespace MDataGeo
{
	class mGeoModelData1;
	class mGeoPartData1;
}
using namespace MViewBasic;
using namespace MDataGeo;
using namespace MBasicFunction;
using namespace MBaseRend;
using namespace mxr;
using namespace std;
namespace MPreRend
{
	class mPreRendStatus;
	class RENDVIEW_EXPORT mPreGeoPartRender
	{
	public:

		mPreGeoPartRender(std::shared_ptr<mxr::Group> parent, MDataGeo::mGeoPartData1 *part);

		~mPreGeoPartRender();

		void setFaceStateSet(std::shared_ptr<mxr::StateSet> faceStateSet);
		void setFaceTransparentNodeformationStateSet(std::shared_ptr<mxr::StateSet> faceTransparentNoDeformationStateSet);
		void setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> meshlineStateSet);
		void setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet);
		void setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet);
		void setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet);

		void calculateSpaceTreeThread();
		Space::SpaceTree* getPartSpaceTree();
		MDataGeo::mGeoPartData1 *getpartData() { return _partData; };

		void ImportPart();

		void setShowFuntion(ShowFuntion showFuntion);
		std::shared_ptr<mxr::Drawable> getFaceDrawable() { return _facerend->getDrawable(); };
		std::shared_ptr<mxr::Drawable> getLineDrawable() { return _linerend->getDrawable(); };
		std::shared_ptr<mxr::Drawable> getPointDrawable() { return _pointrend->getDrawable(); };
		std::shared_ptr<mxr::Drawable> getFaceTransparentNodeformationDrawable() { return _facetransparentnodeformationrend->getDrawable(); };
		std::shared_ptr<mxr::Geode> getGeode() { return _geode; }

	private:
		shared_ptr<mxr::Group> _parent;//父节点
		std::shared_ptr<mxr::Geode> _geode;//当前总节点

		std::shared_ptr<mGroupRender5<Vec3Array, Vec3Array, FloatArray, FloatArray, Vec3Array>>  _facerend;//面
		std::shared_ptr<mGroupRender1<Vec3Array>> _facetransparentnodeformationrend;//变形前透明的面
		std::shared_ptr<mGroupRender2<Vec3Array, Vec3Array>>  _edgelinerend;//边界线
		std::shared_ptr<mGroupRender3<Vec3Array, Vec3Array, FloatArray>>  _facelinerend;//面线
		std::shared_ptr<mGroupRender5<Vec3Array, Vec3Array, FloatArray, FloatArray, Vec3Array>>  _linerend;//线
		std::shared_ptr<mGroupRender5<Vec3Array, Vec3Array, FloatArray, FloatArray, Vec3Array>>  _pointrend;//点

		MDataGeo::mGeoPartData1 *_partData;

		Space::SpaceTree *_spaceTree;
	};
	class RENDVIEW_EXPORT mPreGeoModelRender
	{
	public:

		mPreGeoModelRender(shared_ptr<mxr::Group> parent, shared_ptr<mPreRendStatus> rendStatus, mGeoModelData1 *geoModelData);

		~mPreGeoModelRender();

		void setFaceStateSet(std::shared_ptr<mxr::StateSet> faceStateSet);
		void setFaceTransparentStateSet(std::shared_ptr<mxr::StateSet> faceTransparentStateSet);
		void setFaceTransparentNoDeformationStateSet(std::shared_ptr<mxr::StateSet> faceTransparentNoDeformationStateSet);
		void setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> edgelineStateSet);
		void setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet);
		void setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet);
		void setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet);

		//更新缓存
		void UpdateModelBuffer();

		//更新单个部件操作
		void updateOneModelOperate(QPair<MBasicFunction::ModelOperateEnum, std::set<QString>> postModelOperates);
		//更新全部部件操作
		void updateAllModelOperate(MBasicFunction::ModelOperateEnum postModelOperate);

		void setShowFuntion(ShowFuntion showFuntion);

		Space::AABB getModelAABB();

		Space::SpaceTree *getPartSpaceTree(QString partName);

	private:
		shared_ptr<mxr::Group> _parent;//父节点

		std::shared_ptr<mxr::Geode> _geode;//当前总节点

		std::shared_ptr<mPreRendStatus> _rendStatus;

		mGeoModelData1 *_geoModelData;

		std::shared_ptr<mxr::StateSet> _faceStateSet;//渲染面的状态

		std::shared_ptr<mxr::StateSet> _faceTransparentStateSet;//渲染透明面的状态

		std::shared_ptr<mxr::StateSet> _faceTransparentNodeformationStateSet;//渲染模型透明面无变形的状态

		std::shared_ptr<mxr::StateSet> _facelineStateSet;//渲染面线的状态

		std::shared_ptr<mxr::StateSet> _lineStateSet;//渲染线的状态

		std::shared_ptr<mxr::StateSet> _pointStateSet;//渲染点的状态

		std::shared_ptr<mxr::StateSet> _edgelineStateSet;//渲染边界线的状态

		//零件的顺序
		QList<QString> _partOrder;
		QHash<QString, std::shared_ptr<mPreGeoPartRender>> _partRenders;
	};
}
