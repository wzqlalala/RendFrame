#pragma once
#include "rendbasic_global.h"

#include <QObject>
#include"qvector3d.h"
#include"qmatrix4x4.h"
#include"mMeshViewEnum.h"

namespace MViewBasic
{
	class RENDBASIC_EXPORT mCommonView : public QObject
	{
		Q_OBJECT

	public:
		mCommonView();
		~mCommonView();
		

	
		//�����ӽ�
		QVector3D  _Eye, _Center, _Up;											//ȫ�����������
	
		//��������ͶӰ
		float _Left, _Right, _Bottom, _Top, _NearPlane, _FarPlane;				//����ͶӰȫ�ֱ���
		float _Left_ini, _Right_ini, _Bottom_ini, _Top_ini;						//����ͶӰ��ʼ��������

		//PVM����
		QMatrix4x4 _projection, _view, _model;

		//������ӽǲ���
		QVector3D _Eye_Saved, _Center_Saved, _Up_Saved;
		QMatrix4x4 _projection_Saved, _view_Saved, _model_Saved;

		void InitialCommonView();												//��ʼ��ͨ���ӽ�
		void InitialOrtho();													//��ʼ������ͶӰ(��ת����)
		void InitialCamera();													//��ʼ�����
		void SetOrthoByRatio(float scr_width, float scr_height);				//ͨ����Ļ�������������ͶӰ
		void Rotate(float xoffset, float yoffset, RotateType rotateType = Rotate_XY, float sensitivity = 0.25);//��ת
		void Rotate_ByBotton(float angle);//���Ŵ�ֱ��Ļ������ת�̶��Ƕ�
		void SaveCurrentView();//���浱ǰ�ӽ�
		void CallSavedView();	//���ñ�����ӽ�
		void SetPVMValue();														//����PVM�����ֵ

			

		//�����ӽ�																													
		void SetPerspective(Perspective type);	//���ù۲��ӽ�

	};
}
