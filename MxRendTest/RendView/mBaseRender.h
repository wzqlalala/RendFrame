#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")

#include "app.h"
#include <QObject>
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

	signals:

		void update();

	private:



	protected:
		std::shared_ptr<mxr::Application> _app;

		std::shared_ptr<mxr::Group> _parent;
	};
}