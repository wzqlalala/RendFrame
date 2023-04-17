#include "mCommonView.h"

namespace MViewBasic
{
	mCommonView::mCommonView()
	{
		InitialCommonView();
	}

	mCommonView::~mCommonView()
	{
	}
	void mCommonView::InitialCommonView()
	{
		InitialOrtho();
		InitialCamera();
		SetPVMValue();
	}
	void mCommonView::InitialOrtho()
	{
		_Left_ini = -5;
		_Right_ini = 5;
		_Bottom_ini = -5;
		_Top_ini = 5;
		_NearPlane = 0.1;
		_FarPlane = 99;
	}
	void mCommonView::InitialCamera()
	{
		_Eye = QVector3D(0, 0, 5);
		_Center = QVector3D(0, 0, 0);
		_Up = QVector3D(0, 1, 0);

	}
	void mCommonView::SetPerspective(Perspective type)
	{
		if (type == Front)//设置视角为X轴正向
		{
			_Eye = QVector3D(0.0, 0.0, 5);
			_Up = QVector3D(0.0, 1.0, 0.0);

		}
		else if (type == Rear)
		{
			_Eye = QVector3D(0.0, 0.0, -5.0);
			_Up = QVector3D(0.0, 1.0, 0.0);
		}
		else if (type == Top)
		{
			_Eye = QVector3D(0.0, 5.0, 0.0);
			_Up = QVector3D(0.0, 0.0, -1.0);
		}
		else if (type == Bottom)
		{
			_Eye = QVector3D(0.0, -5.0, 0.0);
			_Up = QVector3D(0.0, 0.0, 1.0);
		}
		else if (type == Left)
		{
			_Eye = QVector3D(-5, 0.0, 0.0);
			_Up = QVector3D(0.0, 1.0, 0.0);
		}
		else if (type == Right)
		{
			_Eye = QVector3D(5, 0.0, 0.0);
			_Up = QVector3D(0.0, 1.0, 0.0);
		}
		else if (type == ISO1)
		{
			_Eye = QVector3D(0, 0, 5);
			_Up = QVector3D(0.0, 1.0, 0.0);
			QMatrix4x4 _MatrixRotate;
			_MatrixRotate.setToIdentity();
			_MatrixRotate.rotate(30, QVector3D(1, 0, 0));
			_MatrixRotate.rotate(-45, QVector3D(0, 1, 0));
			QVector4D NewEye4D = QVector4D(_Eye, 1.0)*_MatrixRotate;
			QVector4D NewUp4D = QVector4D(_Up, 1.0)*_MatrixRotate;
			//正交投影视角在原点处的旋转
			_Eye = QVector3D(NewEye4D.x(), NewEye4D.y(), NewEye4D.z());
			_Up = QVector3D(NewUp4D.x(), NewUp4D.y(), NewUp4D.z());
		}
		else if (type == ISO2)
		{
			_Eye = QVector3D(0, 0, 5);
			_Up = QVector3D(0.0, 1.0, 0.0);
			QMatrix4x4 _MatrixRotate;
			_MatrixRotate.setToIdentity();
			_MatrixRotate.rotate(30, QVector3D(1, 0, 0));
			_MatrixRotate.rotate(45, QVector3D(0, 1, 0));
			QVector4D NewEye4D = QVector4D(_Eye, 1.0)*_MatrixRotate;
			QVector4D NewUp4D = QVector4D(_Up, 1.0)*_MatrixRotate;
			//正交投影视角在原点处的旋转
			_Eye = QVector3D(NewEye4D.x(), NewEye4D.y(), NewEye4D.z());
			_Up = QVector3D(NewUp4D.x(), NewUp4D.y(), NewUp4D.z());
		}
		else if (type == ISO3)
		{
			_Eye = QVector3D(5, 0, 0);
			_Up = QVector3D(0.0, 0.0, 1.0);
			QMatrix4x4 _MatrixRotate;
			_MatrixRotate.setToIdentity();
			_MatrixRotate.rotate(30, QVector3D(0, 1, 0));
			_MatrixRotate.rotate(45, QVector3D(0, 0, 1));
			QVector4D NewEye4D = QVector4D(_Eye, 1.0)*_MatrixRotate;
			QVector4D NewUp4D = QVector4D(_Up, 1.0)*_MatrixRotate;
			//正交投影视角在原点处的旋转
			_Eye = QVector3D(NewEye4D.x(), NewEye4D.y(), NewEye4D.z());
			_Up = QVector3D(NewUp4D.x(), NewUp4D.y(), NewUp4D.z());
		}
		else if (type == ISO4)
		{
			_Eye = QVector3D(-5, 0, 0);
			_Up = QVector3D(0.0, 1.0, 0.0);
			QMatrix4x4 _MatrixRotate;
			_MatrixRotate.setToIdentity();
			_MatrixRotate.rotate(-30, QVector3D(0, 0, 1));
			_MatrixRotate.rotate(45, QVector3D(0, 1, 0));
			QVector4D NewEye4D = QVector4D(_Eye, 1.0)*_MatrixRotate;
			QVector4D NewUp4D = QVector4D(_Up, 1.0)*_MatrixRotate;
			//正交投影视角在原点处的旋转
			_Eye = QVector3D(NewEye4D.x(), NewEye4D.y(), NewEye4D.z());
			_Up = QVector3D(NewUp4D.x(), NewUp4D.y(), NewUp4D.z());
		}
		SetPVMValue();
	}
	void mCommonView::SetOrthoByRatio(float scr_width, float scr_height)
	{
		float ratio = scr_width / scr_height;
		if (scr_width <= scr_height)
		{
			_Left = _Left_ini;
			_Right = _Right_ini;
			_Bottom = _Bottom_ini / ratio;
			_Top = _Top_ini / ratio;
		}
		else
		{
			_Left = _Left_ini * ratio;
			_Right = _Right_ini * ratio;
			_Bottom = _Bottom_ini;
			_Top = _Top_ini;
		}
		SetPVMValue();
	}
	void mCommonView::Rotate(float xoffset, float yoffset, RotateType rotateType , float sensitivity )
	{
		QVector3D _AuxY = QVector3D(_Up.x(), _Up.y(), _Up.z()).normalized(); //上向量
		QVector3D _AuxZ = QVector3D(_Eye.x() , _Eye.y(), _Eye.z()).normalized();
		QVector3D _AuxX = _AuxX.crossProduct(_AuxY, _AuxZ); //Y轴和Z轴叉乘结果为X轴
		if (rotateType == Rotate_XY)
		{
			QVector3D _MouseTrace = _AuxX * xoffset + _AuxY * yoffset;//鼠标轨迹向量
			//获取旋转轴在世界坐标系中的表示
			QVector3D  _AxisRotate = _AxisRotate.crossProduct(_AuxZ, _MouseTrace).normalized();    //旋转轴
			//创建旋转矩阵
			QMatrix4x4 _MatrixRotate;
			_MatrixRotate.setToIdentity();
			_MatrixRotate.rotate(-_MouseTrace.length()*sensitivity, _AxisRotate); //调整旋转方向
			//旋转
			QVector4D NewEye4D = QVector4D(_Eye, 1.0)*_MatrixRotate;
			QVector4D NewUp4D = QVector4D(_Up, 1.0)*_MatrixRotate;
			//正交投影视角在原点处的旋转
			_Eye = QVector3D(NewEye4D.x(), NewEye4D.y(), NewEye4D.z());
			_Up = QVector3D(NewUp4D.x(), NewUp4D.y(), NewUp4D.z());
		}
		else if (rotateType == Rotate_Z)
		{
			QVector3D _MouseTrace = _AuxX * xoffset;//鼠标轨迹向量
				//获取旋转轴在世界坐标系中的表示
			QVector3D  _AxisRotate = _AuxZ;    //旋转轴
			//创建旋转矩阵
			QMatrix4x4 _MatrixRotate;
			_MatrixRotate.setToIdentity();
			if (xoffset <= 0)
			{
				_MatrixRotate.rotate(-_MouseTrace.length()*sensitivity, _AxisRotate); //调整旋转方向
			}
			else
			{
				_MatrixRotate.rotate(_MouseTrace.length()*sensitivity, _AxisRotate); //调整旋转方向
			}
			//旋转
			QVector4D NewUp4D = QVector4D(_Up, 1.0)*_MatrixRotate;

			_Up = QVector3D(NewUp4D.x(), NewUp4D.y(), NewUp4D.z());

		}
		SetPVMValue();
	}
	void mCommonView::Rotate_ByBotton(float angle)
	{
		QVector3D _AuxZ = QVector3D(_Eye.x(), _Eye.y(), _Eye.z()).normalized();
		//创建旋转矩阵
		QMatrix4x4 _MatrixRotate;
		_MatrixRotate.setToIdentity();
		_MatrixRotate.rotate(angle, _AuxZ);
		//旋转
		QVector4D NewUp4D = QVector4D(_Up, 1.0)*_MatrixRotate;
		_Up = QVector3D(NewUp4D.x(), NewUp4D.y(), NewUp4D.z());
		SetPVMValue();
	}

	void mCommonView::SaveCurrentView()
	{
		_Eye_Saved = _Eye;	
		_Center_Saved = _Center;
		_Up_Saved = _Up;
		_projection_Saved = _projection; _view_Saved = _view; _model_Saved = _model;
	}
	void mCommonView::CallSavedView()
	{
		_Eye = _Eye_Saved;
		_Center = _Center_Saved;
		_Up = _Up_Saved;
		_projection = _projection_Saved; _view = _view_Saved; _model = _model_Saved;
	}

	void mCommonView::SetPVMValue()
	{
		_projection.setToIdentity();
		_model.setToIdentity();
		_view.setToIdentity();
		_projection.ortho(_Left, _Right, _Bottom, _Top, _NearPlane, _FarPlane);
		_view.lookAt(_Eye, _Center, _Up);
	}

}
