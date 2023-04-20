#include "mCameraModelView.h"
//
#include <QDebug>
#include <QtMath>
namespace MViewBasic
{

	mCameraModelView::mCameraModelView(QWidget *widget)
		: m_widget(widget),
		m_lastFrame(0.0),
		m_deltaTime(0.0),
		m_timerId(0),
		m_isMouseMoveDisabled(false),
		m_isPitchConstrained(true)
	{
		Position = QVector3D(0,0,0);
		WorldUp = QVector3D(0, 1, 0);;
		Yaw = -90;
		Pitch = 0;
		calculateCameraDirection();
		init();
		m_keyMap[FORWARD] = Qt::Key_W;
		m_keyMap[BACKWARD] = Qt::Key_S;
		m_keyMap[LEFT] = Qt::Key_A;
		m_keyMap[RIGHT] = Qt::Key_D;
		//m_keyMap[DISABLE] = Qt::Key_Q;
		//m_keyMap[ENABLE] = Qt::Key_E;
	}

	void mCameraModelView::init()
	{
		m_widget->activateWindow();
		m_widget->setFocus();
		QCursor::setPos(m_widget->mapToGlobal(m_widget->rect().center()));
		m_widget->setFocusPolicy(Qt::StrongFocus);
		m_widget->setCursor(Qt::BlankCursor);
		//m_widget->setWindowState(Qt::WindowFullScreen);

		Position  = QVector3D(0.0, 0.0, 3.0);
		_Up = QVector3D(0.0, 1.0, 0.0);
		Front = QVector3D(0, 0, -1);

		MouseSensitivity = 0.1f;
		MovementSpeed = 2.5;
		Zoom = 45;
	}

	void mCameraModelView::handle(QEvent *event)
	{
		switch (event->type()) {

		case QEvent::MouseMove:
		{
			auto mouseEvent = static_cast<QMouseEvent*>(event);
			handleMouseMoveEvent(mouseEvent);
			break;
		}
		case QEvent::Timer:
		{
			auto timerEvent = static_cast<QTimerEvent*>(event);
			handleTimerEvent(timerEvent);
			break;
		}
		case QEvent::Wheel:
		{
			auto wheelEvent = static_cast<QWheelEvent*>(event);
			handleWheelEvent(wheelEvent);
			break;
		}
		case QEvent::KeyPress:
		{
			auto keyPressEvent = static_cast<QKeyEvent*>(event);
			m_pressedKeys.insert(keyPressEvent->key());
			if (!keyPressEvent->isAutoRepeat() && m_timerId == 0) {
				m_timerId = m_widget->startTimer(1);
			}
			break;
		}
		case QEvent::KeyRelease:
		{
			auto keyReleaseEvent = static_cast<QKeyEvent*>(event);
			m_pressedKeys.remove(keyReleaseEvent->key());
			if (!keyReleaseEvent->isAutoRepeat() && m_timerId != 0 && m_pressedKeys.empty()) {
				m_widget->killTimer(m_timerId);
				m_timerId = 0;
			}
			break;
		}
		case QEvent::Paint:
		{
			float curTime = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;
			m_deltaTime = curTime - m_lastFrame;
			m_lastFrame = curTime;
			break;
		}
		case QEvent::FocusIn:
		{
			m_lastMousePos = m_widget->geometry().center();
			QCursor::setPos(m_widget->mapToGlobal(m_lastMousePos));        
			m_widget->setCursor(Qt::BlankCursor);          
			m_widget->setMouseTracking(true);
			break;
		}
		}
	}

	//************************************** get set ****************************************************

	void mCameraModelView::setKeyMap(mCameraModelView::CameraMovement key, Qt::Key value)
	{
		m_keyMap[key] = value;
	}

	float mCameraModelView::getMoveSpeed() const
	{
		return MovementSpeed;
	}

	void mCameraModelView::setMoveSpeed(float value)
	{
		MovementSpeed = value;
	}

	float mCameraModelView::getSensitivity() const
	{
		return MouseSensitivity;
	}

	void mCameraModelView::setSensitivity(float value)
	{
		MouseSensitivity = value;
	}

	float mCameraModelView::getYaw() const
	{
		return Yaw ;
	}

	void mCameraModelView::setYaw(float value)
	{
		Yaw  = value;
		calculateCameraDirection();
	}

	float mCameraModelView::getPitch() const
	{
		return Pitch ;
	}

	void mCameraModelView::setPitch(float value)
	{
		Pitch  = value;
		calculateCameraDirection();
	}

	void mCameraModelView::setPitchConstrained(bool flag)
	{
		m_isPitchConstrained = flag;
	}

	float mCameraModelView::getFov() const
	{
		return Zoom;
	}

	void mCameraModelView::setFov(float value)
	{
		Zoom = value;
	}

	QVector3D mCameraModelView::getCameraPos() const
	{
		return Position ;
	}

	void mCameraModelView::setCameraPos(const QVector3D &value)
	{
		Position  = value;
	}

