/**
*	AJUSTAR_VENTANA.cpp
*	@author: anigmo@inf.upv.es
*	@version: v1.0 Oct,2018
*	@require: freeglut
*
**/

#define PROYECTO "AJUSTAR VENTANA " // Lo usamos para mostrar un mensaje

#include <iostream>
#include <gl/freeglut.h>
#include <Utilidades.h> //podemos cambiar freeglut por Utilidades.h (utilidades incluye freeglut y mas)

float x;
float y;
float z;


/* este metodo solo se ejecuta una vez, los calculos y las creaciones
	   de poligonos deberían hacerse aqui en lugar de en display() */
void init() {

	glClearColor(1, 1, 1, 1);  //fijar el color de borrado (fondo)
	glColor3f(1, 0, 0); //fijar el color de dibujo
	glEnable(GL_DEPTH_TEST);//configurar motor de render
	// Desde aqui doy valores iniciales de la cámara. ( elegidos aleatoriamente )
	x = 0;
	y = 0;
	z = 3;
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // borrar el fondo y profundidad
	glMatrixMode(GL_MODELVIEW); // selecciono model-view
	glLoadIdentity(); // carga la identidad en el tope de la pila

	// Situar y orientar la camara: punto de vista, punto de interes, vertical
	gluLookAt(x, y, z, 0, 0, 0, 0, 1, 0);

	glutWireSphere(1, 20, 20);
	glFlush();    // indicar que han acabado las ordenes de dibujo
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h); //utiliza toda el area de dibujo
	float razon_aspecto = float(w) / float(h);

	float apertura;
	float distancia;

	// Matriz de la proyeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* HACER QUE LA ESFERA SE AJUSTE AL TAMAÑO DE LA VENTANA (TOQUE ARRIBA Y ABAJO)  */
	distancia = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	apertura = asin(1 / distancia) * 2;
	apertura = (180 / PI) * apertura;
	gluPerspective(apertura, razon_aspecto, 0.1, 20); //defino una camara de perspectiva
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv); // Reserva Buffers a usar (area de memoria para habilitar)
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); // un buffer , buffer color y buffer profundidad
	glutInitWindowSize(400, 400); // Ajustar tamanyo ventana
	glutCreateWindow(PROYECTO); //Crear la ventana

	/*Registro de las callbacks (funcion que se dispara cuando sucede algo)*/
	glutDisplayFunc(display); // Evento display -> Cuando hay que dibujar
	glutReshapeFunc(reshape); // Evento resize/reshape (cambio ventana (dimension etc)) NO NECESARIO

	init(); // el init hace los calculos y los ajustes antes de dibujar
	glutMainLoop(); //Poner en marcha el bucle de atencion a eventos
}