#include "CCDirector.h"
#include "CCGLWidget.h"
#include <QtCore/QTimer>

GLWidget::GLWidget(int width, int height, QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000 / 60);

    resize(width, height);
}

void GLWidget::update()
{
    cocos2d::CCDirector::sharedDirector()->mainLoop();
}
