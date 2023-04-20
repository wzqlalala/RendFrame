#include "mPostAnimationRender.h"
#include <renderpch.h>
#include "Space.h"

#include "mShaderManage.h"
#include "mTextureManage.h"

// ”Õº¿‡
#include "mModelView.h"
#include "mViewBase.h"

#include <QMatrix3x3>
#include <QFileDialog>
#include <QFileInfo>
#include <QApplication>

using namespace mxr;
using namespace std;
namespace MPostRend
{
	mPostAnimationRender::mPostAnimationRender(std::shared_ptr<mxr::Group> parent):_parent(parent)
	{
		//_drawable->setStateSet(_stateSet);
		//_parent->addChild(_drawable);


	}
	mPostAnimationRender::~mPostAnimationRender()
	{

	}

}