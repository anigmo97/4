/**
*	SEMINARIO2.cpp
*	@author: anigmo@inf.upv.es
*	@version: v1.0 Oct,2018
*	@require: freeglut, utilidades.h
*
**/

#define PROYECTO "SEMINARIO_2 EJEMPLO 1"

#include <iostream>
#include <gl/freeglut.h>
//podemos cambiar por Utilidades.h en lugar de freeglut (utilidades tiene mas funcionalidades)
#include <Utilidades.h>


GLuint pentagono;


void init(){
	// solo se ejecuta una vez
	glClearColor(0, 0, 0.3, 1);

	pentagono = glGenLists(1);

	glNewList(pentagono, GL_COMPILE);
	//como ira un poligono dentro de mi lista:
	glBegin(GL_POLYGON);
	for (auto i = 0; i < 5; i++) {
		//radio circunferencia
		glVertex3f((0.8 * cos(i * 2 * PI / 5)), (0.8 * sin(i * 2 * PI / 5)), 0);
	}
	glEnd();
	glEndList();
}

void display() {
	/*
	*
	Ejemplo dibujar tetera (con marco no cuadrado se hace deforme)
			glClear(GL_COLOR_BUFFER_BIT);
			glutWireTeapot(0.5);
			glFlush();
	*
	*/

	/* Ejemplo dibujar poligono 
	*
			glClear(GL_COLOR_BUFFER_BIT);
			glBegin(GL_POLYGON);
				glVertex2f(-1.0, -1.0);
				glVertex2f(0.0, -1.0);
				glVertex2f(1.0, 0.0);
				glVertex2f(0.0, 1.0);
				glVertex2f(-1.0, 1.0);
			glEnd();
			glFlush();
	*
	*/

	// INTERESANTE 
	//carreteras en sentido contrario a las agujas del reloj en el proyecto final
	// saber si estoy dibujando la cara de delante o la de detras glPoligonMode -> cada cara de una forma
	// depurar con ella

	//Al hacer triangulos tambien se hace en sentido antihorario

	/* EJEMPLO PROFESOR SEMINARIO 2*/
	// DIBUJAR 3 PENTAGONOS, 1 SOLIDO , 1 ARISTAS 1 SOLO PUNTOS

	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT, GL_FILL);
	glColor3f(1, 1, 0);
	glCallList(pentagono);

	glPolygonMode(GL_FRONT, GL_LINE);
	glColor3f(1, 0, 0);
	glLineWidth(4);
	glCallList(pentagono);

	glPolygonMode(GL_FRONT, GL_POINT);
	glColor3f(0, 1, 0);
	glPointSize(10);
	glCallList(pentagono);


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
	glutInitWindowSize(600, 400);
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