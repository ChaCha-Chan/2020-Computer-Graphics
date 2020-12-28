#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <windows.h>
class MyGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL();
    void create_ball(GLfloat* ball);
    void create_ball_with_indices(GLfloat* ball);
    void init_vbo(GLfloat* ball);
    void init_vbo_with_indices(GLfloat* ball, GLint* indices);
    void paintGL();
    void resizeGL(int width, int height);

    void draw1();
    void draw2();
    void draw3();

    int u_num = 20, v_num = 40;
    GLfloat r = 1.0f;

    GLuint VBO, VAO, EBO;

    LARGE_INTEGER  freq, start, end;
    QTimer* timer;
};
#endif // MYGLWIDGET_H
