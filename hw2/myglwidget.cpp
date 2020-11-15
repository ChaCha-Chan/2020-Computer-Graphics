#include "myglwidget.h"

/*###################################################
##  函数: MyGLWidget
##  函数描述： MyGLWidget类的构造函数，实例化定时器timer
##  参数描述：
##  parent: MyGLWidget的父对象
#####################################################*/

MyGLWidget::MyGLWidget(QWidget *parent)
	:QOpenGLWidget(parent)
{
	timer = new QTimer(this); // 实例化一个定时器
	timer->start(16); // 时间间隔设置为16ms，可以根据需要调整
	connect(timer, SIGNAL(timeout()), this, SLOT(update())); // 连接update()函数，每16ms触发一次update()函数进行重新绘图
}


/*###################################################
##  函数: ~MyGLWidget
##  函数描述： ~MyGLWidget类的析构函数，删除timer
##  参数描述： 无
#####################################################*/
MyGLWidget::~MyGLWidget()
{
	delete this->timer;
}


/*###################################################
##  函数: initializeGL
##  函数描述： 初始化绘图参数，如视窗大小、背景色等
##  参数描述： 无
#####################################################*/
void MyGLWidget::initializeGL()
{
	glViewport(0, 0, width(), height());  
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glDisable(GL_DEPTH_TEST);             
}


/*###################################################
##  函数: drawBox
##  函数描述： 画一个从(0, 0, 0)到(1, 1, 1)的1*1*1立方体。
##  参数描述： 无
#####################################################*/
void drawBox() {
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUAD_STRIP);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glEnd();
    glPopMatrix();
}
/*###################################################
##  函数: drawRobot
##  函数描述： 画一个会摆臂、抬腿的机器人。机器人部件包括头、身体、左右手、胯、左右腿。
##  参数描述： 
##  angle：机器人旋转的角度，由此推得机器人身体各部件旋转的角度
#####################################################*/
void drawRobot(float angle) {
    float angle1 = angle;
    if (angle1 > 180.0f) angle1 -= 180.0f;
    if (angle1 > 90.0f)angle1 = 180.0f - angle1;

    //上半身左右移动，故头、身体、左右手看成一个整体
    glPushMatrix();
    glTranslatef(0.5f, 0.0f, 0.5f);
    glRotatef((-angle1 + 45.0f) * 0.8f, 0.0f, 1.0f, 0.0f);
    glTranslatef(-0.5f, 0.0f, -0.5f);

    //头
    glPushMatrix();
    glTranslatef(0.25f, 1.0f, 0.25f);
    glScalef(0.5f, 0.5f, 0.5f);
    drawBox();
    glPopMatrix();

    //身体
    glPushMatrix();
    drawBox();
    glPopMatrix();

    //左手
    glPushMatrix();
    glTranslatef(-0.5f, -0.25f, 0.25f);
    glTranslatef(0.5f, 1.0f, 0.0f);
    glRotatef(angle1 - 45.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(-0.5f, -1.0f, 0.0f);
    //左手大臂
    glPushMatrix();
    glScalef(0.5f, 1.0f, 0.5f);
    drawBox();
    glPopMatrix();
    //左手小臂
    glPushMatrix();
    glTranslatef(0.0f, -1.0f, 0.0f);
    glTranslatef(0.0f, 1.0f, 0.0f);
    glRotatef(angle1, 1.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, -1.0f, 0.0f);
    glScalef(0.5f, 1.0f, 0.5f);
    drawBox();
    glPopMatrix();
    glPopMatrix();//左手结束

    //右手
    glPushMatrix();
    glTranslatef(1.0f, -0.25f, 0.25f);
    glTranslatef(0.0f, 1.0f, 0.0f);
    glRotatef(-angle1 + 45.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, -1.0f, 0.0f);
    //右手大臂
    glPushMatrix();
    glScalef(0.5f, 1.0f, 0.5f);
    drawBox();
    glPopMatrix();
    //右手小臂
    glPushMatrix();
    glTranslatef(0.0f, -1.0f, 0.0f);
    glTranslatef(0.5f, 1.0f, 0.0f);
    glRotatef(90.0f - angle1, 1.0f, 0.0f, -1.0f);
    glTranslatef(-0.5f, -1.0f, 0.0f);
    glScalef(0.5f, 1.0f, 0.5f);
    drawBox();
    glPopMatrix();
    glPopMatrix();//右手结束

    glPopMatrix();//上半身结束


    //胯
    glPushMatrix();
    glTranslatef(0.25f, -0.5f, 0.25f);
    glScalef(0.5f, 0.5f, 0.5f);
    drawBox();
    glPopMatrix();

    //左腿
    glPushMatrix();
    glTranslatef(-0.25f, -1.75f, 0.25f);
    glTranslatef(0.0f, 1.5f, 0.0f);
    glRotatef((-angle1 + 45.0f) * 0.75f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, -1.5f, 0.0f);
    //左腿大腿
    glPushMatrix();
    glScalef(0.5f, 1.5f, 0.5f);
    drawBox();
    glPopMatrix();
    //左腿小腿
    glPushMatrix();
    glTranslatef(-0.1f, -1.5f, 0.0f);
    glTranslatef(0.5f, 1.5f, 0.0f);
    glRotatef(angle1 * 0.5f, -1.0f, 0.0f, 0.0f);
    glTranslatef(-0.5f, -1.5f, 0.0f);
    glScalef(0.6f, 1.5f, 0.6f);
    drawBox();
    glPopMatrix();
    glPopMatrix();//左腿结束

    //右腿
    glPushMatrix();
    glTranslatef(0.75f, -1.75f, 0.25f);
    glTranslatef(0.0f, 1.5f, 0.0f);
    glRotatef((angle1 - 45.0f) * 0.75f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, -1.5f, 0.0f);
    //右腿大腿
    glPushMatrix();
    glScalef(0.5f, 1.5f, 0.5f);
    drawBox();
    glPopMatrix();
    //右腿小腿
    glPushMatrix();
    glTranslatef(0.0f, -1.5f, 0.0f);
    glTranslatef(0.5f, 1.5f, 0.0f);
    glRotatef((90.0f - angle1) * 0.5f, -1.0f, 0.0f, 0.0f);
    glTranslatef(-0.5f, -1.5f, 0.0f);
    glScalef(0.6f, 1.5f, 0.6f);
    drawBox();
    glPopMatrix();
    glPopMatrix();//右腿结束
}

/*###################################################
##  函数: paintGL
##  函数描述： 绘图函数，实现图形绘制，会被update()函数调用
##  参数描述： 无
#####################################################*/
void MyGLWidget::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    //设置光照，材质
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specular[] = { 0.5f, 0.5f, 0.5f ,1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    GLfloat position[] = { 0.0f, 0.0f, 10.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    GLfloat blue[] = { 0.1f, 0.1f, 0.5f, 1.0f };
    GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, blue);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 1.0f);

    //设置投影矩阵
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (GLfloat)width() / (GLfloat)height(), 0.1f, 200.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //每次调度函数，角度更新
    static float angle = 0.0f;
    float delta = 1.2f;
    angle += delta;
    if (angle >= 360.0f) angle = 0.0f;

    //绘制沿着圆移动的机器人
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -15.0f);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    glTranslatef(3.0f, 0.0f, 0.0f);
    drawRobot(angle);
    glPopMatrix();

    glFlush();
}


/*###################################################
##  函数: resizeGL
##  函数描述： 当窗口大小改变时调整视窗尺寸
##  参数描述： 无
#####################################################*/
void MyGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	update();
}

