/**
*	PRACTICA1.cpp
*	@author: anigmo@inf.upv.es
*	@version: v1.0 Oct,2018
*	@require: freeglut
*
**/

#define PROYECTO "PRACTICA_1"

#include <iostream>
#include <gl/freeglut.h>


void display() {
	//fijar el color de borrado (fondo)
	glClearColor(0.0, 0.0, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	// indicar que han acabado las ordenes de dibujo
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
	glutInitWindowPosition(50, 600);
	// Ajustar tamanyo ventana
	glutInitWindowSize(500, 400);
	//Crear la ventana
	glutCreateWindow("Primer programa en OpenGL");
	// Registrar las callbacks (funcion que se dispara cuando sucede algo)
	// Evento display -> Cuando hay que dibujar
	// Evento resize/reshape (cambio ventana)
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	std::cout << PROYECTO  " en marcha" << std::endl;
	//Poner en marcha el bucle de atencion a eventos
	glutMainLoop();//atiende eventos

}