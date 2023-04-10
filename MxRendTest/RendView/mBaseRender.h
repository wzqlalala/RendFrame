#pragma once
#include "rendview_global.h"
//�����������
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

		PickMode _pickMode = NoPick;//��ǰʰȡģʽ

		MultiplyPickMode _multiplyPickMode = QuadPick;//��ѡʰȡģʽ

		//λ��
		QVector<QVector2D> _poses;//��ѡ����ק���ǵ�ǰ�㣬���κ�Բ�ο�ѡ�������ĺ����յĵ㣬����ο�ѡ����ÿһ����
	};
}