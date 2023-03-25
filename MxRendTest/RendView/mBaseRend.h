#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_5_Core>

#include"mMeshViewEnum.h"
#include "app.h"

namespace MViewBasic
{
	class mModelView;
	class mCommonView;
}
namespace mxr
{
	class Group;
	class Viewer;
}
using namespace std;
using namespace MViewBasic;
namespace MBaseRend
{
	class mBackGroundRender;
	class mBaseRender;
	typedef QVector<shared_ptr<mBaseRender>> RenderArray;
	class RENDVIEW_EXPORT mBaseRend : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
	{
		Q_OBJECT

	public:
		mBaseRend(const QString& name);
		~mBaseRend();

		std::shared_ptr<mxr::Application> getApplication() { return _app; };

		shared_ptr<mxr::Group> getRoot() { return _root; };

		shared_ptr<mModelView> getCamera() {return _modelView;};

		void addRender(shared_ptr<mBaseRender> baseRender);

		void removeRender(shared_ptr<mBaseRender> baseRender);

		void clearRender();

	public:
		/*
		* ��д���ຯ��
		*/
		void initializeGL()  Q_DECL_OVERRIDE;
		void resizeGL(int w, int h) Q_DECL_OVERRIDE;
		void paintGL() Q_DECL_OVERRIDE;
		void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
		void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
		void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
		void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

	protected:
		void GetPointDepthAtMouse();

	public:
		//��ѡ�Ŵ�
		//void SetZoomAtFrameCenter();//��������ѡȡ�����Ľ������ţ���������
		//��ͼ����Ӧ
		//void FitView();
		//������ת����Ϊ��ͼ����
		//void SetRotateCenterToViewCenter();
		//������ת����Ϊ��ͼ��һ��
		//void SetRotateCenterToPoint();
		//������ת����Ϊ��������
		//void SetRotateCenterToModelCenter();
		//���õ����ť���Ŵ�ֱ��Ļ����ת
		//void SetRotate_ByButton(float angle);
		//������Ļ��������(�����������ţ�������)
		//void SetZoomAtViewCenter_ByButton(ScaleDirection);
		//��ʼ���ӽǲۺ���
		//void SetPerspective(Perspective);
		//���浱ǰ�ӽ�
		//void SaveCurrentView();
		//���ñ�����ӽ�
		//void CallSavedView();

		//������Ļ��������(����ƶ��������ţ�����)
		//void SetZoomAtViewCenter_ByMove();
		//���õ������м�������ת����
		//void SetRotateType(RotateType);
		//���õ������м�����ƽ��
		//void SetTranslateXY();

		//�����Զ���Ƕ���ת�ۺ���
		//void slotSetRotate_ByButton(float angle);

		//��ʾ���ظ����ӽ�
		//void slotUpdateOrthoAndCamera();

	protected:
		QString _name;

		std::shared_ptr<mxr::Application> _app;

		std::shared_ptr<mxr::Group> _root;

		std::shared_ptr<mxr::Viewer> _viewer;

		std::shared_ptr< mBackGroundRender> _bgRend;

		std::shared_ptr<mModelView> _modelView;

		std::shared_ptr<mCommonView> _commonView;

		RenderArray _renderArray;

		//���ڴ�С
		int SCR_WIDTH = 800, SCR_HEIGHT = 600;
		//�������Ļ�е�����
		GLint lastX{}, lastY{}, nowX{}, nowY{};
		//��һ���������ȡ����������
		GLint Posx_Firstmouse, Posy_Firstmouse;
		//���ο������
		QVector2D left_up, left_down, right_down, right_up;
		//��ȡ��ǰ��������ֵ������ģ�ͣ�//��������ĳһ����ת
		float BufDepth = 0.0; //BufDepth��Χ��0, 1��

		//ģ�Ͳ���
		QVector3D _center_model; //ģ������
		float _maxRadius_model;//�����ת�뾶(��ģ������)
		QVector3D _center_now; //��ǰ��ת����
		float _maxRadius_now;//�����ת�뾶���Ե�ǰ���ģ�
		float _left, _right, _top, _bottom, _back, _front; //ģ����X,Y,Z�����ϵ�������


		//����������ж�
		bool isFirstMouse = true, isMiddleMousePress = false, isLeftMousePress = false, isRightMousePress = false;
		//����ƶ��ж�
		bool isMouseMove = false;

		//�ӽǹ����ж�
		bool ifGetRotateCenter = false; //�Ƿ��ȡ��ת���ģ���ǰ���ĵ�����ص����꣩
		bool ifRotateAtViewCenter = false;//�Ƿ������ӽ�������ת
		bool ifRotateAtXY = true;//�Ƿ���XYƽ����ת
		bool ifRotateAtZ = false;//�Ƿ�����Z����ת
		bool ifTranslateXY = false; //�Ƿ���XYƽ�����ƶ�
		bool ifZoomByMouseMove = false; //�ǹ���������ƶ���������
		bool ifZoomAtFrameCenter = false; //�ǹ�����ѡȡ�����Ľ�������



	};
}
