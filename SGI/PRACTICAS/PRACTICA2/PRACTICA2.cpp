/**
*	PRACTICA2.cpp
*	@author: anigmo@inf.upv.es
*	@version: v1.0 Oct,2018
*	@require: freeglut
*
**/

#define PROYECTO "PRACTICA_2 ESTRELLA DE DAVID" // Lo usamos para mostrar un mensaje

#include <iostream>
#include <gl/freeglut.h>
#include <Utilidades.h> //podemos cambiar freeglut por Utilidades.h (utilidades incluye freeglut y mas)


GLint id; // int en openGL (ajusta su tamaño en bits y int no)

/* este metodo solo se ejecuta una vez, los calculos y las creaciones
	   de poligonos deberían hacerse aqui en lugar de en display() */
void init() {
	
	id = glGenLists(1); // le pasamos el numero de listas contiguas que queremos y nos retorna un int
	/* A partir del nombre de la lista de dibujo y el modo de compilacion creamos la lista de dibujo
	{ GL_COMPILE -> solo instrucciones a compilar  GL_COMPILE_AND_EXECUTE -> insts. a compilar y ejecutar }   */
	glNewList(id, GL_COMPILE); 
	glPushAttrib(GL_CURRENT_BIT); // Encapsulamos o apilamos estado actual diciendole que queremos que guarde
	/* GL_CURRENT_BIT guardaría:
		Current RGBA color
		Current color index
		Current normal vector
		Current texture coordinates
		Current raster position
		GL_CURRENT_RASTER_POSITION_VALID flag
		RGBA color associated with current raster position
		Color index associated with current raster position
		Texture coordinates associated with current raster position
		GL_EDGE_FLAG flag
		*/
	
	glColor3f(0.0, 0.0, 0.3); // fijamos el color en el que dibujaremos
	
	double angulo;
	/*CREA TRIANGULO HACIA ARRIBA */
	glBegin(GL_TRIANGLE_STRIP); //iniciamos un triangle_strip
	for (int i = 0; i < 4; i++) {
		angulo = i * (2 * PI / 3) + (PI / 2);
		/* AÑADIMOS VERTICES AL TRIANGLE_STRIP*/
		glVertex3f(1.0 * cos(angulo), 1.0 * sin(angulo), 0.0); 
		glVertex3f(0.7 * cos(angulo), 0.7 * sin(angulo), 0.0);
	}
	glEnd(); //Terminamos Triangle_Strip
	/*CREA TRIANGULO HACIA ABAJO*/
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i < 4; i++) {
		angulo = i * (2 * PI / 3) + (3 * PI / 2);
		glVertex3f(1.0 * cos(angulo), 1.0 * sin(angulo), 0.0);
		glVertex3f(0.7 * cos(angulo), 0.7 * sin(angulo), 0.0);
	}
	glEnd();

	glPopAttrib(); // desapilamos estado 
	glEndList(); // terminamos lista de dibujo
}

void display() {	
	glClearColor(1, 1, 1, 1);   //fijar el color de borrado (fondo)
	glClear(GL_COLOR_BUFFER_BIT); // Hace el borrado ( pone el fondo )
	/* ESTABLECER EL MODO DE DIBUJO DE LOS POLIGONOS  (por defecto es GL_FILL en FRONT y BACK asique no hace falta) 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    // { GL_LINE | GL_POINT | GL_FILL }     
	*/  
	glCallList(id); // llamamos a la lista de dibujo y se ejecutan sus acciones
	glFlush();    // indicar que han acabado las ordenes de dibujo
}

void reshape(int w, int h) {
	// NO ES NECESARIA EN ESTA PRACTICA
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv); // Reserva Buffers a usar (area de memoria para habilitar)
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // solo un buffer y sera de rgb calda celda es rojo verde y azul
	glutInitWindowPosition(100, 200); //Posicionar la ventana de dibujo en la pantalla
	glutInitWindowSize(400, 400); // Ajustar tamanyo ventana
	glutCreateWindow(PROYECTO); //Crear la ventana

	/*Registro de las callbacks (funcion que se dispara cuando sucede algo)*/
	glutDisplayFunc(display); // Evento display -> Cuando hay que dibujar
	glutReshapeFunc(reshape); // Evento resize/reshape (cambio ventana (dimension etc)) NO NECESARIO

	init(); // el init hace los calculos y los ajustes antes de dibujar
	std::cout << PROYECTO  " en marcha" << std::endl; // print por la salida estandar
	glutMainLoop(); //Poner en marcha el bucle de atencion a eventos
}