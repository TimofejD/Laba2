#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

GLuint VBO;
static float scale = 0.0f;
using namespace std;

void RenderSceneCB()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //устанавливаем цвет
	glClear(GL_COLOR_BUFFER_BIT); //очистка буфера кадра
	glEnableVertexAttribArray(0);
	scale += 0.0015f;
	glm::mat4x4 transformMatrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		sinf(scale), 0.0f, 0.0f, 1.0f
	);
	glm::mat4 RotateMatrixZ(cosf(scale), sinf(scale), 0.0f, 0.0f,
							-sinf(scale), cosf(scale), 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0, 0.0f, 0.0f, 1.0f);
	glm::mat4 RotateMatrixY(cosf(scale), sinf(scale), 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							-sinf(scale), cosf(scale), 1.0f, 0.0f,
							0, 0.0f, 0.0f, 1.0f);
	glm::mat4 RotateMatrixX(1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, cosf(scale), -sinf(scale), 0.0f,
							0.0f, sinf(scale), cosf(scale), 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f);

	glm::mat4x4 Matrix = transformMatrix /** RotateMatrixZ * RotateMatrixY * RotateMatrixX*/;

	glLoadMatrixf(reinterpret_cast<const float*>(&Matrix));


	glDrawArrays(GL_LINE_LOOP, 0, 3);
	glDisableVertexAttribArray(0);
	glutSwapBuffers(); // меняем фоновый буфер и буфер кадра местами
	glutIdleFunc(RenderSceneCB);

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv); // инициализация окна

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //установка режима отображения
	glutInitWindowSize(1024, 768); //задаем размер, позицию окна, даем название
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