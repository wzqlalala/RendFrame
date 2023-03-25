#include "mPostRigidWallData1.h"

//Qt
#include <QMatrix>
#include <QMatrix4x4>
#include <QVector3D>
#include <queue>
#include<math.h>

namespace MDataPost
{
	mPostRigidWallDataBase::mPostRigidWallDataBase(int ID, PostRigidWallType rigidWallType, double *normalStartEndPos)
	{
		_rigidWallID = ID;

		_rigidWallType = rigidWallType;

		_normalStartPos = QVector3D(normalStartEndPos[0], normalStartEndPos[1], normalStartEndPos[2]);

		_normalEndPos = QVector3D(normalStartEndPos[3], normalStartEndPos[4], normalStartEndPos[5]);

		_modelSize = ModelSize();

	}

	int mPostRigidWallDataBase::getRigidWallID()
	{
		return _rigidWallID;
	}

	PostRigidWallType mPostRigidWallDataBase::getRigidWallType()
	{
		return _rigidWallType;
	}

	QVector3D mPostRigidWallDataBase::getRigidWallStartPos()
	{
		return _normalStartPos;
	}

	QVector3D mPostRigidWallDataBase::getRigidWallEndPos()
	{
		return _normalEndPos;
	}

	void mPostRigidWallDataBase::setFaceVertex(QVector<QVector3D> vertexs)
	{
		_faceVertexs = vertexs;
		_modelSize = ModelSize();
		for (QVector3D vertex : _faceVertexs)
		{
			_modelSize.xmax = max(_modelSize.xmax, vertex.x());
			_modelSize.ymax = max(_modelSize.ymax, vertex.y());
			_modelSize.zmax = max(_modelSize.zmax, vertex.z());
			_modelSize.xmin = min(_modelSize.xmin, vertex.x());
			_modelSize.ymin = min(_modelSize.ymin, vertex.y());
			_modelSize.zmin = min(_modelSize.zmin, vertex.z());
		}
	}

	void mPostRigidWallDataBase::caculateRigidWallSize()
	{
		for (QVector3D vertex : _lineVertexs)
		{
			_modelSize.xmax = max(_modelSize.xmax, vertex.x());
			_modelSize.ymax = max(_modelSize.ymax, vertex.y());
			_modelSize.zmax = max(_modelSize.zmax, vertex.z());
			_modelSize.xmin = min(_modelSize.xmin, vertex.x());
			_modelSize.ymin = min(_modelSize.ymin, vertex.y());
			_modelSize.zmin = min(_modelSize.zmin, vertex.z());
		}
	}


	mPostInfinitelyLargePlaneRigidWall::mPostInfinitelyLargePlaneRigidWall(int ID, double *normalStartEndPos):mPostRigidWallDataBase(ID,PostRigidWallType::InfinitelyLargePlaneRigidWall, normalStartEndPos)
	{


	}

	QVector<QVector3D> mPostInfinitelyLargePlaneRigidWall::getFaceVertex()
	{
		return _faceVertexs;
	}

	QVector<QVector3D> mPostInfinitelyLargePlaneRigidWall::getLineVertex()
	{
		return QVector<QVector3D>();
	}

	mPostLimitedLargePlaneRigidWall::mPostLimitedLargePlaneRigidWall(int ID, double *normalStartEndPos) :mPostRigidWallDataBase(ID, PostRigidWallType::LimitedLargePlanesRigidWall, normalStartEndPos)
	{

	}

	QVector<QVector3D> mPostLimitedLargePlaneRigidWall::getFaceVertex()
	{
		if (_faceVertexs.empty())
		{
			calculateVertex();
		}
		return _faceVertexs;
	}

	QVector<QVector3D> mPostLimitedLargePlaneRigidWall::getLineVertex()
	{
		if (_lineVertexs.empty())
		{
			calculateVertex();
			caculateRigidWallSize();
		}
		return _lineVertexs;
	}

