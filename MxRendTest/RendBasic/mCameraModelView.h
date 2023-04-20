#pragma once
#include "rendbasic_global.h"

#include <QWheelEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>
#include <QTime>
#include <QHash>
#include <QSet>
#include <QWidget>

#include "mViewBase.h"

namespace MViewBasic
{
	class RENDBASIC_EXPORT mCameraModelView :public mViewBase
	{
	public:
		enum CameraMovement {
			FORWARD,
			BACKWARD,
			LEFT,
			RIGHT,
			DISABLE,
			ENABLE
		};
		mCameraModelView(QWidget* widget);

		void init();

		void handle(QEvent* event);

		void setKeyMap(CameraMovement key, Qt::Key value);

		float getMoveSpeed() const;
		void setMoveSpeed(float value);

		float getSensitivity() const;
		void setSensitivity(float value);
 
		float getYaw() const;
		void setYaw(float value);

		float getPitch() const;
		void setPitch(float value);
		void setPitchConstrained(bool flag);
		//fov
		float getFov() const;
		void setFov(float value);
  
		QVector3D getCameraPos() const;
		void setCameraPos(const QVector3D& value);
		//
		QVector3D getCameraUp() const;
		void setCameraUp(const QVector3D& value);

		QVector3D getCameraDirection() const;


		void ResetOrthoAndCamera(QVector3D center, float maxRadius) override;//传入旋转中心和最大旋转半径

	private:
		void handleMouseMoveEvent(QMouseEvent* event);
		void handleWheelEvent(QWheelEvent* event);
		void handleTimerEvent(QTimerEvent* event);
		void calculateCameraDirection();
		void SetPVMValue() override;

	private:
		QWidget* m_widget;

		// camera Attributes
		QVector3D Position;
		QVector3D Front;
		QVector3D Up;
		QVector3D Right;
		QVector3D WorldUp;
		// euler Angles
		float Yaw;
		float Pitch;
		// camera options
		float MovementSpeed;
		float MouseSensitivity;
		float Zoom;

		//QVector3D m_cameraPos;
		//QVector3D m_cameraDirection;
		//QVector3D m_cameraUp;

		QHash<CameraMovement, Qt::Key> m_keyMap;
		QSet<int> m_pressedKeys;
		int m_timerId;
		float m_deltaTime; 
		float m_lastFrame;

		QPoint m_lastMousePos;

		bool m_isMouseMoveDisabled;
		bool m_isPitchConstrained;
	};
}

