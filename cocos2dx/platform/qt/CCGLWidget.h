#ifndef  _GL_WIDGET_H_
#define  _GL_WIDGET_H_

#include "CCCommon.h"
#include <QtOpenGL/QGLWidget>

class CC_DLL GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(){}
    GLWidget(int width, int height, QWidget *parent = 0);

public slots:
    void update();
};

#endif // _GL_WIDGET_H_
