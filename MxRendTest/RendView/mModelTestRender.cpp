#include "mModelTestRender.h"
#include <renderpch.h>
#include "Space.h"

#include "mShaderManage.h"
#include "mTextureManage.h"

//视图类
#include "mModelView.h"
#include "mViewBase.h"

#include <QMatrix3x3>
#include <QFileDialog>
#include <QFileInfo>
#include <QApplication>

using namespace mxr;
using namespace std;
namespace MBaseRend
{
	mModelTestRender::mModelTestRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent):mBaseRender(app, parent)
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
	mModelTestRender::~mModelTestRender()
	{
		_parent->removeChild(_drawable);
	}
	QPair<QVector3D, float> mModelTestRender::setData(const tinyobj::attrib_t & attrib, const std::vector<tinyobj::shape_t>& shapes, const std::vector<tinyobj::material_t>& materials)
	{
		AABB aabb;
		struct Material
		{
			QVector3D ambient;
			QVector3D diffuse;
			QVector3D specular;
		};

		int n = attrib.vertices.size();
		if (materials.size() == 0)
		{
			QVector<QVector3D> vertexs;
			QVector<QVector3D> normals;
			QVector<QVector3D> colors;
			vertexs.reserve(n / 3);
			normals.reserve(n / 3);
			colors.reserve(n / 3);
			for (int i = 0; i < n; i += 3)
			{
				vertexs.append(QVector3D(attrib.vertices.at(i + 0), attrib.vertices.at(i + 1), attrib.vertices.at(i + 2)));
				normals.append(QVector3D(attrib.normals.at(i + 0), attrib.normals.at(i + 1), attrib.normals.at(i + 2)));
				colors.append(QVector3D(attrib.colors.at(i + 0), attrib.colors.at(i + 1), attrib.colors.at(i + 2)));
				aabb.push(vertexs.last());
			}

			_drawable->setVertexAttribArray(0, MakeAsset<Vec3Array>(vertexs));
			_drawable->setVertexAttribArray(1, MakeAsset<Vec3Array>(normals));
			_drawable->setVertexAttribArray(2, MakeAsset<Vec3Array>(colors));

			n = shapes.size();
			QVector<uint> indexs;
			for (int i = 0; i < n; i++)
			{
				tinyobj::shape_t shape = shapes.at(i);
				QString name = QString::fromStdString(shape.name);
				for (tinyobj::index_t index : shape.mesh.indices)
				{
					indexs.append(index.vertex_index);
				}
			}
			_drawable->setIndexAttribute(MakeAsset<UIntArray>(indexs));
		}
		else if (materials.size() == 1)//贴图
		{
			QFileDialog fileDialog(nullptr, "选择贴图", qApp->applicationDirPath(), "*.jpg;*.png");
			fileDialog.setFileMode(QFileDialog::ExistingFiles);
			fileDialog.setModal(true);
			QString diffusetexture, normaltexture;
			if (fileDialog.exec()) 
			{
				if (fileDialog.selectedFiles().size() == 0)
				{
					return{ QVector3D(), float() };
				}
				diffusetexture = fileDialog.selectedFiles().first();
				QFileDialog fileDialog1(nullptr, "选择贴图", qApp->applicationDirPath(), "*.jpg;*.png");
				fileDialog1.setFileMode(QFileDialog::ExistingFiles);
				fileDialog1.setModal(true);
				if (fileDialog1.exec())
				{
					if (fileDialog1.selectedFiles().size() == 0)
					{
						return{ QVector3D(), float() };
					}
					normaltexture = fileDialog1.selectedFiles().first();
				}
				QFileInfo diffuseinfo(diffusetexture);
				QFileInfo normalinfo(normaltexture);
				QVector<QVector3D> vertexs;
				QVector<QVector3D> normals;
				QVector<QVector2D> texcoords;
				vertexs.reserve(n / 3);
				normals.reserve(n / 3);
				texcoords.reserve(n / 3);
				for (int i = 0; i < n; i += 3)
				{
					vertexs.append(QVector3D(attrib.vertices.at(i + 0), attrib.vertices.at(i + 1), attrib.vertices.at(i + 2)));
					aabb.push(vertexs.last());
				}
				n = attrib.normals.size();
				for (int i = 0; i < n; i += 3)
				{
					normals.append(QVector3D(attrib.normals.at(i + 0), attrib.normals.at(i + 1), attrib.normals.at(i + 2)));
				}
				n = attrib.texcoords.size();
				for (int i = 0; i < n; i += 2)
				{
					texcoords.append(QVector2D(attrib.texcoords.at(i + 0), attrib.texcoords.at(i + 1)));
				}
				if (normaltexture.isEmpty())//只有纹理贴图
				{

				}
				else//法线贴图
				{
					shared_ptr<Shader> shaderTest = mShaderManage::GetInstance()->GetShader("ModelTestRendMat");

					_diffuseMap = mTextureManage::GetInstance()->GetTexture(diffuseinfo.completeBaseName(), 3);
					_normalMap = mTextureManage::GetInstance()->GetTexture(normalinfo.completeBaseName(), 3);

					_stateSet->setTexture("diffuseMap", _diffuseMap);
					_stateSet->setTexture("normalMap", _normalMap);

					QVector<QVector3D> aPos;
					QVector<QVector3D> aNormal;
					QVector<QVector2D> aTexcoord;
					QVector<QVector3D> aTangent;
					QVector<QVector3D> aBitangent;
					n = shapes.size();
					for (int i = 0; i < n; i++)
					{
						tinyobj::shape_t shape = shapes.at(i);
						QString name = QString::fromStdString(shape.name);
						int num = shape.mesh.indices.size() / 3;
						for (int j = 0; j < num; j++)
						{
							QVector3D pos1 = vertexs.at(shape.mesh.indices.at(j * 3 + 0).vertex_index);
							QVector3D pos2 = vertexs.at(shape.mesh.indices.at(j * 3 + 1).vertex_index);
							QVector3D pos3 = vertexs.at(shape.mesh.indices.at(j * 3 + 2).vertex_index);

							// texture coordinates
							QVector2D uv1 = texcoords.at(shape.mesh.indices.at(j * 3 + 0).texcoord_index);
							QVector2D uv2 = texcoords.at(shape.mesh.indices.at(j * 3 + 1).texcoord_index);
							QVector2D uv3 = texcoords.at(shape.mesh.indices.at(j * 3 + 2).texcoord_index);

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
							
							aPos.append(pos1); aPos.append(pos2); aPos.append(pos3);
							aNormal.append(normals.at(shape.mesh.indices.at(j * 3 + 0).normal_index)); aNormal.append(normals.at(shape.mesh.indices.at(j * 3 + 1).normal_index)); aNormal.append(normals.at(shape.mesh.indices.at(j * 3 + 2).normal_index));
							aTexcoord.append(uv1); aTexcoord.append(uv2); aTexcoord.append(uv3);
							aTangent.append(QVector<QVector3D>(3, tangent1));
							aBitangent.append(QVector<QVector3D>(3, bitangent1));
						}
					}

					_drawable->setVertexAttribArray(0, MakeAsset<Vec3Array>(aPos));
					_drawable->setVertexAttribArray(1, MakeAsset<Vec3Array>(aNormal));
					_drawable->setVertexAttribArray(2, MakeAsset<Vec2Array>(aTexcoord));
					_drawable->setVertexAttribArray(3, MakeAsset<Vec3Array>(aTangent));
					_drawable->setVertexAttribArray(4, MakeAsset<Vec3Array>(aBitangent));
				}
			}
			
		}
		else
		{
			shared_ptr<Shader> shaderTest = mShaderManage::GetInstance()->GetShader("ModelTestRendMat");
			_stateSet->setShader(shaderTest);//设置着色器

			QVector<QVector3D> vertexs;
			QVector<QVector3D> normals;
			QVector<QVector2D> texcoords;
			vertexs.reserve(n / 3);
			normals.reserve(n / 3);
			texcoords.reserve(n / 3);
			for (int i = 0; i < n; i += 3)
			{
				vertexs.append(QVector3D(attrib.vertices.at(i + 0), attrib.vertices.at(i + 1), attrib.vertices.at(i + 2)));
				aabb.push(vertexs.last());
			}
			n = attrib.normals.size();
			for (int i = 0; i < n; i += 3)
			{
				normals.append(QVector3D(attrib.normals.at(i + 0), attrib.normals.at(i + 1), attrib.normals.at(i + 2)));
			}
			n = attrib.texcoords.size();
			for (int i = 0; i < n; i += 2)
			{
				texcoords.append(QVector2D(attrib.texcoords.at(i + 0), attrib.texcoords.at(i + 1)));
			}

			QVector<Material> allmaterials;
			n = materials.size();
			for (int i = 0; i < n; i++)
			{
				Material mat;
				for (int j = 0; j < 3; j++)
				{
					mat.ambient[j] = materials.at(i).ambient[j];
				}
				for (int j = 0; j < 3; j++)
				{
					mat.diffuse[j] = materials.at(i).diffuse[j];
				}
				for (int j = 0; j < 3; j++)
				{
					mat.specular[j] = materials.at(i).specular[j];
				}
				allmaterials.append(mat);
				//QMatrix3x3 matrix;
				//float *data = matrix.data();
				//for (int j = 0; j < 3; i++)
				//{
				//	data[j + 0] = materials.at(i).ambient[j];
				//}
				//for (int j = 0; j < 3; i++)
				//{
				//	data[j + 3] = materials.at(i).diffuse[j];
				//}
				//for (int j = 0; j < 3; i++)
				//{
				//	data[j + 6] = materials.at(i).specular[j];
				//}
				//allmaterials.append(matrix);
			}

			QVector<QVector3D> aPos;
			QVector<QVector3D> aNormal;
			QVector<QVector2D> aTexcoord;
			QVector<float> aMaterial;
			//_drawable->setVertexAttribArray(0, MakeAsset<Vec3Array>(aPos));
			//_drawable->setVertexAttribArray(1, MakeAsset<Vec3Array>(aNormal));
			//_drawable->setVertexAttribArray(2, MakeAsset<Vec2Array>(aTexcoord));
			//_drawable->setVertexAttribArray(3, MakeAsset<FloatArray>(aMaterial));
			n = shapes.size();
			for (int i = 0; i < n; i++)
			{
				tinyobj::shape_t shape = shapes.at(i);
				QString name = QString::fromStdString(shape.name);
				int num = shape.mesh.indices.size()/3;
				for (int j = 0; j < num; j++)
				{
					for (int k = 0; k < 3; ++k)
					{
						aPos.append(vertexs.at(shape.mesh.indices.at(j * 3 + k).vertex_index));
						aNormal.append(normals.at(shape.mesh.indices.at(j * 3 + k).normal_index));
						//aTexcoord.append(texcoords.at(shape.mesh.indices.at(j * 3 + k).texcoord_index));
					}
					aMaterial.append(shape.mesh.material_ids.at(j)); aMaterial.append(aMaterial.last()); aMaterial.append(aMaterial.last());
				}
			}

			_drawable->setVertexAttribArray(0, MakeAsset<Vec3Array>(aPos));
			_drawable->setVertexAttribArray(1, MakeAsset<Vec3Array>(aNormal));
			//_drawable->setVertexAttribArray(2, MakeAsset<Vec2Array>(aTexcoord));
			_drawable->setVertexAttribArray(2, MakeAsset<FloatArray>(aMaterial));

			for (int i = 0; i < allmaterials.size(); ++i)
			{
				_stateSet->setUniform(MakeAsset<Uniform>(QString("materials[%1].ambient").arg(i).toStdString(), allmaterials.at(i).ambient));
				_stateSet->setUniform(MakeAsset<Uniform>(QString("materials[%1].diffuse").arg(i).toStdString(), allmaterials.at(i).diffuse));
				_stateSet->setUniform(MakeAsset<Uniform>(QString("materials[%1].specular").arg(i).toStdString(), allmaterials.at(i).specular));
			}

		}

		QVector3D center = (aabb.maxEdge + aabb.minEdge) / 2.0;
		float radius = sqrt(pow(aabb.maxEdge.x() - aabb.minEdge.x(), 2) + pow(aabb.maxEdge.y() - aabb.minEdge.y(), 2) + pow(aabb.maxEdge.z() - aabb.minEdge.z(), 2));

		return { center ,radius };
	
	}
	void mModelTestRender::updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView)
	{
		_stateSet->getUniform("projection")->SetData(modelView->_projection);
		_stateSet->getUniform("view")->SetData(modelView->_view);
		_stateSet->getUniform("model")->SetData(modelView->_model);
		_stateSet->getUniform("light.position")->SetData(2 * modelView->_Eye - modelView->_Center);
		_stateSet->getUniform("viewPos")->SetData(modelView->_Eye);
	}
}