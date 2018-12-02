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
static float girox = 0, giroy = 0;
static int xanterior, yanterior;

void init() { // solo se ejecuta una vez
	glClearColor(0, 0, 0.3, 1.0); //fijar el color de borrado (fondo)
	glEnable(GL_DEPTH_TEST);//configurar motor de render
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
	glRotatef(girox, 1, 0, 0); // general escena
	glRotatef(giroy, 0, 1, 0); // eje local a tetera
	if (modoDibujo == ALAMBRICO) {
		glutWireTeapot(0.5);
	}
	else if (modoDibujo == SOLIDO) {
		glutSolidTeapot(0.5);

	}
	else {
		glutSolidTeapot(0.5);
		glColor3f(1, 1, 1);
		glutWireTeapot(0.5);
	}
	glPopMatrix();

	//tetera 2
	glPushMatrix();
	glTranslatef(0, 0, -0.2);
	glColor3f(0, 1, 0);
	//ORDEN CAMBIADO
	glRotatef(giroy, 0, 1, 0); // eje local a tetera
	glRotatef(girox, 1, 0, 0); // general escena
	if (modoDibujo == ALAMBRICO) {
		glutWireTeapot(0.5);
	}
	else if (modoDibujo == SOLIDO) {
		glutSolidTeapot(0.5);

	}
	else {
		glutSolidTeapot(0.5);
		glColor3f(1, 1, 1);
		glutWireTeapot(0.5);
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


void onKey(unsigned char tecla, int x, int i) {
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

void onClick(int button, int state, int x, int y) {
	// Callback de atencion al click del raton
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		xanterior = x;
		yanterior = y;
	}
}

void onDrag(int x, int y) {
	//callback de arrastre del raton
	static const float pixel2grados = 1.0;
	// al mover el raton hacia la derecha la x aumenta y el giro es positivo alrededor de y
	giroy += (x - xanterior) * pixel2grados;
	//al mover el raton hacia abajo la y aumenta y el giro es positivo alrededor de x
	girox += (y - yanterior)*pixel2grados;

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

//TODO HACER MODO DRAGING TRUE FALSE
//SOLO SE HACE DRAG SI DRAGGIN == TRUE