	QVector3D mCameraModelView::getCameraUp() const
	{
		return _Up;
	}

	void mCameraModelView::setCameraUp(const QVector3D &value)
	{
		_Up = value;
	}

	QVector3D mCameraModelView::getCameraDirection() const
	{
		return Front;
	}

	void mCameraModelView::ResetOrthoAndCamera(QVector3D center, float maxRadius)
	{
		_MaxRadius = maxRadius;
		_Center = center;
		Position = QVector3D(0.0, 0.0, 2 * maxRadius);
		_Up = QVector3D(0.0, 1.0, 0.0);
		//Front = center - Position ;
		calculateCameraDirection();
		m_widget->update();
	}

	//***************************************** handle event *********************************************

	void mCameraModelView::handleMouseMoveEvent(QMouseEvent *event)
	{
		float offsetX = event->x() - m_lastMousePos.x();
		float offsetY = m_lastMousePos.y() - event->y();
		offsetX *= MouseSensitivity;
		offsetY *= MouseSensitivity;
		
		m_lastMousePos = event->pos();
		
		if (m_isMouseMoveDisabled) return;
		   
		Yaw  += offsetX;
		Pitch  += offsetY;
		

		if (m_isPitchConstrained) {
			
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		calculateCameraDirection();

		m_widget->update();
	}

	void mCameraModelView::handleWheelEvent(QWheelEvent *event)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= event->angleDelta().y()/360.0;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
		//Zoom += event->angleDelta().y() / 360.0;
		//if (Zoom > 45.0) {
		//	Zoom = 45.0;
		//}
		//else if (Zoom < 1.0) {
		//	Zoom = 1.0;
		//}
		calculateCameraDirection();
		m_widget->update();
	}

	void mCameraModelView::handleTimerEvent(QTimerEvent *event)
	{
		if (event->timerId() != m_timerId) {
			return;
		}
		float speed = m_deltaTime * MovementSpeed;
		if (m_pressedKeys.contains(m_keyMap[FORWARD])) {
			Position  += speed * Front;
			calculateCameraDirection();
		}
		if (m_pressedKeys.contains(m_keyMap[BACKWARD])) {
			Position  -= speed * Front;
			calculateCameraDirection();
		}
		if (m_pressedKeys.contains(m_keyMap[LEFT])) {
			Position  -= speed * Right;
			calculateCameraDirection();
		}
		if (m_pressedKeys.contains(m_keyMap[RIGHT])) {
			Position  += speed * Right;
			calculateCameraDirection();
		}
		//禁用鼠标监控
		if (m_pressedKeys.contains(m_keyMap[DISABLE])) {
			m_isMouseMoveDisabled = true;
			m_widget->setCursor(Qt::ArrowCursor);
		}

		if (m_pressedKeys.contains(m_keyMap[ENABLE])) {
			m_isMouseMoveDisabled = false;
			m_widget->setCursor(Qt::BlankCursor);  
		}

		m_widget->update();
	}


	//**************************************** calculate method ********************************************
	
	void mCameraModelView::calculateCameraDirection()
	{
		Front = QVector3D(cos(qDegreesToRadians(Yaw )) * cos(qDegreesToRadians(Pitch )), sin(qDegreesToRadians(Pitch )), sin(qDegreesToRadians(Yaw )) * cos(qDegreesToRadians(Pitch )));
		Front.normalize();
		Right = QVector3D::crossProduct(Front, WorldUp);
		Up = QVector3D::crossProduct(Right, Front);
		SetPVMValue();
		_Eye = Position;
		//_Center = Position + Front;
		_Up = Up;
	}


	void mCameraModelView::SetPVMValue()
	{
		_view.setToIdentity();
		_view.lookAt(Position , Position  + Front, Up);
		_model.setToIdentity();
		_projection.setToIdentity();
		_projection.perspective(getFov(), 1.0 * m_widget->width() / m_widget->height(), 0.1, 100.0);
	}
}

/*
namespace MViewBasic
{
	void mCameraModelView::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
	{

		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();

	}
	void mCameraModelView::ZoomAtMouse_Bywheel(int posX, int posY, int yoffset, ScalePerspectice scalePerspectice, float sensitivity)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}
	void mCameraModelView::SetPVMValue()
	{
		{
			_view.setToIdentity();
			_view.lookAt(Position , Position  + Front, QVector3D(0,1,0));
			_model.setToIdentity();
			_projection.setToIdentity();
			_projection.perspective(qDegreesToRadians(Zoom), _widget->width() / _widget->height(), 0.1, 100.0);
		}
	}
	void mCameraModelView::ResetOrthoAndCamera(QVector3D center, float maxRadius)
	{
		
			_MaxRadius = maxRadius;
			_Center = center;
			Position  = QVector3D(0.0, 0.0, maxRadius + 1);
			_Up = QVector3D(0.0, 1.0, 0.0);
			updateCameraVectors();
			_widget->update();

		
	}
}

*/