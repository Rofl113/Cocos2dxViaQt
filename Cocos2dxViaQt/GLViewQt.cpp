#include "GLViewQt.h"
// cocos2d lib
#include <cocos/base/CCEventMouse.h>
#include <cocos/base/CCDirector.h>
#include <cocos/base/CCEventDispatcher.h>
// Common
#include "QtWidgetCocos2d.h"



namespace
{

cocos2d::EventMouse::MouseButton convertType(const Qt::MouseButton& type)
{
	switch (type)
	{
	case Qt::LeftButton:
		return cocos2d::EventMouse::MouseButton::BUTTON_LEFT;

	case Qt::MiddleButton:
		return cocos2d::EventMouse::MouseButton::BUTTON_MIDDLE;

	case Qt::RightButton:
		return cocos2d::EventMouse::MouseButton::BUTTON_RIGHT;

	default:
		assert(0);
		break;
	}
	return cocos2d::EventMouse::MouseButton::BUTTON_UNSET;
}

} // END namespace


GLViewQt::GLViewQt(const std::string& name, QtWidgetCocos2d* widget)
    : _window(widget)
    , _screenScaleFactor(1.0f)
    , _lastWindowHeight(0.0f)
    , _bgColor(0, 0, 0, 1)
{
	this->setViewName(name);

	_window->setMouseMoveFunc([this] (QMouseEvent* event)
	{
		this->mouseMove(event);
	});
	_window->setMousePressFunc([this] (QMouseEvent* event)
	{
		this->mousePress(event);
	});
	_window->setMouseReleaseFunc([this] (QMouseEvent* event)
	{
		this->mouseRelease(event);
	});
	_window->setWheelFunc([this] (QWheelEvent* event)
	{
		this->wheel(event);
	});

	_window->setWindowFlags(_window->windowFlags() & ~Qt::WindowMaximizeButtonHint);
//	_window->setBaseSize(width, height);
//	_window->setMinimumSize(500, 500);
//	_window->setMaximumSize(1920, 1080);
//	_window->show();
}

GLViewQt::~GLViewQt()
{

}

GLViewQt* GLViewQt::create(const std::string& name, QtWidgetCocos2d* widget)
{
	assert(widget);
	if (nullptr == widget)
	{
		return nullptr;
	}
	const auto frameSize = widget->frameSize();
	const float widthFrame = float(frameSize.width());
	const float heightFrame = float(frameSize.height());

	auto view = new (std::nothrow) GLViewQt(name, widget);
	assert(view);
	if (view == nullptr)
	{
		return nullptr;
	}
	view->setDesignResolutionSize(widthFrame, heightFrame, ResolutionPolicy::EXACT_FIT);
	const bool ret = view->initGlew();
	assert(ret);
	if (ret == false)
	{
		return nullptr;
	}
	return view;
}

bool GLViewQt::isOpenGLReady()
{
	return true;
}

void GLViewQt::end()
{
//	CC_SAFE_DELETE(_window);

	delete this;
}

void GLViewQt::swapBuffers()
{
	{
		/* Swap buffers */
		_window->makeCurrent();
//		_window->swapBuffers();
	}
}

void GLViewQt::setIMEKeyboardState(bool /*bOpen*/)
{
	/* Open or close IME keyboard */
}

void GLViewQt::setViewName(const std::string& name)
{
	ClassBase::setViewName(name);
	if (_window)
	{
		_window->setWindowTitle(getViewName().c_str());
	}
}

void GLViewQt::setFrameSize(float width, float height)
{
	ClassBase::setFrameSize(width, height);
	_window->resize(width, height);
	setDesignResolutionSize(_window->baseSize().width(), _window->baseSize().height(), ResolutionPolicy::NO_BORDER);
	cocos2d::Director::getInstance()->setProjection(cocos2d::Director::getInstance()->getProjection());
	glClearColor(_bgColor.r, _bgColor.g, _bgColor.b, _bgColor.a);
}

void GLViewQt::resize()
{
	if (_window)
	{
		const float ratio = (float)(_window->baseSize().width()) / (float)(_window->baseSize().height());
		this->setFrameSize(_window->height() * ratio, _window->height());
	}
}

void GLViewQt::updateWindowSize()
{
	if (_window)
	{
		/* resize window if window height changed */
		if (_lastWindowHeight != _window->height())
		{
			_lastWindowHeight = _window->height();
			this->resize();
		}
	}
}

