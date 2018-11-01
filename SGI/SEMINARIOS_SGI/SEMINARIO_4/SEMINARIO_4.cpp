/**
*	SEMINARIO4.cpp
*
*   CAMBIOS CAMARA
*
*	@author: anigmo@inf.upv.es
*	@version: v1.0 Oct,2018
*	@require: freeglut, UTILIDADES
*
**/

#define PROYECTO "SEMINARIO_4"

#include <iostream>
#include <gl/freeglut.h>
#include <Utilidades.h>



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
	//gluLookAt(0, 0, 0, -1, 0, 0, 0, 1, 0); ver de cerca
	//gluLookAt(8, 0, 0, -1, 0, 0, 0, 1, 0);  // si reduzco se ve mas cerca
	//gluLookAt(0, 0, 0, -1, 0, 0, 0, 1, 0);
	gluLookAt(3, 4, 4, 0, 0, 0, 0, 1, 0);


	ejes();

	glPushMatrix();
		glTranslatef(0, 0, 0.5);
		glColor3f(1, 0, 0);
		glutSolidTeapot(0.5);
		glColor3f(1, 1, 1);
		glutWireTeapot(0.51);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0, 0, -0.2);
		glColor3f(0, 1, 0);
		glutSolidTeapot(0.5);
		glColor3f(1, 1, 1);
		glutWireTeapot(0.51);
	glPopMatrix();

	glFlush(); // indicar que han acabado las ordenes de dibujo
}

void reshape(GLint w, GLint h) {

	glViewport(0, 0, w, h); //utiliza toda el area de dibujo

	float razon_aspecto = float(w) / float(h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//defino la camara ortografica
	//if (razon_aspecto > 1) { //para que la imagen siempre mantenga su relacion aspecto
	//	//hay que ampliar el ancho
	//	glOrtho(-2 * razon_aspecto, 2 * razon_aspecto, -2, 2, -2, 2);
	//}
	//else {
	//	glOrtho(-2, 2, -2 / razon_aspecto, 2 / razon_aspecto, -2, 2);
	//}
	//glOrtho(-2, 2, -2, 2, -2, 2);

	//defino una camara de perspectiva
	gluPerspective(20, razon_aspecto, 0.2, 10);
	// si pongo menor angulo se ve mas grande
	

}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	// Buffers a usar (area de memoria para habilitar)
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); // buffer profundidad
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