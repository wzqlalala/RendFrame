#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")
#include "mBaseRender.h"
#include "app.h"
//#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <QObject>
#include <QTimer>
#include <QThread>
#include <QFutureWatcher>

//MViewBasic
#include "mMeshViewEnum.h"

//MBasicFunction
#include "mBasicEnum.h"
#include "mBasicStructor.h"

#include "SpaceTree.h"

namespace mxr
{
	class Shader;
	class Texture;
	class Uniform;
	class Drawable;
	class StateSet;
	class Group;
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
	class mGeoPickData1;
	class mPreGeoPickThread;
}

using namespace MViewBasic;
using namespace MBasicFunction;
using namespace MBaseRend;
using namespace MDataGeo;
using namespace std;
namespace MPreRend
{
	class mPreRend;
	class mPreGeoModelRender;
	class mPreRendStatus;
	class mPreHighLightRender;
	class RENDVIEW_EXPORT mPreRender :public mBaseRender
	{
		Q_OBJECT
	public:

		mPreRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mPreRend *preRend);

		bool getIsDragSomething(QVector2D pos) override;

		void dragSomething(QVector2D pos) override;

		void startPick(QVector<QVector2D> poses) override;

		bool getModelAABB(Space::AABB &aabb);

		mGeoModelData1 *getGeoModelData() { return _geoModelData; }; 

		//mPreMeshPickData *getMeshPickData() { return _pickData; };

		//更新高亮渲染
		void updateHighLightRender();

		//更新部件操作
		void updateModelOperate(QPair<MBasicFunction::ModelOperateEnum, std::set<QString>> modelOperates);
		
		//清除全部的渲染
		void clearRender();
		
		//设置当前的显示模式	
		void setShowFuntion(ShowFuntion shoFuntion);

		//设置点的大小
		void setPointSize(int size);

		//设置边界线的宽度
		void setEdgeLineWidth(float size);

		//设置线的颜色
		void setFaceLineColor(QVector4D color);


		~mPreRender();

		void updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView) override;

	private:

		//初始化线程
		void initialPickThreads();

	protected:

		shared_ptr<mPreRendStatus> _rendStatus;

		shared_ptr<mPreGeoModelRender> _geoModelRender;//几何渲染

		mGeoModelData1 *_geoModelData;//模型数据

		mPreGeoPickThread *_geoPickThread;//拾取
		QFutureWatcher<void> w;
		mGeoPickData1 *_pickData;//拾取数据

		shared_ptr<mxr::Texture> _pointTexture;

		/*********************************模型*****************************************************/
		std::shared_ptr<mxr::StateSet> _faceStateSet;//渲染面的状态
		std::shared_ptr<mxr::StateSet> _independentlineStateSet;//渲染独立线的状态
		std::shared_ptr<mxr::StateSet> _dotlineStateSet;//渲染线的状态
		std::shared_ptr<mxr::StateSet> _edgelineStateSet;//渲染边界线的状态
		std::shared_ptr<mxr::StateSet> _pointStateSet;//渲染点的状态
		std::shared_ptr<mxr::StateSet> _facelineStateSet;//渲染面线的状态

		//计时器
		QTimer* _aniTimer;

		//拾取线程
		////前面的部件数量少
		//mPostMeshPickThread *_thread; QFutureWatcher<void> w;
		//mPostMeshPickData *_pickData;
		//shared_ptr<mPreHighLightRender> _highLightRender;
		
	};
}
