/**
*	SEMINARIO_5_EJEMPLO_2.cpp
*
*   ANIMACIONES
*	REALIZA ANIMACIONES EN FUNCION DEL TIEMPO ( LA VELOCIDAD ES IGUAL EN TODOS LOS PCS )
*
*	@author: anigmo@inf.upv.es
*	@version: v1.0 Nov,2018
*	@require: freeglut, UTILIDADES
*
**/

#define PROYECTO "SEMINARIO_5_EJEMPLO_2"

#include <iostream>
#include <gl/freeglut.h>
#include <Utilidades.h>

// Variables globales
static float alfa = 0;

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

	//tetera 1
	glPushMatrix();
	glTranslatef(0, 0, 0.5);
	glColor3f(1, 0, 0);
	glRotatef(alfa, 0, 1, 0);
	glutSolidTeapot(0.5);
	glColor3f(1, 1, 1);
	glutWireTeapot(0.51);
	glPopMatrix();

	//tetera 2
	glPushMatrix();
	glTranslatef(0, 0, -0.2);
	glColor3f(0, 1, 0);
	glRotatef(alfa / 2, 0, 1, 0);
	glutSolidTeapot(0.5);
	glColor3f(1, 1, 1);
	glutWireTeapot(0.51);
	glPopMatrix();

	glutSwapBuffers(); // indicar que han acabado las ordenes de dibujo
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

void update() {
	//Fase de actualiacion
	// sin control tiempo
	//alfa += 0.1; // la utilizo en update y display --> sera global
	// control del tiempo
	static const float omega = 2; //vueltas por segundo

	// solo calculo una vez la hora anterior inicial
	static int antes = glutGet(GLUT_ELAPSED_TIME); // tiempo en ms

	// HORA actual
	int ahora = glutGet(GLUT_ELAPSED_TIME);

	//tiempo transcurrido en segundos
	float tiempo_transcurrido = (float)(ahora - antes) / 1000.0;

	//incremento = velocidad * tiempo 
	// angulo_alfa
	alfa += 360 * omega*tiempo_transcurrido; // 360 grados * vueltas_seg * tiempo
	//Actualizar para la proxima vez
	antes = ahora;


	// mandar evento redibujo
	glutPostRedisplay(); // redibujar
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
	glutIdleFunc(update); // envento fase actulizacion

	std::cout << PROYECTO  " en marcha" << std::endl;
	//Poner en marcha el bucle de atencion a eventos
	glutMainLoop();//atiende eventos

}