void GLViewQt::setBgColor(cocos2d::Color4B &color)
{
	_bgColor = cocos2d::Color4F((float)(color.r) / 255.f, (float)(color.g) / 255.f, (float)(color.b) / 255.f, (float)(color.a) / 255.f);
	glClearColor(_bgColor.r, _bgColor.g, _bgColor.b, _bgColor.a);
}

void GLViewQt::mouseMove(QMouseEvent *event)
{
	if (not _captured)
	{
		return;
	}

	_mouseX = (float)(event->x()) / _screenScaleFactor;
	_mouseY = (float)(event->y()) / _screenScaleFactor;

	_mouseX /= this->getFrameZoomFactor();
	_mouseY /= this->getFrameZoomFactor();

	if (_isInRetinaMonitor)
	{
		if (_retinaFactor == 1)
		{
			_mouseX *= 2;
			_mouseY *= 2;
		}
	}

	if (_captured)
	{
		intptr_t id = 0;
		this->handleTouchesMove(1, &id, &_mouseX, &_mouseY);
	}

	//Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
	float cursorX = (_mouseX - _viewPortRect.origin.x) / _scaleX;
	float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - _mouseY) / _scaleY;

	cocos2d::EventMouse ccEvent(cocos2d::EventMouse::MouseEventType::MOUSE_MOVE);
	// Set current button
	{
		const auto typeButton = convertType(event->button());
		// Check
		if (typeButton != cocos2d::EventMouse::MouseButton::BUTTON_UNSET)
		{
			ccEvent.setMouseButton(typeButton);
		}
		else
		{
			assert(0);
			return;
		}
	}
	ccEvent.setCursorPosition(cursorX, cursorY);
	cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&ccEvent);
}

void GLViewQt::mousePress(QMouseEvent *event)
{
	if (event->button() != Qt::LeftButton)
	{
		return;
	}

	_captured = true;
	_mouseX = (float)(event->x()) / _screenScaleFactor;
	_mouseY = (float)(event->y()) / _screenScaleFactor;

	if (this->getViewPortRect().equals(cocos2d::Rect::ZERO) || this->getViewPortRect().containsPoint(cocos2d::Vec2(_mouseX, _mouseY)))
	{
		intptr_t id = 0;
		this->handleTouchesBegin(1, &id, &_mouseX, &_mouseY);
	}

	//Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
	float cursorX = (_mouseX - _viewPortRect.origin.x) / _scaleX;
	float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - _mouseY) / _scaleY;

	cocos2d::EventMouse ccEvent(cocos2d::EventMouse::MouseEventType::MOUSE_DOWN);
	ccEvent.setCursorPosition(cursorX, cursorY);
	const auto typeButton = convertType(event->button());
	// Check
	if (typeButton != cocos2d::EventMouse::MouseButton::BUTTON_UNSET)
	{
		ccEvent.setMouseButton(typeButton);
	}
	else
	{
		assert(0);
		return;
	}
	cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&ccEvent);
}

void GLViewQt::mouseRelease(QMouseEvent *event)
{
	if (event->button() != Qt::LeftButton)
	{
		return;
	}

	if (_captured)
	{
		_captured = false;
		intptr_t id = 0;
		this->handleTouchesEnd(1, &id, &_mouseX, &_mouseY);
	}

	//Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
	float cursorX = (_mouseX - _viewPortRect.origin.x) / _scaleX;
	float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - _mouseY) / _scaleY;

	cocos2d::EventMouse ccEvent(cocos2d::EventMouse::MouseEventType::MOUSE_UP);
	ccEvent.setCursorPosition(cursorX, cursorY);
	const auto typeButton = convertType(event->button());
	// Check
	if (typeButton != cocos2d::EventMouse::MouseButton::BUTTON_UNSET)
	{
		ccEvent.setMouseButton(typeButton);
	}
	else
	{
		assert(0);
		return;
	}
	cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&ccEvent);
}

void GLViewQt::wheel(QWheelEvent *event)
{
	//Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
	float cursorX = (_mouseX - _viewPortRect.origin.x) / _scaleX;
	float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - _mouseY) / _scaleY;

	cocos2d::EventMouse ccEvent(cocos2d::EventMouse::MouseEventType::MOUSE_SCROLL);
	ccEvent.setScrollData((float)event->x(), -(float)event->y());
	ccEvent.setCursorPosition(cursorX, cursorY);
	cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&ccEvent);
}

QWidget *GLViewQt::getGLWidget()
{
	return _window;
}
