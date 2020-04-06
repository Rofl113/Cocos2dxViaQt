#include "QtWidgetCocos2d.h"



QtWidgetCocos2d::QtWidgetCocos2d(QWidget *parent)
    : ClassBase(parent)
    , mouseMoveFunc(nullptr)
    , mousePressFunc(nullptr)
    , mouseReleaseFunc(nullptr)
    , wheelFunc(nullptr)
    , keyEventFunc(nullptr)
{
}

QtWidgetCocos2d::~QtWidgetCocos2d()
{
}

void QtWidgetCocos2d::setMouseMoveFunc(const std::function<void(QMouseEvent*)>& func)
{
	mouseMoveFunc = func;
}

void QtWidgetCocos2d::setMousePressFunc(const std::function<void(QMouseEvent*)>& func)
{
	mousePressFunc = func;
}

void QtWidgetCocos2d::setMouseReleaseFunc(const std::function<void(QMouseEvent*)>& func)
{
	mouseReleaseFunc = func;
}

void QtWidgetCocos2d::setWheelFunc(const std::function<void(QWheelEvent*)>& func)
{
	wheelFunc = func;
}

void QtWidgetCocos2d::setKeyEventFunc(const std::function<void(QKeyEvent*)>& func)
{
	keyEventFunc = func;
}

void QtWidgetCocos2d::mouseMoveEvent(QMouseEvent *event)
{
	if (mouseMoveFunc)
	{
		mouseMoveFunc(event);
	}

	ClassBase::mouseMoveEvent(event);
}

void QtWidgetCocos2d::mousePressEvent(QMouseEvent *event)
{
	if (mousePressFunc)
	{
		mousePressFunc(event);
	}

	ClassBase::mousePressEvent(event);
}

void QtWidgetCocos2d::mouseReleaseEvent(QMouseEvent *event)
{
	if (mouseReleaseFunc)
	{
		mouseReleaseFunc(event);
	}

	ClassBase::mouseReleaseEvent(event);
}

void QtWidgetCocos2d::wheelEvent(QWheelEvent *event)
{
	if (wheelFunc)
	{
		wheelFunc(event);
	}

	ClassBase::wheelEvent(event);
}

void QtWidgetCocos2d::keyPressEvent(QKeyEvent *event)
{
	if (keyEventFunc)
	{
		keyEventFunc(event);
	}

	ClassBase::keyPressEvent(event);
}

void QtWidgetCocos2d::keyReleaseEvent(QKeyEvent *event)
{
	if (keyEventFunc)
	{
		keyEventFunc(event);
	}

	ClassBase::keyReleaseEvent(event);
}
