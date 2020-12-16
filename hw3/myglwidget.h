#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
//#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

class MyGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL();

    void init_shaders(const char* vertex_path, const char* fragment_path);
    void init_vbo(int u, int v);

    void draw();

    void paintGL();
    void resizeGL(int width, int height);

private:
    QTimer *timer;
    GLuint shader_program;
    int u_num, v_num;
};

void create_ball(GLfloat* ball, GLuint u_num, GLuint v_num, GLfloat r);
GLfloat* get_point_on_ball(GLfloat u, GLfloat v, GLfloat r);

#endif // MYGLWIDGET_H
