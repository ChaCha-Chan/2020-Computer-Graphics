#include "myglwidget.h"
#include<iostream>
using namespace std;

MyGLWidget::MyGLWidget(QWidget *parent)
	:QOpenGLWidget(parent)
{
	timer = new QTimer(this); 
	timer->start(3000);
	connect(timer, SIGNAL(timeout()), this, SLOT(update())); 
}

MyGLWidget::~MyGLWidget()
{
	delete this->timer;
}

void MyGLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	glViewport(0, 0, width(), height());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

GLfloat* get_point_on_ball(GLfloat u, GLfloat v, GLfloat r) {
	GLfloat* points = new GLfloat[3];
	points[0] = r * sin(M_PI * u) * cos(2 * M_PI * v);//x
	points[1] = r * sin(M_PI * u) * sin(2 * M_PI * v);//y
	points[2] = r * cos(M_PI * u);//z
	return points;
}

void MyGLWidget::create_ball(GLfloat* ball) {
	GLfloat u_step = 1.0f / u_num;
	GLfloat v_step = 1.0f / v_num;
	GLuint offset = 0;
	for (int u = 0; u < u_num; u++) {
		for (int v = 0; v < v_num; v++) {
			GLfloat* point_1 = get_point_on_ball(u * u_step, v * v_step, r);
			GLfloat* point_2 = get_point_on_ball((u + 1) * u_step, v * v_step, r);
			GLfloat* point_3 = get_point_on_ball((u + 1) * u_step, (v + 1) * v_step, r);
			GLfloat* point_4 = get_point_on_ball(u * u_step, (v + 1) * v_step, r);

			GLfloat* points[6] = { point_1, point_4, point_3, point_1, point_3, point_2 };
			for (int i = 0; i < 6; i++) {
				memcpy(ball + offset, points[i], sizeof(GLfloat) * 3);
				offset += 3;
			}
			delete[] point_1, point_2, point_3, point_4;
		}
	}
}

void MyGLWidget::create_ball_with_indices(GLfloat* ball) {
	GLfloat u_step = 1.0f / u_num;
	GLfloat v_step = 1.0f / v_num;
	GLuint offset = 0;
	for (int u = 0; u < u_num; u++) {
		for (int v = 0; v < v_num; v++) {
			GLfloat* point_1 = get_point_on_ball(u * u_step, v * v_step, r);
			GLfloat* point_2 = get_point_on_ball((u + 1) * u_step, v * v_step, r);
			GLfloat* point_3 = get_point_on_ball((u + 1) * u_step, (v + 1) * v_step, r);
			GLfloat* point_4 = get_point_on_ball(u * u_step, (v + 1) * v_step, r);

			GLfloat* points[4] = { point_1, point_2, point_3, point_4};
			for (int i = 0; i < 4; i++) {
				memcpy(ball + offset, points[i], sizeof(GLfloat) * 3);
				offset += 3;
			}
			delete[] point_1, point_2, point_3, point_4;
		}
	}
}

void MyGLWidget::init_vbo(GLfloat* ball) {
	//创建VBO，VAO
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 3 * u_num * v_num, ball, GL_STATIC_DRAW);

	//链接顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
}

void MyGLWidget::init_vbo_with_indices(GLfloat* ball, GLint* indices) {
	//创建VBO，VAO，EBO
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 3 * u_num * v_num, ball, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * 6 * 3 * u_num * v_num, indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
}

//不使用VBO
void MyGLWidget::draw1() {
	GLfloat* ball = new GLfloat[6 * 3 * u_num * v_num];
	create_ball(ball);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 6 * 3 * u_num * v_num; i += 3) {
		glVertex3f(ball[i], ball[i + 1], ball[i + 2]);
	}
	glEnd();
	delete[] ball;
}
//使用VBO
void MyGLWidget::draw2() {
	//创建球的数组
	GLfloat* ball = new GLfloat[6 * 3 * u_num * v_num];
	create_ball(ball);
	init_vbo(ball);
	glDrawArrays(GL_TRIANGLES, 0, 6 * u_num * v_num);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	delete[] ball;
}
//使用EBO
void MyGLWidget::draw3() {
	//创建球的数组
	GLfloat* ball = new GLfloat[4 * 3 * u_num * v_num];
	create_ball_with_indices(ball);
	//创建索引数组
	GLint* indices = new GLint[6 * 3 * u_num * v_num];
	for (GLint i = 0, j = 0; i < 6 * 3 * u_num * v_num; i += 6, j += 4) {
		GLint index[6] = { j, j + 3, j + 2, j, j + 2, j + 1 };
		memcpy(indices + i, index, sizeof(GLint) * 6);
	}
	init_vbo_with_indices(ball, indices);
	glDrawElements(GL_TRIANGLES, 6 * u_num * v_num, GL_UNSIGNED_INT, 0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	delete[] ball;
	delete[] indices;
}

void MyGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLfloat)width() / (GLfloat)height(), 0.1f, 200.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f);

	glColor3f(1.0f, 1.0f, 0.0f);

	static int cnt = 0;
	static double total_time = 0.0;
	double current_time;

	LARGE_INTEGER  freq, start, end;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);

	//draw1();
	//draw2();
	draw3();

	QueryPerformanceCounter(&end);
	cnt += 1;
	current_time = (__int64)(end.QuadPart - start.QuadPart) * 1000 / (double)freq.QuadPart;
	total_time += current_time;
	printf(" %lfms\n\n", current_time);
	if (cnt % 10 == 0) {
		printf(" cnt = %d, avg_time = %lfms.\n\n", cnt, total_time / cnt);
	}
}

void MyGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	update();
}

