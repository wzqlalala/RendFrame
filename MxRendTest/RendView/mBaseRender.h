#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")

#include "app.h"
#include <QObject>
#include <QVector2D>

#include "mMeshViewEnum.h"
namespace mxr
{
	class Group;
}
namespace MViewBasic
{
	class mModelView;
	class mCommonView;
}
using namespace MViewBasic;
using namespace std;
namespace MBaseRend
{
	class RENDVIEW_EXPORT mBaseRender : public QObject
	{
		Q_OBJECT

	public:
		mBaseRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent);

		~mBaseRender();

		virtual void updateUniform(shared_ptr<mModelView> modelView, shared_ptr<mCommonView> commonView) {};

		virtual void setPickParameters(PickMode pickMode, MultiplyPickMode multiplyPickMode, QVector<QVector2D> poses) { _pickMode = pickMode; _multiplyPickMode = multiplyPickMode; _poses = poses; };

		virtual void startPick() {};

		virtual bool getIsDragSomething() { return false; };
	protected:

		void makeCurrent() { _app->GLContext()->makeCurrent(_app->GLContext()->surface()); };

		void doneCurrent() { _app->GLContext()->doneCurrent(); };

	signals:

		void update();

	private:



	protected:
		std::shared_ptr<mxr::Application> _app;

		std::shared_ptr<mxr::Group> _parent;

		PickMode _pickMode = NoPick;//当前拾取模式

		MultiplyPickMode _multiplyPickMode = QuadPick;//框选拾取模式

		//位置
		QVector<QVector2D> _poses;//单选和拖拽就是当前点，矩形和圆形框选就是中心和最终的点，多边形框选就是每一个点
	};
}