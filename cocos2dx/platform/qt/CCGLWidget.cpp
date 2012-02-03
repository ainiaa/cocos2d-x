#include "CCDirector.h"
#include "CCGLWidget.h"
#include <QtCore/QTimer>

GLWidget::GLWidget(int width, int height, QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
  , mouseMoveFunc(NULL)
  , mousePressFunc(NULL)
  , mouseReleaseFunc(NULL)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000 / 60);

    resize(width, height);
}

void GLWidget::setMouseMoveFunc(PTRFUN func)
{
    mouseMoveFunc = func;
}

void GLWidget::setMousePressFunc(PTRFUN func)
{
    mousePressFunc = func;
}

void GLWidget::setMouseReleaseFunc(PTRFUN func)
{
    mouseReleaseFunc = func;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (mouseMoveFunc)
        mouseMoveFunc(event);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if (mousePressFunc)
        mousePressFunc(event);
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (mouseReleaseFunc)
        mouseReleaseFunc(event);
}

void GLWidget::update()
{
    glewInit();
    cocos2d::CCDirector::sharedDirector()->mainLoop();
}
