#pragma once
#include <functional>
#include <QOpenGLWidget>

class QTimer;

class QtWidgetCocos2d : public QOpenGLWidget
{
	Q_OBJECT
	using ClassBase = QOpenGLWidget;
	QtWidgetCocos2d() = delete;
	QtWidgetCocos2d(QtWidgetCocos2d&) = delete;
	QtWidgetCocos2d& operator =(QtWidgetCocos2d&) = delete;
public:
	~QtWidgetCocos2d();
	QtWidgetCocos2d(QWidget *parent = 0);

	void setMouseMoveFunc(const std::function<void(QMouseEvent*)>& func);
	void setMousePressFunc(const std::function<void(QMouseEvent*)>& func);
	void setMouseReleaseFunc(const std::function<void(QMouseEvent*)>& func);
	void setWheelFunc(const std::function<void(QWheelEvent*)>& func);
	void setKeyEventFunc(const std::function<void(QKeyEvent*)>& func);

private: /// QOpenGLWidget
	virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;
	virtual void wheelEvent(QWheelEvent *event) override;
	virtual void keyPressEvent(QKeyEvent *event) override;
	virtual void keyReleaseEvent(QKeyEvent *event) override;

private:
	std::function<void(QMouseEvent*)> mouseMoveFunc;
	std::function<void(QMouseEvent*)> mousePressFunc;
	std::function<void(QMouseEvent*)> mouseReleaseFunc;
	std::function<void(QWheelEvent*)> wheelFunc;

	std::function<void(QKeyEvent*)> keyEventFunc;
};
