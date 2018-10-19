/**
*	PRATICA_3.cpp
*
*	Dibuja 4 estrellas dobles
*
*	@author: anigmo@inf.upv.es
*	@version: v1.0 Oct,2018
*	@require: freeglut
*
**/

#define PROYECTO "PRACTICA_3"

#include <iostream>
#include <gl/freeglut.h>
#include <Utilidades.h>


static GLuint estrella,estrella_doble;

void init() { // solo se ejecuta una vez y calcula los vertices, objetos etc
	glClearColor(1, 1, 1, 1);   //fijar el color de borrado (fondo)

	/* LISTA DE DIBUJO ESTRELLA */
	estrella = glGenLists(1); // le pasamos el numero de listas contiguas que queremos y nos retorna un int
			glNewList(estrella, GL_COMPILE);// le decimos que solo tiene que compilar la lista
			glPushAttrib(GL_CURRENT_BIT); // Encapsulamos o apilamos estado actual diciendole que queremos que guarde
				glColor3f(0.0, 0.0, 0.3); // fijamos el color en el que dibujaremos (hasta el popAtrib ó otro glColor)
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
					//glColor3f(0.0, 1.0, 0.3); // cambiamos el color del triángulo hacia abajo
					glBegin(GL_TRIANGLE_STRIP);
					for (int i = 0; i < 4; i++) {
						angulo = i * (2 * PI / 3) + (3 * PI / 2);
						//glColor3f(0.0, 0.0, 0.3);//pintar azules los vértices externos
						glVertex3f(1.0 * cos(angulo), 1.0 * sin(angulo), 0.0);
						//glColor3f(0.0, 1.0, 0.3); //pintar verdes los vertices internos
						glVertex3f(0.7 * cos(angulo), 0.7 * sin(angulo), 0.0);
					}
					glEnd();
			glPopAttrib(); // desapilamos estado 
	glEndList(); // terminamos lista de dibujo



	/* LISTA DE DIBUJO ESTRELLA DOBLE
		Lo que haremos es llamar dos veces a estrella, una será la interna (la debemos transformar) y otra la externa
	*/ 
	estrella_doble = glGenLists(1); 
	glNewList(estrella_doble, GL_COMPILE);
		//ESTRELLA INTERNA
			glPushMatrix(); // encapsulamos las transformaciones para que solo se apliquen a esta estrella
			glRotatef(360/12,0,0,1); // la rotamos
				glScalef(0.4, 0.4, 0.05); // lo escalamos
				glCallList(estrella); // llamamos/creamos una estrella
				//ORDEN DE EJECUCION ES INVERSO AL CODIGO -> SE CREA , SE ESCALA Y SE ROTA
			glPopMatrix(); // fin encapsulado

		// ESTRELLA EXTERNA
			/* NO SE APLICAN TRANSFORMACIONES --> NO SE NECESITA ENCAPSULAR*/
			glCallList(estrella);// llamamos/creamos una estrella 

	glEndList();


}


void display() {
	glClear(GL_COLOR_BUFFER_BIT); // borrar el fondo
	/* estos dos comandos hacen que cada display no dismunuya el tamaño al transformar */
	glMatrixMode(GL_MODELVIEW); // selecciono model-view
	glLoadIdentity(); // carga la identidad en el tope de la pila
	int posiciones[4][2] = { {-1, 1}, {+1, +1}, {-1, -1}, {+1, -1} }; // TOP LEFT , TOP RIGHT, BOTTOM LEFT, BOTTOM RIGHT
	int angulos[4] = { 15,-15,-15,15 }; // +15 grados sentido antihorario...
	for (auto i = 0; i < 4; i++) {
		glPushMatrix();
		glTranslatef(0.5*posiciones[i][0],0.5*posiciones[i][1],0);
		glScalef(0.5, 0.5, 0);
		glRotatef(angulos[i], 0, 0, 1); 
		glCallList(estrella_doble); // llamamos a la lista de dibujo y se ejecutan sus acciones
		glPopMatrix();
	}
	
	glFlush();    // indicar que han acabado las ordenes de dibujo
}

void reshape(int w, int h) {

}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	// Buffers a usar (area de memoria para habilitar)
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // solo un buffer y sera de rgb calda celda es rojo verde y azul
	glutInitWindowPosition(100, 200);//Posicionar la ventana de dibujo en la pantalla
	glutInitWindowSize(600, 600);// Ajustar tamanyo ventana
	glutCreateWindow(PROYECTO); //Crear la ventana
	init();
	/* Registrar las callbacks (funcion que se dispara cuando sucede algo)*/
	glutDisplayFunc(display);// Evento display -> Cuando hay que dibujar
	glutReshapeFunc(reshape);// Evento resize/reshape (cambio ventana) NO NECESARIO AQUI
	std::cout << PROYECTO  " en marcha" << std::endl;
	glutMainLoop();//Poner en marcha el bucle de atencion a eventos

}