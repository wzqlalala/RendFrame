#include "mTestRender.h"
#include "mShaderManage.h"
#include "mTextureManage.h"
//视图类
#include "mModelView.h"
#include "mCommonView.h"

#include <renderpch.h>

#include <qapplication.h>
using namespace mxr;
using namespace std;
namespace MBaseRend
{
	float z{0.0f};
	mTestRender::mTestRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent):mBaseRender(app, parent)
	{
		//QOpenGLContext *context = _app->GLContext();
		//QSurface *surface = context->surface();
		//context->makeCurrent(surface);

		_stateSet = MakeAsset<mxr::StateSet>();

		shared_ptr<Shader> shaderTest = mShaderManage::GetInstance()->GetShader("testRend");
		//shared_ptr<Shader> shaderTest = MakeAsset<Shader>(qApp->applicationDirPath().toStdString() + "/../GLSL/testRend.glsl");

		_stateSet->setShader(shaderTest);//设置着色器
		_stateSet->setAttributeAndModes(MakeAsset<PolygonMode>(PolygonMode::FRONT_AND_BACK, PolygonMode::FILL), 1);//填充模式
		_stateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);//深度开启
		_stateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);//混合关闭
		_stateSet->setDrawMode(GL_TRIANGLES);//三角形绘制
		_stateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_stateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_stateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_stateSet->setUniform(MakeAsset<Uniform>("viewPos", QVector3D()));
		_stateSet->setUniform(MakeAsset<Uniform>("lightPos", QVector3D(0.5f, 1.0f, 0.3f)));


		//_drawable->setIndexAttribute(MakeAsset<UIntArray>(QVector<int>{0, 1, 2}));
		//appendOnePart();
	}

	void mTestRender::appendOnePart()
	{
		std::shared_ptr<mxr::Drawable> _drawable = MakeAsset<mxr::Drawable>();
		_drawable->setStateSet(_stateSet);
		_parent->addChild(_drawable);

		/**纹理**/
		_diffuseMap = mTextureManage::GetInstance()->GetTexture("brickwall.jpg", 3);
		_normalMap = mTextureManage::GetInstance()->GetTexture("brickwall_normal.jpg", 3);
		//_diffuseMap = MakeAsset<Texture>(qApp->applicationDirPath().toStdString() + "/../Texture/brickwall.jpg", 3);
		//_normalMap = MakeAsset<Texture>(qApp->applicationDirPath().toStdString() + "/../Texture/brickwall_normal.jpg", 3);
		_stateSet->setTexture("diffuseMap", _diffuseMap);
		_stateSet->setTexture("normalMap", _normalMap);

		//设置顶点属性
		QVector3D pos1(-1.0f, 1.0f, 0.0f + z);
		QVector3D pos2(-1.0f, -1.0f, 0.0f + z);
		QVector3D pos3(1.0f, -1.0f, 0.0f + z);
		QVector3D pos4(1.0f, 1.0f, 0.0f + z);
		// texture coordinates
		QVector2D uv1(0.0f, 1.0f);
		QVector2D uv2(0.0f, 0.0f);
		QVector2D uv3(1.0f, 0.0f);
		QVector2D uv4(1.0f, 1.0f);

		z -= 0.5;

		//三角形1
		QVector3D edge1 = pos2 - pos1;
		QVector3D edge2 = pos3 - pos1;
		QVector2D deltaUV1 = uv2 - uv1;
		QVector2D deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x()* deltaUV2.y() - deltaUV2.x() * deltaUV1.y());

		QVector3D tangent1, bitangent1;
		QVector3D tangent2, bitangent2;
		tangent1.setX(f * (deltaUV2.y() * edge1.x() - deltaUV1.y() * edge2.x()));
		tangent1.setY(f * (deltaUV2.y() * edge1.y() - deltaUV1.y() * edge2.y()));
		tangent1.setZ(f * (deltaUV2.y() * edge1.z() - deltaUV1.y() * edge2.z()));

		bitangent1.setX(f * (-deltaUV2.x() * edge1.x() + deltaUV1.x() * edge2.x()));
		bitangent1.setY(f * (-deltaUV2.x() * edge1.y() + deltaUV1.x() * edge2.y()));
		bitangent1.setZ(f * (-deltaUV2.x() * edge1.z() + deltaUV1.x() * edge2.z()));

		//三角形2
		edge1 = pos3 - pos1;
		edge2 = pos4 - pos1;
		deltaUV1 = uv3 - uv1;
		deltaUV2 = uv4 - uv1;

		f = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV2.x() * deltaUV1.y());

		tangent2.setX(f * (deltaUV2.y() * edge1.x() - deltaUV1.y() * edge2.x()));
		tangent2.setY(f * (deltaUV2.y() * edge1.y() - deltaUV1.y() * edge2.y()));
		tangent2.setZ(f * (deltaUV2.y() * edge1.z() - deltaUV1.y() * edge2.z()));


		bitangent2.setX(f * (-deltaUV2.x() * edge1.x() + deltaUV1.x() * edge2.x()));
		bitangent2.setY(f * (-deltaUV2.x() * edge1.y() + deltaUV1.x() * edge2.y()));
		bitangent2.setZ(f * (-deltaUV2.x() * edge1.z() + deltaUV1.x() * edge2.z()));

		shared_ptr<Vec3Array> vertex = MakeAsset<Vec3Array>(QVector<QVector3D>{pos1, pos2, pos3, pos1, pos3, pos4 });
		shared_ptr<Vec2Array> coord = MakeAsset<Vec2Array>(QVector<QVector2D>{uv1, uv2, uv3, uv1, uv3, uv4});
		shared_ptr<Vec3Array> norm = MakeAsset<Vec3Array>(QVector<QVector3D>(6, QVector3D(0.0f, 0.0f, 1.0f)));
		shared_ptr<Vec3Array> tangent = MakeAsset<Vec3Array>(QVector<QVector3D>(3, tangent1) + QVector<QVector3D>(3, tangent2));
		shared_ptr<Vec3Array> bitangent = MakeAsset<Vec3Array>(QVector<QVector3D>(3, bitangent1) + QVector<QVector3D>(3, bitangent2));

		_drawable->setVertexAttribArray(0, vertex);
		_drawable->setVertexAttribArray(1, norm);
		_drawable->setVertexAttribArray(2, coord);
		_drawable->setVertexAttribArray(3, tangent);
		_drawable->setVertexAttribArray(4, bitangent);
	}

	mTestRender::~mTestRender()
	{
		if (_parent)
		{
			_parent->removeAllChild();
		}
	}
	void mTestRender::updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView)
	{
		_stateSet->getUniform("projection")->SetData(modelView->_projection);
		_stateSet->getUniform("view")->SetData(modelView->_view);
		_stateSet->getUniform("model")->SetData(modelView->_model);
		//_stateSet->getUniform("lightPos")->SetData(QVector3D(0.5f, 1.0f, 0.3f));
		_stateSet->getUniform("viewPos")->SetData(modelView->_Eye);
		_stateSet->setTexture("diffuseMap", _diffuseMap);
		_stateSet->setTexture("normalMap", _normalMap);
	}
}
