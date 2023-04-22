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

		mPreGeoPartRender(std::shared_ptr<mxr::Group> parent, MDataGeo::mGeoPartData1 *part, mGeoModelData1 *geoModelData);

		~mPreGeoPartRender();

		void setFaceStateSet(std::shared_ptr<mxr::StateSet> stateset);
		void setIndependentLineStateSet(std::shared_ptr<mxr::StateSet> stateset);
		void setDotLineStateSet(std::shared_ptr<mxr::StateSet> stateset);
		void setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> stateset);
		void setPointStateSet(std::shared_ptr<mxr::StateSet> stateset);
		Space::AABB getPartAABB();
		MDataGeo::mGeoPartData1 *getpartData() { return _partData; };

		void appendPart(mGeoModelData1 *geoModelData);

		void setShowFuntion(ShowFuntion showFuntion);
		std::shared_ptr<mxr::Drawable> getFaceDrawable() { return _facerend->getDrawable(); };
		std::shared_ptr<mxr::Drawable> getIndependentLineDrawable() { return _independentlinerend->getDrawable(); };
		std::shared_ptr<mxr::Drawable> getDotLineDrawable() { return _independentlinerend->getDrawable(); };
		std::shared_ptr<mxr::Drawable> getEdgeLineDrawable() { return _edgelinerend->getDrawable(); };
		std::shared_ptr<mxr::Drawable> getPointDrawable() { return _pointrend->getDrawable(); };
		std::shared_ptr<mxr::Geode> getGeode() { return _geode; }

	private:
		shared_ptr<mxr::Group> _parent;//父节点
		std::shared_ptr<mxr::Geode> _geode;//当前总节点

		std::shared_ptr<mGroupRender2<Vec3Array, Vec3Array>>  _facerend;//面
		std::shared_ptr<mGroupRender1<Vec3Array>>  _edgelinerend;//边界线
		std::shared_ptr<mGroupRender2<Vec3Array, Vec3Array>>  _independentlinerend;//独立线
		std::shared_ptr<mGroupRender2<Vec3Array, Vec3Array>>  _dotlinerend;//点画线
		std::shared_ptr<mGroupRender2<Vec3Array, Vec3Array>>  _pointrend;//点

		MDataGeo::mGeoPartData1 *_partData;

		//Space::SpaceTree *_spaceTree;
	};
	class RENDVIEW_EXPORT mPreGeoModelRender
	{
	public:

		mPreGeoModelRender(shared_ptr<mxr::Group> parent, shared_ptr<mPreRendStatus> rendStatus, mGeoModelData1 *geoModelData);

		~mPreGeoModelRender();

		bool updateRender();

		void setFaceStateSet(std::shared_ptr<mxr::StateSet> stateset);
		void setIndependentLineStateSet(std::shared_ptr<mxr::StateSet> stateset);
		void setDotLineStateSet(std::shared_ptr<mxr::StateSet> stateset);
		void setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> stateset);
		void setPointStateSet(std::shared_ptr<mxr::StateSet> stateset);

		//更新缓存
		void UpdateModelBuffer();

		//更新单个部件操作
		bool updateModelOperate(QPair<MBasicFunction::ModelOperateEnum, std::set<QString>> postModelOperates);

		void setShowFuntion(ShowFuntion showFuntion);

		Space::AABB getModelAABB();

		Space::AABB getPartAABB(QString partName);


	private:
		shared_ptr<mxr::Group> _parent;//父节点

		std::shared_ptr<mxr::Geode> _geode;//当前总节点

		std::shared_ptr<mPreRendStatus> _rendStatus;

		mGeoModelData1 *_geoModelData;

		std::shared_ptr<mxr::StateSet> _faceStateSet;//渲染面的状态

		std::shared_ptr<mxr::StateSet> _indepentdentlineStateSet;//渲染独立线的状态

		std::shared_ptr<mxr::StateSet> _dotlineStateSet;//渲染点划线的状态

		std::shared_ptr<mxr::StateSet> _pointStateSet;//渲染点的状态

		std::shared_ptr<mxr::StateSet> _edgelineStateSet;//渲染边界线的状态

		QHash<QString, std::shared_ptr<mPreGeoPartRender>> _partRenders;//部件渲染

		std::shared_ptr<mViewBase> _modelView;
		std::shared_ptr<mViewBase> _commonView;
	};
}
