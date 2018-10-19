/**
*	SEMINARIO3.cpp
*
* Dibuja 
*
*	@author: anigmo@inf.upv.es
*	@version: v1.0 Oct,2018
*	@require: freeglut
*
**/

#define PROYECTO "SEMINARIO_3"

#include <iostream>
#include <gl/freeglut.h>
#include <Utilidades.h>


static GLuint flor, corola, petalo;

void init() { // solo se ejecuta una vez
	glClearColor(0, 0, 0.3, 1.0); //fijar el color de borrado (fondo)
	petalo = glGenLists(1);
	glNewList(petalo, GL_COMPILE);
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix(); // encapsula las transformaciones para que no se apliquen a petalo
	glScalef(0.15,0.5,1); // la z da igual lo que ponga porque no la vemos (la ventana por defecto mide 2 x 2)
	//clic sobre dibujo manda display
	// si cliclo muchas veces se sigue escalando
	glutSolidSphere(1, 20, 20); // radio esfera , numero meriddianos y paralelos
	glPopMatrix();
	glEndList();

	corola = glGenLists(1);
	glNewList(corola, GL_COMPILE);


	// petalos
	for (auto i = 0; i < 12; i++) {
		//hago 12 petalos los giro al final asique lo pongo lo primero
		/*  Tengo que pasar el petalo de su sistema de referencia al del petalo
		Necesito escalar y luego trasladarlo									*/
		// se codifica en orden contrario
		glPushMatrix();
		glRotatef(i*30,0,0,1); // 360 / 12
		glTranslatef(0.00, 0.25, 0.0);
		glScalef(0.5, 0.5, 0.5); // z da igual
		glCallList(petalo);
		glPopMatrix();
	}

	glColor3f(1, 1, 0);
	glPushMatrix();// encapsulo las transformaciones de la corola
	glScalef(0.2, 0.2 ,0.2);
	glutSolidSphere(1, 20, 20);
	glPopMatrix(); // termina la encapsulacion




	glEndList();

	//flor
	flor = glGenLists(1);
	glNewList(flor, GL_COMPILE);
	/* debemos pasar al sistema de referencia de la flor */

	//tallo
	glColor3f(0, 1, 0);
	glPushMatrix(); // encapsulado
	glTranslatef(0.0, -0.125, 0.0); // bajo el tallo hasta abajo ( esta centrado de inicio)
	glScalef(0.05, 0.75, 0.05); // lo escalamos
	glutSolidCube(1);
	glPopMatrix(); // fin encapsulado

	//corola
	/* NO ES NECESARIO ENCAPSULAR PERO ES CONVENIENTE*/
	// SI DIBUJAMOS VARIAS FLORES SI AFECTARÍA ASIQUE LO ENCAPSULAMOS
	glPushMatrix();
	glTranslatef(0.0, 0.25, 0);// la z da igual
	glRotatef(10, 0, 0, 1); // REVISAR se hace para k no caiga recta (MEJORA VISUAL)
	glScalef(0.5, 0.5, 0.5);
	glCallList(corola);
	glPopMatrix();

	glEndList();

	
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT); // borrar el fondo
	/* estos dos comandos hacen que cada display no dismunuya el pealo */
	glMatrixMode(GL_MODELVIEW); // selecciono model-view
	glLoadIdentity(); // carga la identidad en el tope de la pila
	//glCallList(petalo); // llamo a la lista petalo
	//glCallList(corola);
	for (auto i = 0; i < 40; i++) {
		glPushMatrix();
		glTranslatef((rand() % 200 - 100) / 100.0, (rand() % 100 - 50) / 100.0, 0);
		glScalef(0.4, 0.4, 0.4);
		glCallList(flor);
		glPopMatrix();
	}
	
	glFlush(); // indicar que han acabado las ordenes de dibujo
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
	glutInitWindowSize(600, 600);
	//Crear la ventana
	glutCreateWindow(PROYECTO); // Registrar las callbacks (funcion que se dispara cuando sucede algo)
	init();
	
	
	glutDisplayFunc(display);// Evento display -> Cuando hay que dibujar
	glutReshapeFunc(reshape);// Evento resize/reshape (cambio ventana)

	std::cout << PROYECTO  " en marcha" << std::endl;
	//Poner en marcha el bucle de atencion a eventos
	glutMainLoop();//atiende eventos

}