	ModelSize mPostLimitedLargePlaneRigidWall::getRigidWallSize()
	{
		if (_lineVertexs.empty())
		{
			calculateVertex();
			caculateRigidWallSize();
		}
		return _modelSize;
	}

	void mPostLimitedLargePlaneRigidWall::calculateVertex()
	{
		QVector3D normal = _normalEndPos - _normalStartPos;
		QVector3D lineVec = QVector3D(_lineEndVector[0], _lineEndVector[1], _lineEndVector[2]) - _normalStartPos;
		QVector3D lineVec1 = QVector3D::crossProduct(normal, lineVec);
		_faceVertexs.resize(6);
		_lineVertexs.resize(8);

		QVector<QVector3D> vertexs(4);

		vertexs[0] = _normalStartPos;
		vertexs[1] = _normalStartPos + lineVec * _L;
		vertexs[2] = vertexs[1] + lineVec1 * _M;
		vertexs[3] = _normalStartPos + lineVec1 * _M;

		_lineVertexs[0] = vertexs[0];
		_lineVertexs[1] = vertexs[1];
		_lineVertexs[2] = vertexs[1];
		_lineVertexs[3] = vertexs[2];
		_lineVertexs[4] = vertexs[2];
		_lineVertexs[5] = vertexs[3];
		_lineVertexs[6] = vertexs[3];
		_lineVertexs[7] = vertexs[0];

		_faceVertexs[0] = vertexs[0];
		_faceVertexs[1] = vertexs[1];
		_faceVertexs[2] = vertexs[2];
		_faceVertexs[3] = vertexs[2];
		_faceVertexs[4] = vertexs[3];
		_faceVertexs[5] = vertexs[0];

	}

	mPostRectangularRigidWall::mPostRectangularRigidWall(int ID, double *normalStartEndPos) :mPostRigidWallDataBase(ID, PostRigidWallType::RectangularRigidWall, normalStartEndPos)
	{


	}

	QVector<QVector3D> mPostRectangularRigidWall::getFaceVertex()
	{
		if (_faceVertexs.empty())
		{
			calculateVertex();
		}
		return _faceVertexs;
	}

	QVector<QVector3D> mPostRectangularRigidWall::getLineVertex()
	{
		if (_lineVertexs.empty())
		{
			calculateVertex();
			caculateRigidWallSize();
		}
		return _lineVertexs;
	}

	ModelSize mPostRectangularRigidWall::getRigidWallSize()
	{
		if (_lineVertexs.empty())
		{
			calculateVertex();
			caculateRigidWallSize();
		}
		return _modelSize;
	}

