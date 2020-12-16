#include "myglwidget.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

#define M_PI       3.14159265358979323846
/*###################################################
##  函数: get_point_on_ball
##  函数描述：通过角度和半径计算球面坐标
##  参数描述：
##  u,v:角度, r:半径
#####################################################*/
GLfloat* get_point_on_ball(GLfloat u, GLfloat v, GLfloat r) {
	GLfloat* points = new GLfloat[3];
	points[0] = r * sin(M_PI * u) * cos(2 * M_PI * v);//x
	points[1] = r * sin(M_PI * u) * sin(2 * M_PI * v);//y
	points[2] = r * cos(M_PI * u);//z
	return points;
}

/*###################################################
##  函数: create_ball
##  函数描述：创建存储球面顶点坐标和法向量的数组
##  参数描述：
##  ball:存储顶点坐标和法向量的数组, u_num,v_num:角度拆分数, r:半径
#####################################################*/
void create_ball(GLfloat* ball, GLuint u_num, GLuint v_num, GLfloat r) {
	GLfloat u_step = 1.0f / u_num;
	GLfloat v_step = 1.0f / v_num;
	GLuint offset = 0;
	for (int u = 0; u < u_num; u++) {
		for (int v = 0; v < v_num; v++) {
			GLfloat* point_1 = get_point_on_ball(u * u_step, v * v_step, r);
			GLfloat* point_2 = get_point_on_ball((u + 1) * u_step, v * v_step, r);
			GLfloat* point_3 = get_point_on_ball((u + 1) * u_step, (v + 1) * v_step, r);
			GLfloat* point_4 = get_point_on_ball(u * u_step, (v + 1) * v_step, r);

			//取法向量
			GLfloat v1[3], v2[3], v3[3];
			v1[0] = point_2[0] - point_1[0];
			v1[1] = point_2[1] - point_1[1];
			v1[2] = point_2[2] - point_1[2];

			v2[0] = point_3[0] - point_2[0];
			v2[1] = point_3[1] - point_2[1];
			v2[2] = point_3[2] - point_2[2];

			v3[0] = (v1[1] * v2[2]) - (v2[1] * v1[2]);
			v3[1] = (v1[2] * v2[0]) - (v2[2] * v1[0]);
			v3[2] = (v1[0] * v2[1]) - (v2[0] * v1[1]);

			// 单位化
			GLfloat a = 1.0 / sqrt(v3[0] * v3[0] + v3[1] * v3[1] + v3[2] * v3[2]);
			for (int i = 0; i < 3; i++) {
				v3[i] *= a;
			}
			
			GLfloat* points[6] = { point_1, point_4, point_3, point_1, point_3, point_2 };
			for (int i = 0; i < 6; i++) {
				memcpy(ball + offset, points[i], sizeof(GLfloat) * 3);
				offset += 3;
				memcpy(ball + offset, v3, sizeof(GLfloat) * 3);
				offset += 3;
			}
		}
	}
}

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
    initializeOpenGLFunctions();
    glViewport(0, 0, width(), height());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	init_shaders("shader_p.vert", "shader_p.frag");
	init_vbo(160, 320);
}

/*###################################################
##  函数: init_shaders
##  函数描述： 读取着色器，并初始化
##  参数描述： v_path: 顶点着色器的文件路径, f_path: 片段着色器的文件路径
#####################################################*/
void MyGLWidget::init_shaders(const char* v_path, const char* f_path) {
	//读取着色器
	string v_code, f_code;
	ifstream v_shader_file, f_shader_file;
	v_shader_file.open(v_path);
	f_shader_file.open(f_path);
	stringstream v_shader_stream, f_shader_stream;
	v_shader_stream << v_shader_file.rdbuf();
	f_shader_stream << f_shader_file.rdbuf();
	v_shader_file.close();
	f_shader_file.close();
	v_code = v_shader_stream.str();
	f_code = f_shader_stream.str();
	const char* v_shader_source = v_code.c_str();
	const char* f_shader_source = f_code.c_str();

	// 顶点着色器
	GLuint v_shader;
	v_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(v_shader, 1, &v_shader_source, NULL);
	glCompileShader(v_shader);

	//片段着色器
	GLuint f_shader;
	f_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(f_shader, 1, &f_shader_source, NULL);
	glCompileShader(f_shader);

	//着色器程序
	shader_program = glCreateProgram();
	glAttachShader(shader_program, v_shader);
	glAttachShader(shader_program, f_shader);
	glLinkProgram(shader_program);
	glDeleteShader(v_shader);
	glDeleteShader(f_shader);
	glUseProgram(shader_program);

	//获取并设置投影矩阵
	GLfloat projection_matrix[16];
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLfloat)width() / (GLfloat)height(), 0.1f, 200.0f);
	glGetFloatv(GL_PROJECTION_MATRIX, projection_matrix);
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, projection_matrix);
}

/*###################################################
##  函数: init_vbo
##  函数描述： 初始化vbo,vao
##  参数描述： u_num_, v_num_: 角度拆分数
#####################################################*/
void MyGLWidget::init_vbo(int u_num_, int v_num_) {
	u_num = u_num_, v_num = v_num_;
	//创建球的数组
	GLfloat* ball = new GLfloat[6 * 6 * u_num * v_num];
	create_ball(ball, u_num, v_num, 1.0f);

	//创建VBO，VAO
	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 6 * u_num * v_num, ball, GL_STATIC_DRAW);

	//链接顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}

/*###################################################
##  函数: draw
##  函数描述： 画图
##  参数描述： 无
#####################################################*/
void MyGLWidget::draw(){
	//获取并设置modelview矩阵
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

	static float angle = 60.0f;

	GLfloat modelview_matrix[16];
	glTranslatef(0.0f, 0.0f, -5.0f);
	glRotatef(angle, 1.0f, 1.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "modelview"), 1, GL_FALSE, modelview_matrix);

	glDrawArrays(GL_TRIANGLES, 0, 6 * u_num * v_num);

	angle += 1.0f;
	if (abs(angle - 360.0f) <= 1e-1)angle = 0.0f;

	glPopMatrix();
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
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	draw();
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

