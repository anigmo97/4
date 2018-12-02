/**
*	SEMINARIO6.cpp
*
*   Captura de eventos de usuario en OpenGL
*
*	@author: anigmo@inf.upv.es
*	@version: v1.0 Oct,2018
*	@require: freeglut, UTILIDADES
*
**/

#define PROYECTO "SEMINARIO_6_EJEMPLO_1"

#include <iostream>
#include <gl/freeglut.h>
#include <Utilidades.h>

// Variables globales
static enum { ALAMBRICO, SOLIDO, DOBLE } modoDibujo;
static float giroxTR = 0, giroyTR = 0,giroxTV = 0, giroyTV = 0;
static int xanterior, yanterior;
static bool dragging = false; // variable control 

static GLubyte esRoja, esVerde;


void onMenu(int valor) {
	//Callbak de atencion al menu de contxto
	if (valor == 0) {
		modoDibujo = ALAMBRICO;
	}
	else if (valor == 1) {
		modoDibujo = SOLIDO;
	}
	else {
		modoDibujo = DOBLE;
	}

	glutPostRedisplay();
}

void init() { // solo se ejecuta una vez
	glClearColor(0, 0, 0.3, 1.0); //fijar el color de borrado (fondo)
	glEnable(GL_DEPTH_TEST);//configurar motor de render

	//crear menu
	glutCreateMenu(onMenu);
	glutAddMenuEntry("ALAMBRICO", 0);
	glutAddMenuEntry("SOLIDO", 1);
	glutAddMenuEntry("DOBLE", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // borrar el fondo y profundidad
	/* estos dos comandos hacen que cada display no dismunuya el pealo */
	glMatrixMode(GL_MODELVIEW); // selecciono model-view
	glLoadIdentity(); // carga la identidad en el tope de la pila

	//situar y orientar origen
	gluLookAt(3, 4, 4, 0, 0, 0, 0, 1, 0);


	ejes();

	//tetera 1
	glPushMatrix();
	glTranslatef(0, 0, 0.5);
	glColor3f(1, 0, 0);
	glRotatef(giroxTR, 1, 0, 0); // general escena
	glRotatef(giroyTR, 0, 1, 0); // eje local a tetera
	if (modoDibujo == ALAMBRICO) {
		glutWireTeapot(0.51);
	}
	else if (modoDibujo == SOLIDO) {
		glutSolidTeapot(0.5);

	}
	else {
		glutSolidTeapot(0.5);
		glColor3f(1, 1, 1);
		glutWireTeapot(0.51);
	}
	glPopMatrix();

	//tetera 2
	glPushMatrix();
	glTranslatef(0, 0, -0.4);
	glColor3f(0, 1, 0);
	//ORDEN CAMBIADO
	glRotatef(giroyTV, 0, 1, 0); // eje local a tetera
	glRotatef(giroxTV, 1, 0, 0); // general escena
	if (modoDibujo == ALAMBRICO) {
		glutWireTeapot(0.51);
	}
	else if (modoDibujo == SOLIDO) {
		glutSolidTeapot(0.5);

	}
	else {
		glutSolidTeapot(0.5);
		glColor3f(1, 1, 1);
		glutWireTeapot(0.51);
	}
	glPopMatrix();

	glutSwapBuffers(); // indicar que han acabado las ordenes de dibujo
}

void reshape(GLint w, GLint h) {

	glViewport(0, 0, w, h); //utiliza toda el area de dibujo

	float razon_aspecto = float(w) / float(h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//defino una camara de perspectiva
	gluPerspective(20, razon_aspecto, 0.2, 10);
	// si pongo menor angulo se ve mas grande

}


void onKey(unsigned char tecla, int x, int y) {
	//Callback de atencion al teclado

	switch (tecla) {
	case 'a':
		modoDibujo = ALAMBRICO;
		break;
	case 's':
		modoDibujo = SOLIDO;
		break;
	case 'd':
		modoDibujo = DOBLE;
		break;
	}

	glutPostRedisplay(); // evento de redibujo con los cambios
}

void select() {
	//dibuja en el back_buffer la escena con solo los objetos que interesa seleccionar
	// cada uno de un color, y sin intercambiar con el buffer frontal
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // borrar el fondo y profundidad
	/* estos dos comandos hacen que cada display no dismunuya el pealo */
	glMatrixMode(GL_MODELVIEW); // selecciono model-view
	glLoadIdentity(); // carga la identidad en el tope de la pila

	//situar y orientar origen
	gluLookAt(3, 4, 4, 0, 0, 0, 0, 1, 0);


	
	//tetera 1
	glPushMatrix();
	glTranslatef(0, 0, 0.5);
	glColor3f(1, 0, 0);
	glRotatef(giroxTR, 1, 0, 0); // general escena
	glRotatef(giroyTR, 0, 1, 0); // eje local a tetera
	glutSolidTeapot(0.5);
	glPopMatrix();

	//tetera 2
	glPushMatrix();
	glTranslatef(0, 0, -0.4);
	glColor3f(0, 1, 0);
	//ORDEN CAMBIADO
	glRotatef(giroyTV, 0, 1, 0); // eje local a tetera
	glRotatef(giroxTV, 1, 0, 0); // general escena
	glutSolidTeapot(0.5);
	glPopMatrix();

}

void onClick(int button, int state, int x, int y) {
	// Callback de atencion al click del raton
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		xanterior = x;
		yanterior = y;
		dragging = true;

		//	quiero saber el color de x,y en el backbuffer habiendo dibujado con colores solidos 
		// diferentes las teteras
		// preguntar cuanto mide mi viewport
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		int vy = viewport[3] - y;

		select(); // dibuja en el backbuffer ("no se ve")

		glReadPixels(x, vy, 1, 1, GL_RED, GL_UNSIGNED_BYTE, &esRoja);// x,vy dimensiones, canal a leer, como lo quiero leer
		glReadPixels(x, vy, 1, 1, GL_GREEN, GL_UNSIGNED_BYTE, &esVerde);// x,vy dimensiones, canal a leer, como lo quiero leer
	}
	else {
		dragging = false;
	}
}

void onDrag(int x, int y) {
	//callback de arrastre del raton

	

	if (!dragging) { return; }
	static const float pixel2grados = 1.0;
	
	if (esRoja==0xFF) {
		giroxTR += (x - xanterior )* pixel2grados;
		giroyTR += (y - yanterior) * pixel2grados;
	}
	else if (esVerde==0xFF) {
		giroxTV += (x - xanterior)* pixel2grados;
		giroyTV += (y - yanterior) * pixel2grados;
	}

	xanterior = x;
	yanterior = y;

	glutPostRedisplay();

}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	// Buffers a usar (area de memoria para habilitar)
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // buffer profundidad
	//Posicionar la ventana de dibujo en la pantalla
	glutInitWindowPosition(100, 200);
	// Ajustar tamanyo ventana
	glutInitWindowSize(600, 600);
	//Crear la ventana
	glutCreateWindow(PROYECTO); // Registrar las callbacks (funcion que se dispara cuando sucede algo)
	init();


	glutDisplayFunc(display);// Evento display -> Cuando hay que dibujar
	glutReshapeFunc(reshape);// Evento resize/reshape (cambio ventana)
	glutKeyboardFunc(onKey); // Evento pulsar tecla
	glutMouseFunc(onClick);
	glutMotionFunc(onDrag);


	std::cout << PROYECTO  " en marcha" << std::endl;
	//Poner en marcha el bucle de atencion a eventos
	glutMainLoop();//atiende eventos

}