	void mPostRectangularRigidWall::calculateVertex()
	{
		QVector3D normal = _normalEndPos - _normalStartPos;
		QVector3D lineVec = QVector3D(_lineEndVector[0], _lineEndVector[1], _lineEndVector[2]) - _normalStartPos;
		QVector3D lineVec1 = QVector3D::crossProduct(normal, lineVec);
		
		QVector<QVector3D> vertexs(8);

		vertexs[0] = _normalStartPos;
		vertexs[1] = _normalStartPos + lineVec * _L;
		vertexs[2] = vertexs[1] + lineVec1 * _M;
		vertexs[3] = _normalStartPos + lineVec1 * _M;
		vertexs[4] = vertexs[0] - normal * _P;
		vertexs[5] = vertexs[1] - normal * _P;
		vertexs[6] = vertexs[2] - normal * _P;
		vertexs[7] = vertexs[3] - normal * _P;

		_lineVertexs.resize(24);
		_faceVertexs.resize(36);

		_lineVertexs[0] = vertexs[0];
		_lineVertexs[1] = vertexs[1];
		_lineVertexs[2] = vertexs[1];
		_lineVertexs[3] = vertexs[2];
		_lineVertexs[4] = vertexs[2];
		_lineVertexs[5] = vertexs[3];
		_lineVertexs[6] = vertexs[3];
		_lineVertexs[7] = vertexs[0];

		_lineVertexs[8] = vertexs[0];
		_lineVertexs[9] = vertexs[4];
		_lineVertexs[10] = vertexs[1];
		_lineVertexs[11] = vertexs[5];
		_lineVertexs[12] = vertexs[2];
		_lineVertexs[13] = vertexs[6];
		_lineVertexs[14] = vertexs[3];
		_lineVertexs[15] = vertexs[7];

		_lineVertexs[16] = vertexs[4];
		_lineVertexs[17] = vertexs[5];
		_lineVertexs[18] = vertexs[5];
		_lineVertexs[19] = vertexs[6];
		_lineVertexs[20] = vertexs[6];
		_lineVertexs[21] = vertexs[7];
		_lineVertexs[22] = vertexs[7];
		_lineVertexs[23] = vertexs[4];

		_faceVertexs[0] = vertexs[0];
		_faceVertexs[1] = vertexs[1];
		_faceVertexs[2] = vertexs[2];
		_faceVertexs[3] = vertexs[2];
		_faceVertexs[4] = vertexs[3];
		_faceVertexs[5] = vertexs[0];

		_faceVertexs[6] = vertexs[1];
		_faceVertexs[7] = vertexs[0];
		_faceVertexs[8] = vertexs[4];
		_faceVertexs[9] = vertexs[4];
		_faceVertexs[10] = vertexs[5];
		_faceVertexs[11] = vertexs[1];

		_faceVertexs[12] = vertexs[1];
		_faceVertexs[13] = vertexs[5];
		_faceVertexs[14] = vertexs[6];
		_faceVertexs[15] = vertexs[6];
		_faceVertexs[16] = vertexs[2];
		_faceVertexs[17] = vertexs[1];

		_faceVertexs[18] = vertexs[2];
		_faceVertexs[19] = vertexs[6];
		_faceVertexs[20] = vertexs[7];
		_faceVertexs[21] = vertexs[7];
		_faceVertexs[22] = vertexs[3];
		_faceVertexs[23] = vertexs[2];

		_faceVertexs[24] = vertexs[0];
		_faceVertexs[25] = vertexs[3];
		_faceVertexs[26] = vertexs[7];
		_faceVertexs[27] = vertexs[7];
		_faceVertexs[28] = vertexs[4];
		_faceVertexs[29] = vertexs[0];

		_faceVertexs[30] = vertexs[4];
		_faceVertexs[31] = vertexs[7];
		_faceVertexs[32] = vertexs[6];
		_faceVertexs[33] = vertexs[6];
		_faceVertexs[34] = vertexs[5];
		_faceVertexs[35] = vertexs[4];

	}

	mPostCylindricalRigidWall::mPostCylindricalRigidWall(int ID, double *normalStartEndPos) :mPostRigidWallDataBase(ID, PostRigidWallType::CylindricalRigidWall, normalStartEndPos)
	{


	}

	QVector<QVector3D> mPostCylindricalRigidWall::getFaceVertex()
	{
		if (_faceVertexs.empty())
		{
			calculateVertex();
		}
		return _faceVertexs;
	}

	QVector<QVector3D> mPostCylindricalRigidWall::getLineVertex()
	{
		if (_lineVertexs.empty())
		{
			calculateVertex();
			caculateRigidWallSize();
		}
		return _lineVertexs;
	}

	ModelSize mPostCylindricalRigidWall::getRigidWallSize()
	{
		if (_lineVertexs.empty())
		{
			calculateVertex();
			caculateRigidWallSize();
		}
		return _modelSize;
	}

