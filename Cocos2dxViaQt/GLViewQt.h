#pragma once
// cocos2d lib
#include <cocos/platform/desktop/CCGLViewImpl-desktop.h>
// Qt lib
#include <QMouseEvent>

#include "QtWidgetCocos2d.h"





class GLViewQt : public cocos2d::GLViewImpl
{
	using ClassBase = cocos2d::GLViewImpl;
	GLViewQt() = delete;
	GLViewQt(GLViewQt&) = delete;
	GLViewQt& operator =(GLViewQt&) = delete;
	GLViewQt(const std::string& name, QtWidgetCocos2d* widget);
public:
	virtual ~GLViewQt() override;
	static GLViewQt* create(const std::string& name, QtWidgetCocos2d* widget);

public:
	/* override functions */
	virtual bool isOpenGLReady() override;
	virtual void end() override;
	virtual void swapBuffers() override;
	virtual void setFrameSize(float width, float height) override;
	virtual void setIMEKeyboardState(bool bOpen) override;
	virtual void setViewName(const std::string& name);

	/* Qt functions */
	void mouseMove(QMouseEvent *event);
	void mousePress(QMouseEvent *event);
	void mouseRelease(QMouseEvent *event);
	void wheel(QWheelEvent *event);
	QWidget * getGLWidget();

	void resize(); // auto adjust frame size according to current window size
	void updateWindowSize();
	void setBgColor(cocos2d::Color4B &color);

private:
	float			_screenScaleFactor;
	float			_lastWindowHeight;
	cocos2d::Color4F			_bgColor;
	QtWidgetCocos2d*	_window;
};
