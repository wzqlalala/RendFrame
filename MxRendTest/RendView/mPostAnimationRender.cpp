#include "mPostAnimationRender.h"
#include <renderpch.h>
#include "Space.h"

#include "mShaderManage.h"
#include "mTextureManage.h"

//视图类
#include "mModelView.h"
#include "mCommonView.h"

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
		_stateSet = MakeAsset<mxr::StateSet>();
		_drawable = MakeAsset<mxr::Drawable>();
		_drawable->setStateSet(_stateSet);
		_parent->addChild(_drawable);

		shared_ptr<Shader> shaderTest = mShaderManage::GetInstance()->GetShader("ModelTestRend");
		_stateSet->setShader(shaderTest);//设置着色器
		_stateSet->setAttributeAndModes(MakeAsset<PolygonMode>(PolygonMode::FRONT_AND_BACK, PolygonMode::FILL), 1);//填充模式
		_stateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);//深度开启
		_stateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);//混合关闭
		_stateSet->setDrawMode(GL_TRIANGLES);//三角形绘制
		_stateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_stateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_stateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_stateSet->setUniform(MakeAsset<Uniform>("viewPos", QVector3D()));
		_stateSet->setUniform(MakeAsset<Uniform>("light.position", QVector3D(0.5f, 1.0f, 0.3f)));
		_stateSet->setUniform(MakeAsset<Uniform>("light.ambient", QVector3D(0.3f, 0.3f, 0.3f)));
		_stateSet->setUniform(MakeAsset<Uniform>("light.diffuse", QVector3D(0.8f, 0.8f, 0.8f)));
		_stateSet->setUniform(MakeAsset<Uniform>("light.specular", QVector3D(0.8f, 0.8f, 0.8f)));

		_drawable->setVertexAttribArray(0, MakeAsset<Vec3Array>());
		_drawable->setVertexAttribArray(1, MakeAsset<Vec3Array>());
		_drawable->setVertexAttribArray(2, MakeAsset<Vec3Array>());
	}
	mPostAnimationRender::~mPostAnimationRender()
	{

	}

}