	void mPostCylindricalRigidWall::calculateVertex()
	{
		QVector3D normal = _normalEndPos - _normalStartPos;
		//平面方程为Ax+By+Cz+D=0, 求半径矢量
		srand(0);
		normal.normalized();
		//得到一个随机向量
		QVector3D randVec = { 0,0,1 };
		while (normal == randVec || normal == -randVec) //重合
		{
			randVec.setX(rand());
			randVec.setY(rand());
			randVec.setZ(rand());
			randVec.normalized();
		}
		//两个向量叉乘求出半径矢量
		QVector3D res = QVector3D::crossProduct(normal, randVec).normalized();
		float angle = 10;
		float currentAngle = 0;
		QVector<QVector3D> face1, face2, face3;
		while (currentAngle < 360)
		{
			currentAngle += angle;

			QMatrix4x4 matrixRotate;
			matrixRotate.setToIdentity();
			matrixRotate.rotate(currentAngle, normal);
			QVector4D tmp = QVector4D(res, 1.0) * matrixRotate;
			QVector3D next_rVec = QVector3D(tmp.x(), tmp.y(), tmp.z());
			next_rVec.normalized();

			//求出三角形坐标
			QVector3D pos1 = _normalStartPos;
			QVector3D pos2;
			if (face1.empty())
			{
				pos2 = _normalStartPos + res * _radius;
			}
			else
			{
				pos2 = face1.back();
			}
			QVector3D pos3 = _normalStartPos + next_rVec * _radius;
			//顶面
			face1.push_back(pos1);
			face1.push_back(pos2);
			face1.push_back(pos3);
			_lineVertexs.push_back(pos2);
			_lineVertexs.push_back(pos3);
			//底面
			face2.push_back(pos1 - normal * _length);
			face2.push_back(pos2 - normal * _length);
			face2.push_back(pos3 - normal * _length);
			_lineVertexs.push_back(pos2 - normal * _length);
			_lineVertexs.push_back(pos3 - normal * _length);

			//侧面线
			if (currentAngle - angle == 0 || currentAngle - angle == 180)
			{
				_lineVertexs.push_back(pos2);
				_lineVertexs.push_back(pos2 - normal * _length);
			}

			//侧面
			int id = face1.size() - 1;
			face3.push_back(face1[id - 1]);
			face3.push_back(face2[id]);
			face3.push_back(face2[id - 1]);
			face3.push_back(face1[id - 1]);
			face3.push_back(face1[id]);
			face3.push_back(face2[id]);

		}

		_faceVertexs = face1 + face2 + face3;
	}

	mPostSphericalRigidWall::mPostSphericalRigidWall(int ID, double *normalStartEndPos) : mPostRigidWallDataBase(ID, PostRigidWallType::SphericalRigidWall, normalStartEndPos)
	{


	}
	QVector<QVector3D> mPostSphericalRigidWall::getFaceVertex()
	{
		if (_faceVertexs.empty())
		{
			calculateVertex();
			_modelSize.xmax = _normalStartPos.x() + _radius;
			_modelSize.xmin = _normalStartPos.x() - _radius;
			_modelSize.ymax = _normalStartPos.y() + _radius;
			_modelSize.ymin = _normalStartPos.y() - _radius;
			_modelSize.zmax = _normalStartPos.z() + _radius;
			_modelSize.zmin = _normalStartPos.z() - _radius;
		}
		return _faceVertexs;
	}
	QVector<QVector3D> mPostSphericalRigidWall::getLineVertex()
	{
		//if (_lineVertexs.empty())
		//{
		//	calculateVertex();
		//	_modelSize.xmax = _normalStartPos.x() + _radius;
		//	_modelSize.xmin = _normalStartPos.x() - _radius;
		//	_modelSize.ymax = _normalStartPos.y() + _radius;
		//	_modelSize.ymin = _normalStartPos.y() - _radius;
		//	_modelSize.zmax = _normalStartPos.z() + _radius;
		//	_modelSize.zmin = _normalStartPos.z() - _radius;
		//}
		return _lineVertexs;
	}

	ModelSize mPostSphericalRigidWall::getRigidWallSize()
	{
		if (_lineVertexs.empty())
		{
			calculateVertex();
			caculateRigidWallSize();
		}
		return _modelSize;
	}

