#include "mViewBase.h"
#include "mViewToolClass.h"
#include "mMeshViewEnum.h"


namespace MViewBasic
{
	mViewBase::mViewBase()
	{
		InitialModelView();
	}
	mViewBase::~mViewBase()
	{

	}
	void mViewBase::InitialModelView()
	{
		InitialOrtho();
		InitialCamera();
	}
	void mViewBase::InitialOrtho()
	{
		//正交投影参数
		_Left_ini = -1;
		_Right_ini = 1;
		_Bottom_ini = -1;
		_Top_ini = 1;
		_NearPlane = 0.1;
		_FarPlane = 99;
		_Left_res = _Left_ini;
		_Right_res = _Right_ini;
		_Bottom_res = _Bottom_ini;
		_Top_res = _Top_ini;


	}
	void mViewBase::InitialCamera()
	{
		//视角参数
		_Eye = QVector3D(0, 0, 1);
		_Eye_0 = QVector3D(0, 0, 1);
		_Center = QVector3D(0, 0, 0);
		_Up = QVector3D(0, 1, 0);
	}
	void mViewBase::SetOrthoByRatio(float scr_width, float scr_height)
	{
		SCR_WIDTH = scr_width;
		SCR_HEIGHT = scr_height;

		SetPVMValue();
	}
	void mViewBase::ReturnOrthoByRatio()
	{
	}
	void mViewBase::SetPVMValue()
	{
		_projection.setToIdentity();
		_model.setToIdentity();
		_view.setToIdentity();
		_projection.ortho(_Left, _Right, _Bottom, _Top, _NearPlane, _FarPlane);
		_view.lookAt(_Eye, _Center, _Up);
	}
	void mViewBase::ZoomAtMouse_Bywheel(int posX, int posY, int yoffset, ScalePerspectice scalePerspectice, float sensitivity)
	{
	}
	void mViewBase::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
	{
	}
	void mViewBase::ResetOrthoAndCamera(QVector3D center, float maxRadius)
	{

	}
}
