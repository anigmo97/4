/**
*	SEMINARIO_5_EJEMPLO_4.cpp
*
*   REALIZA ANIMACIONES EN FUNCION DEL TIEMPO ( LA VELOCIDAD ES IGUAL EN TODOS LOS PCS )
*   REALIZAR UN NUMERO DE FRAMES CONSTANTE CONTROLADO
*   ELIMINAMOS EL EVENTO IDLE PARA GENERAR NUESTROS EVENTOS CON CONTADORES
*
*	@author: anigmo@inf.upv.es
*	@version: v1.0 Nov,2018
*	@require: freeglut, UTILIDADES
*
**/

#define PROYECTO "SEMINARIO_5_EJEMPLO_4 FPS CONSTANTES"

#include <iostream>
#include <gl/freeglut.h>
#include <Utilidades.h>
#include <sstream>

// Variables globales
static float alfa = 0;

void init() { // solo se ejecuta una vez
	glClearColor(0, 0, 0.3, 1.0); //fijar el color de borrado (fondo)
	glEnable(GL_DEPTH_TEST);//configurar motor de render
}

void FPS() {
	// cuenta fotogramas hasta que pasa 1 segundo
	// entonces los muestra y reinicia la cuenta

	int ahora, tiempo_transcurrido;
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	static int fotogramas = 0;
	// Cada vez que paso sumo un fotograma
	fotogramas++;

	//Calcular tiempo transcurrido 
	ahora = glutGet(GLUT_ELAPSED_TIME);
	tiempo_transcurrido = ahora - antes;

	// si ha transcurrido mas de un segundo muestro y reinicio
	if (tiempo_transcurrido > 1000) {
		stringstream titulo;
		titulo << "FPS = " << fotogramas;
		glutSetWindowTitle(titulo.str().c_str());
		fotogramas = 0;
		antes = ahora;
	}

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

	FPS(); // duncion cuentas las veces k entra en display en 1 segundo
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

void update() {
	//Fase de actualiacion
	// control del tiempo
	static const float vueltas_por_segundo = 2; //vueltas por segundo

	// solo calculo una vez la hora anterior inicial
	static int antes = glutGet(GLUT_ELAPSED_TIME); // tiempo en ms

	// HORA actual
	int ahora = glutGet(GLUT_ELAPSED_TIME);

	//tiempo transcurrido en segundos
	float tiempo_transcurrido = (float)(ahora - antes) / 1000.0;

	//incremento = velocidad * tiempo 
	// angulo_alfa
	alfa += 360 * vueltas_por_segundo*tiempo_transcurrido; // 360 grados * vueltas_seg * tiempo
	//Actualizar para la proxima vez
	antes = ahora;


	// mandar evento redibujo
	glutPostRedisplay(); // redibujar
}

void onTimer(int tiempo) {
	//Callback de atencion a la cuenta atras
	update();

	//encolo un nuevo timer
	glutTimerFunc(tiempo, onTimer,tiempo);
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
	//glutIdleFunc(update); // envento fase actulizacion
	glutTimerFunc(1000 / 50, onTimer, 1000 / 50); // cada 20 ms

	std::cout << PROYECTO  " en marcha" << std::endl;
	//Poner en marcha el bucle de atencion a eventos
	glutMainLoop();//atiende eventos

}