	void mPostSphericalRigidWall::calculateVertex()
	{
		struct Triangle
		{
			Triangle(QVector3D A, QVector3D B, QVector3D C)
			{
				a = A; b = B; c = C;
			}
			void transverseX()
			{
				a[0] = -a.x();
				b[0] = -b.x();
				c[0] = -c.x();
			};
			void transverseY()
			{
				a[1] = -a.y();
				b[1] = -b.y();
				c[1] = -c.y();
			};
			void transverseZ()
			{
				a[2] = -a.z();
				b[2] = -b.z();
				c[2] = -c.z();
			};
			QVector3D a, b, c;
		};
		float resolution = _radius / 10;
		resolution *= resolution;
		std::queue<Triangle> triangles;
		triangles.push(Triangle(QVector3D(_radius, 0, 0), QVector3D(0, _radius, 0), QVector3D(0, 0, _radius)));

		//原始坐标的第一象限
		while (!triangles.empty())
		{
			auto& t = triangles.front();

			// 当三角形各边长度都不大于resolution时就不再进一步细分
			if (distSquare(t.a, t.b) > resolution ||
				distSquare(t.b, t.c) > resolution ||
				distSquare(t.c, t.a) > resolution) {
				auto d = midArcPoint(t.a, t.b), e = midArcPoint(t.b, t.c),
					f = midArcPoint(t.c, t.a);
				triangles.push({ t.a, f, d });
				triangles.push({ t.b, d, e });
				triangles.push({ t.c, e, f });
				triangles.push({ d, e, f });
			}
			else
			{
				// 第一象限
				_faceVertexs.append(t.a);
				_faceVertexs.append(t.b);
				_faceVertexs.append(t.c);

				t.transverseX();
				// 第二象限
				_faceVertexs.append(t.a);
				_faceVertexs.append(t.b);
				_faceVertexs.append(t.c);

				t.transverseY();
				// 第三象限
				_faceVertexs.append(t.a);
				_faceVertexs.append(t.b);
				_faceVertexs.append(t.c);

				t.transverseX();
				// 第四象限
				_faceVertexs.append(t.a);
				_faceVertexs.append(t.b);
				_faceVertexs.append(t.c);

				t.transverseZ();
				// 第八象限
				_faceVertexs.append(t.a);
				_faceVertexs.append(t.b);
				_faceVertexs.append(t.c);

				t.transverseY();
				// 第五象限
				_faceVertexs.append(t.a);
				_faceVertexs.append(t.b);
				_faceVertexs.append(t.c);

				t.transverseX();
				// 第六象限
				_faceVertexs.append(t.a);
				_faceVertexs.append(t.b);
				_faceVertexs.append(t.c);

				t.transverseY();
				// 第七象限
				_faceVertexs.append(t.a);
				_faceVertexs.append(t.b);
				_faceVertexs.append(t.c);


			}
			triangles.pop();
		}

		for (int i = 0;i< _faceVertexs.size();++i)
		{
			_faceVertexs[i].setX(_normalStartPos.x() + _faceVertexs[i].x());
			_faceVertexs[i].setY(_normalStartPos.y() + _faceVertexs[i].y());
			_faceVertexs[i].setZ(_normalStartPos.z() + _faceVertexs[i].z());
		}
	}

	float mPostSphericalRigidWall::distSquare(QVector3D a, QVector3D b)
	{
		return (a - b).lengthSquared();
	}

	QVector3D mPostSphericalRigidWall::midArcPoint(QVector3D a, QVector3D b)
	{
		QVector3D c{ a.x() + b.x(), a.y() + b.y(), a.z() + b.z() };
		float mod = sqrt(c.x()* c.x() + c.y() * c.y() + c.z() * c.z());
		c.setX(c.x()/ mod), c.setY(c.y()/ mod), c.setZ(c.z()/mod);
		return c * _radius;
	}
}
