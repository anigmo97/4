/**
*	PRACTICA2.cpp
*	@author: anigmo@inf.upv.es
*	@version: v1.0 Oct,2018
*	@require: freeglut
*
**/

#define PROYECTO "PRACTICA_2"

#include <iostream>
#include <gl/freeglut.h>

//podemos cambiar por Utilidades.h en lugar de freeglut (utilidades tiene mas funcionalidades)
#include <Utilidades.h>

GLuint indices[8] = { 0,1,2,3,4,5,0,1 };
GLfloat x;
GLfloat y;
GLfloat radio;
GLint id;

void init() {
	// solo se ejecuta una vez
	glClearColor(1, 1, 1, 1);
	id = glGenLists(1);
	glNewList(id, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(0.0, 0.0, 0.3);
	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f((1 * cos(0 * 2 * PI / 3 + PI / 2)), (1 * sin(0 * 2 * PI / 3 + PI / 2)), 0);
		glVertex3f((0.7 * cos(0 * 2 * PI / 3 + PI / 2)), (0.7 * sin(0 * 2 * PI / 3 + PI / 2)), 0);
		glVertex3f((1 * cos(1 * 2 * PI / 3 + PI / 2)), (1 * sin(1 * 2 * PI / 3 + PI / 2)), 0);
		glVertex3f((0.7 * cos(1 * 2 * PI / 3 + PI / 2)), (0.7 * sin(1 * 2 * PI / 3 + PI / 2)), 0);
		glVertex3f((1 * cos(2 * 2 * PI / 3 + PI / 2)), (1 * sin(2 * 2 * PI / 3 + PI / 2)), 0);
		glVertex3f((0.7 * cos(2 * 2 * PI / 3 + PI / 2)), (0.7 * sin(2 * 2 * PI / 3 + PI / 2)), 0);
		glVertex3f((1 * cos(0 * 2 * PI / 3 + PI / 2)), (1 * sin(0 * 2 * PI / 3 + PI / 2)), 0);
		glVertex3f((0.7 * cos(0 * 2 * PI / 3 + PI / 2)), (0.7 * sin(0 * 2 * PI / 3 + PI / 2)), 0);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f((1 * cos(0 * 2 * PI / 3 + 3*PI / 2)), (1 * sin(0 * 2 * PI / 3 + 3*PI / 2)), 0);
	glVertex3f((0.7 * cos(0 * 2 * PI / 3 + 3*PI / 2)), (0.7 * sin(0 * 2 * PI / 3 + 3*PI / 2)), 0);
	glVertex3f((1 * cos(1 * 2 * PI / 3 + 3*PI / 2)), (1 * sin(1 * 2 * PI / 3 + 3*PI / 2)), 0);
	glVertex3f((0.7 * cos(1 * 2 * PI / 3 + 3*PI / 2)), (0.7 * sin(1 * 2 * PI / 3 + 3*PI / 2)), 0);
	glVertex3f((1 * cos(2 * 2 * PI / 3 + 3*PI / 2)), (1 * sin(2 * 2 * PI / 3 + 3*PI / 2)), 0);
	glVertex3f((0.7 * cos(2 * 2 * PI / 3 + 3*PI / 2)), (0.7 * sin(2 * 2 * PI / 3 + 3*PI / 2)), 0);
	glVertex3f((1 * cos(0 * 2 * PI / 3 + 3*PI / 2)), (1 * sin(0 * 2 * PI / 3 + 3*PI / 2)), 0);
	glVertex3f((0.7 * cos(0 * 2 * PI / 3 + 3*PI / 2)), (0.7 * sin(0 * 2 * PI / 3 + 3*PI / 2)), 0);
	glEnd();
	glPopAttrib();
	glEndList();
	


}

void display() {

	
	/*for (auto i = 0; i < 6; i++) {
		if (i % 2 == 0) { radio = 1; }
		else { radio = 0.7; }
		x = (radio* cos(i * 2 * PI / 3 + PI / 2));
		y = (radio * sin(i * 2 * PI / 3 + PI / 2));
		glVertex3f(x, y, 0);
	}*/
	
	
	
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCallList(id);
	glFlush();

	
	glFlush();
}

void reshape(int w, int h) {

}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	// Buffers a usar (area de memoria para habilitar)
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // solo un buffer y sera de rgb calda celda es rojo verde y azul
	//Posicionar la ventana de dibujo en la pantalla
	glutInitWindowPosition(100, 200);
	// Ajustar tamanyo ventana
	glutInitWindowSize(400, 400);
	//Crear la ventana
	glutCreateWindow(PROYECTO);
	// Registrar las callbacks (funcion que se dispara cuando sucede algo)
	// Evento display -> Cuando hay que dibujar
	// Evento resize/reshape (cambio ventana)
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	init();

	std::cout << PROYECTO  " en marcha" << std::endl;
	//Poner en marcha el bucle de atencion a eventos
	glutMainLoop();//atiende eventos

}