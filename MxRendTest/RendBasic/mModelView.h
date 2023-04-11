#pragma once
#include "rendbasic_global.h"

#include <QObject>
#include<qmatrix4x4.h>
#include <QVector3D>
#include <QVector2D>
#include"mMeshViewEnum.h"

namespace MViewBasic
{
	class RENDBASIC_EXPORT mModelView : public QObject
	{
		Q_OBJECT

	public:
		mModelView(QObject *parent = 0);
		~mModelView();
	

	public:
		
		//��Ļ�����ش�С
		float SCR_WIDTH, SCR_HEIGHT;
				
		//������ӽǲ���
		QVector3D _Eye = QVector3D(0, 0, 1);
		QVector3D _Eye_0 = QVector3D(0, 0, 1);
		QVector3D _Center = QVector3D(0, 0, 0);
		QVector3D _Up = QVector3D(0, 1, 0);

		//�����ת�뾶
		float _MaxRadius;						

		//������Ļ�������
		float _Ratio_Old;						//��һ�ε���Ļ�����
		float _Ratio_New;						// Ŀǰ����Ļ�����

		//��������ͶӰ
		double _Left, _Right, _Bottom, _Top, _NearPlane, _FarPlane;				//����ͶӰȫ�ֱ���
		double _Left_ini, _Right_ini, _Bottom_ini, _Top_ini;						//����ͶӰ��ʼ��������
		double _Left_res, _Right_res, _Bottom_res, _Top_res;						//���ڴ�С�ı�󱣳�ģ������������		
		
		//PVM����
		QMatrix4x4 _projection, _view, _model;
		
		//������ӽǲ���
		QVector3D _Eye_Saved, _Eye_0_Saved, _Center_Saved, _Up_Saved;
		double _MaxRadius_Saved;
		double _Left_Saved, _Right_Saved, _Bottom_Saved, _Top_Saved, _NearPlane_Saved, _FarPlane_Saved;
		double _Left_ini_Saved, _Right_ini_Saved, _Bottom_ini_Saved, _Top_ini_Saved;
		double _Left_res_Saved, _Right_res_Saved, _Bottom_res_Saved, _Top_res_Saved;
		QMatrix4x4 _projection_Saved, _view_Saved, _model_Saved;

		//���ŵı���
		double _ScaleProportion = 1;

		//��ǰ����ͶӰ�Ĳ���(�����ƶ�����)
		float width_firstmouse, height_firstmouse;
		bool IfFirstMouse = false;
		//��ʼ��ģ���ӽ�
		void InitialModelView();
		//��ʼ������ͶӰ
		void InitialOrtho();
		//��ʼ���ӽ�
		void InitialCamera();
		//ͨ����Ļ�������������ͶӰ
		void SetOrthoByRatio(float scr_width, float scr_height);	
		//ͨ����Ļ����Ȼ�ԭ����ͶӰ
		void ReturnOrthoByRatio();		
		//����PVM�����ֵ
		void SetPVMValue();	
		//��ȡPVMֵ
		QMatrix4x4 getPVMValue() { return _projection * _view * _model; };

		//��ת ƽ�� ����
		void Translate(float xoffset, float yoffset);//ƽ��
		void Rotate(float xoffset, float yoffset, RotateType rotateType = Rotate_XY, float sensitivity = 0.25);//��ת
		void Rotate_ByBotton(float angle);//���Ŵ�ֱ��Ļ������ת�̶��Ƕ�	
		void ZoomAtMouse_Bywheel(int posX, int posY, int rollervalue, ScalePerspectice scalePerspectice, float sensitivity = 1.2);//�����������ţ��������ţ�
		void ZoomAtFrameCenter(int CenterposX, int CenterposY, float sensitivity = 2.0);//���ݿ�ѡ��������
		void ZoomAtViewCenter_ByBotton(ScaleDirection scaleDirection, float sensitivity = 2);//�����ӽ���������(�����һ������һ�� ������)
		void ZoomAtViewCenter_ByPercent(float percent);//�����ӽ���������(���ٷֱ����� ��������0.1)
		void ZoomAtViewCenter_ByMove(int FirstPosx, int FirstPosy, int nowPosx, int nowPosy,float sensitivity = 1.1);	//�����ӽ��������ţ�����ƶ����ţ�������
		//�����ӽ�																													
		void SetPerspective(Perspective type);	//���ù۲��ӽ�
		void FitView(QVector3D center);		//�����ӽ�ʹģ�ͳ���������ͼ
		void SaveCurrentView();//���浱ǰ�ӽ�
		void CallSavedView();	//���ñ�����ӽ�
		
		//������ת����
		void SetRotateCenterToPoint(QVector3D center, float maxRadius);	//������ת����Ϊѡ��ĵ�
		void SetRotateCenterToViewCenter(QVector3D center,float maxRadius);	//������ת����Ϊ��ͼ����				
		void SetRotateCenterToModelCenter(QVector3D center,float maxRadius);	//������ת����Ϊ��������
		
		//ģ�ͷ����仯������ӽǣ�������һ�ε���ת���ԣ�
		void ResetOrthoAndCamera(QVector3D center,float maxRadius);//������ת���ĺ������ת�뾶
		//���ģ�Ͷ��������ӽǣ��ӽ�����Ӧ��
		void UpdateOrthoAndCamera(QVector<QVector3D> Allvertex,QVector<QVector3D> AddVertex);
		void UpdateOrthoAndCamera(float maxRadius,QVector<QVector3D> AddVertex);
		//���/ɾ��ģ�Ͳ��ֶ��������ӽǣ��ӽ�������Ӧ��
		void UpdateOrthoAndCamera(QVector<QVector3D> Allvertex);
		void UpdateOrthoAndCamera(float maxRadius_modelcenter, float maxRadius_nowcenter);

		
	};
}

