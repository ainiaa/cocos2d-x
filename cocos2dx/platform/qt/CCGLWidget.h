#ifndef  _GL_WIDGET_H_
#define  _GL_WIDGET_H_

#include "CCCommon.h"
#include <QtOpenGL/QGLWidget>

class CC_DLL GLWidget : public QGLWidget
{
    Q_OBJECT

    typedef void(*PTRFUN)(QMouseEvent *event);

public:
    GLWidget(){}
    GLWidget(int width, int height, QWidget *parent = 0);

    void setMouseMoveFunc(PTRFUN func);
    void setMousePressFunc(PTRFUN func);
    void setMouseReleaseFunc(PTRFUN func);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

public slots:
    void update();

private:
    PTRFUN mouseMoveFunc;
    PTRFUN mousePressFunc;
    PTRFUN mouseReleaseFunc;
};

#endif // _GL_WIDGET_H_
