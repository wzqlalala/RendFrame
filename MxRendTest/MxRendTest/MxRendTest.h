#pragma once

#include <QtWidgets/QMainWindow>
#include <QKeyEvent>
#include "ui_MxRendTest.h"

#include "mPostRendStatus.h"

namespace MPreRend
{
	class mPreRend;
}
namespace MPostRend
{
	class mPostRend;
	class mPostRender;
}
namespace MDataPost
{
	class mDataPost1;
	class mPostOneFrameRendData;
}
namespace MBaseRend
{
	class mModelTestRender;
	class mTestRender;
}
using namespace std;
class MxRendTest : public QMainWindow
{
    Q_OBJECT

public:
    MxRendTest(int id = 0);

protected:

	void keyPressEvent(QKeyEvent *event) override;


	bool getData(shared_ptr<MDataPost::mDataPost1> dp, MDataPost::mPostOneFrameRendData * oneFrameRendData);

private:
    Ui::MxRendTestClass ui;

	MPreRend::mPreRend *_preRend;

	MPostRend::mPostRend *_postRend;
	shared_ptr<MPostRend::mPostRender> _postRender;

	std::shared_ptr<MBaseRend::mModelTestRender> _modelRender;

	shared_ptr<MBaseRend::mTestRender> _testRender;

	//≤‚ ‘
	PostMode _postMode{ OneFrame};
	ShowFuntion _showfuntion{ ElementFace };
	bool _isShowInitialShape{ false };
	PostModelOperateEnum _modelOperate{ ColorAllPartOperate };
	PostModelOperateEnum _modelOperate1{ HideAllPartOperate };
	set<QString> _hideNames;
	bool _dispersed{ false };
	bool _isEquivariance{ true };
	float _lineWidth = 1.0f;
	int _pointSize = 1.0;
	//«–√Ê
	QVector<QVector3D> planeNormals{ QVector3D(0,0,1), QVector3D(0,1,0),QVector3D(1,0,0), QVector3D(1,1,1), QVector3D(1,1,0),QVector3D(0,1,1),QVector3D(1,0,1), QVector3D(1,2,0) };
	int _cuttingPlaneNum = 0;
	bool _isShowPlane = true;
	bool _isOnlyShowPlane = false;

	//∂Øª≠
	bool _isStartAnimation = false;
	bool _isLinearAnimation = true;

	// ∞»°
	MultiplyPickMode _multuiplyPickMode = MultiplyPickMode::QuadPick;
	QVector<PickFilter> _pickfilters{ PickFilter::PickNode, PickFilter::PickAnyMesh, PickFilter::PickMeshFace, PickFilter::Pick1DMesh, PickFilter::Pick2DMesh, PickFilter::PickNodeByLineAngle, PickFilter::PickNodeByFaceAngle, PickFilter::Pick1DMeshByAngle, PickFilter::Pick2DMeshByAngle, PickFilter::PickMeshFaceByAngle };
	int _pickfilterID = 0;

};
