#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

GLuint VBO;
static float scale = 0.0f;
using namespace std;

glm::mat4x4 RotMat(float RotateX, float RotateY, float RotateZ)
{
	float x = glm::radians(RotateX);
	float y = glm::radians(RotateY);
	float z = glm::radians(RotateZ);

	glm::mat4x4 rx(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosf(x), -sinf(x), 0.0f,
		0.0f, sinf(x), cosf(x), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4x4 ry(cosf(y), 0.0f, -sinf(x), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sinf(x), 0.0f, cosf(y), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4x4 rz(cosf(z), -sinf(z), 0.0f, 0.0f,
		sinf(z), cosf(z), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return(rz * ry * rx);
}


class Pipeline
{
public:
	Pipeline()
	{
		m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
		m_worldPos = glm::vec3(0.0f, 0.0f, 0.0f);
		m_rotateInfo = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	void Scale(float ScaleX, float ScaleY, float ScaleZ)
	{
		m_scale.x = ScaleX;
		m_scale.y = ScaleY;
		m_scale.z = ScaleZ;
	}

	void WorldPos(float x, float y, float z)
	{
		m_worldPos.x = x;
		m_worldPos.y = y;
		m_worldPos.z = z;
	}

	void Rotate(float RotateX, float RotateY, float RotateZ)
	{
		m_rotateInfo.x = RotateX;
		m_rotateInfo.y = RotateY;
		m_rotateInfo.z = RotateZ;
	}

	const glm::mat4x4* GetTrans()
	{
		glm::mat4x4 TranslationTrans(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			m_worldPos.x, m_worldPos.y, m_worldPos.z, 1.0f);
		glm::mat4 RotateTrans = RotMat(m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z);
		glm::mat4 ScaleTrans(m_scale.x, 0.0f, 0.0f, 0.0f,
			0.0f, m_scale.x, 0.0f, 0.0f,
			0.0f, 0.0f, m_scale.x, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
		glm::mat4 ProjectionMatrix = InitPerspectiveProj();
		m_transformation = ProjectionMatrix * TranslationTrans * RotateTrans * ScaleTrans;
		return &m_transformation;
	}
	glm::mat4x4 InitPerspectiveProj() const
	{
		const float ar = m_persProj.Width / m_persProj.Height;
		const float zNear = m_persProj.zNear;
		const float zFar = m_persProj.zFar;
		const float zRange = zNear - zFar;
		const float tanHalfFOV = tanf(glm::radians(m_persProj.FOV / 2.0));
		glm::mat4 ProjMat(1.0f / (tanHalfFOV * ar), 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f / tanHalfFOV, 0.0f, 0.0f,
			0.0f, 0.0f, (-zNear - zFar) / zRange, 2.0f * zFar * zNear / zRange,
			0.0f, 0.0f, 1.0f, 0.0f);
		return(ProjMat);
	}
	void SetPerspectiveProj(float FOV, float Width, float Height, float zNear, float zFar)
	{
		m_persProj.FOV = FOV;
		m_persProj.Width = Width;
		m_persProj.Height = Height;
		m_persProj.zNear = zNear;
		m_persProj.zFar = zFar;
	}
private:
	glm::vec3 m_scale;
	glm::vec3 m_worldPos;
	glm::vec3 m_rotateInfo;
	glm::mat4x4 m_transformation;
	struct {
		float FOV;
		float Width;
		float Height;
		float zNear;
		float zFar;
	} m_persProj;
};

void RenderSceneCB()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //устанавливаем цвет
	glClear(GL_COLOR_BUFFER_BIT); //очистка буфера кадра
	glEnableVertexAttribArray(0);
	scale += 0.015f;
	Pipeline p;
	p.Scale(sinf(scale * 0.1f), sinf(scale * 0.1f), sinf(scale * 0.1f));
	p.WorldPos(0.0f, 0.0f, sinf(scale));
	p.Rotate(sinf(scale) * 90.0f, sinf(scale) * 90.0f, sinf(scale) * 90.0f);
	p.SetPerspectiveProj(10.0f, WINDOW_WIDTH, WINDOW_HEIGHT, -300.0f, 100.0f);
	glLoadMatrixf(reinterpret_cast<const float*>(p.GetTrans()));


	glDrawArrays(GL_LINE_LOOP, 0, 3);
	glDisableVertexAttribArray(0);
	glutSwapBuffers(); // меняем фоновый буфер и буфер кадра местами
	glutIdleFunc(RenderSceneCB);

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv); // инициализация окна

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //установка режима отображения
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); //задаем размер, позицию окна, даем название
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Laba 2");

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	glMatrixMode(GL_MODELVIEW);
	glm::vec3 Vertices[3] = { { 0.5f, 0.5f, 0.0f }, { -0.5f, 0.5f, 0.0f }, { 0.0f, -0.5f, 0.0f } };


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	glutDisplayFunc(RenderSceneCB); //работа в самой оконной системе
	glutMainLoop(); //зацикливаем и вызываем функцию отображения окна на экран

	return 0;
}