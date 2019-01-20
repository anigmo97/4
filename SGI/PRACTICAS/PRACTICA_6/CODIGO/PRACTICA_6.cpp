/**
*	PRACTICA6.cpp
*	@author: anigmo@inf.upv.es
*	@version: v1.0 Nov,2018
*	@require: freeglut
*
**/

#define PROYECTO "PRACTICA_6 CARRETERA" // Lo usamos para mostrar un mensaje

#include "CARRETERA.h" // calculo y dibujo carretera
#include "Global.h"    // includes


/* POSICION INICIAL CAMARA */
float x;
float y;
float z;
/* POSICION A LA QUE MIRO */
float x_a_la_que_miro = 10;
float z_a_la_que_miro = 0;
/*VECTOR VELOCIDAD*/
float vector_velocidad[3] = { 0,0,0 };
/*MODULO VELOCIDAD*/
float modulo_velocidad = 0;
/*VECTOR DIRECCION*/
float vector_direccion[3] = { 1, 0, 0 };
/* ANGULO */
float angulo = -45.0; // PARA ORIENTARLO AL PRINCIPIO DE LA CARRETERA
/* VARIABLES CARRETERA*/
extern float punto_en_x;
extern float punto_en_z;
extern float vector_t[2];
extern float vector_n[2];
/*VERTICES EXTERNOS QUAD*/
extern GLfloat v0[3];
extern GLfloat v1[3];
extern GLfloat v2[3];
extern GLfloat v3[3];
/*VERTICES INTERNOS QUAD (LINEA AMARILLA) */
extern GLfloat v0_interior[3];
extern GLfloat v1_interior[3];
extern GLfloat v2_interior[3];
extern GLfloat v3_interior[3];


/* este metodo solo se ejecuta una vez, los calculos y las creaciones
	   de poligonos deberían hacerse aqui en lugar de en display() */
void init() {
	glClearColor(1, 1, 1, 1);  //fijar el color de borrado (fondo)
	glColor3f(1, 0, 0); //fijar el color de dibujo
	glEnable(GL_DEPTH_TEST);//configurar motor de render
	// Desde aqui doy valores iniciales de la cámara. ( elegidos aleatoriamente )
	x = 0;
	y = 1;
	z = 0;
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
		titulo << PROYECTO << "   FPS = " << fotogramas;
		titulo << " VELOCIDAD = " << modulo_velocidad << " m/s";
		titulo << " ANGULO = " << angulo;
		titulo << " dir z= " << vector_direccion[Z_DIM];
		titulo << " dir x = " << vector_direccion[X_DIM];
		titulo << "x = " << x<< "z = " << z;
		glutSetWindowTitle(titulo.str().c_str());
		fotogramas = 0;
		antes = ahora;
	}

}


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // borrar el fondo y profundidad
	glMatrixMode(GL_MODELVIEW); // selecciono model-view
	glLoadIdentity(); // carga la identidad en el tope de la pila

	/*CALCULO A DONDE MIRO EN CADA MOMENTO*/
	glColor3f(0, 0, 0);
	// Situar y orientar la camara: punto de vista, punto de interes, vertical
	gluLookAt(x, y, z, x_a_la_que_miro, 0, z_a_la_que_miro, 0, 1, 0);

	ejes();
	
	calcula_punto_en_x_y_punto_en_z_iniciales(x);

	glPolygonMode(GL_FRONT, GL_LINE);
	for (int i = 0; i < 100; i++) {
		calcula_vectores_t_y_n_normalizados();

		calcula_vertice(VECTOR_1);
		calcula_vertice(VECTOR_2);
		calcula_vertice_interior(VECTOR_1);
		calcula_vertice_interior(VECTOR_2);

		if (i > 0) {  
			dibuja_carretera(QUAD_ARRIBA);
			dibuja_carretera(QUAD_ABAJO);
			dibuja_carretera(RAYA_CENTRAL_AMARILLA);	
		};

		/* LOS PUNTOS ACTUALES SERÁN LOS ANTERIORES EN LA SIGUIENTE ITERACION*/
		pasar_v1_a_v0();
		pasar_v2_a_v3();

		/*ACTUALIZAMOS X Y Z*/
		actualiza_punto_en_x_y_z();

		//cin.get();

	}
	glEnd();

	glutSwapBuffers(); // cambiado por flush
		// indicar que han acabado las ordenes de dibujo

	FPS();
}


void reshape(int w, int h) {
	glViewport(0, 0, w, h); //utiliza toda el area de dibujo
	float razon_aspecto = float(w) / float(h);

	// Matriz de la proyeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45, razon_aspecto, 0.1, 200); //defino una camara de perspectiva
}


void update() {
	//Fase de actualizacion
	// control del tiempo

	// solo calculo una vez la hora anterior inicial
	static int antes = glutGet(GLUT_ELAPSED_TIME); // tiempo en ms

	// HORA actual
	int ahora = glutGet(GLUT_ELAPSED_TIME);

	//tiempo transcurrido en segundos
	float tiempo_transcurrido = (float)(ahora - antes) / 1000.0;

	vector_direccion[X_DIM] = cos(angulo*PI/180);
	vector_direccion[Z_DIM] = -sin(angulo*PI/180);

	x_a_la_que_miro = x + 10 * vector_direccion[X_DIM];
	z_a_la_que_miro = z + 10 * vector_direccion[Z_DIM];

	//incremento = velocidad * tiempo 
	x += tiempo_transcurrido * modulo_velocidad * vector_direccion[X_DIM] ;
	z += tiempo_transcurrido * modulo_velocidad * vector_direccion[Z_DIM];
	
	//Actualizar para la proxima vez
	antes = ahora;

	// mandar evento redibujo
	glutPostRedisplay(); // redibujar
}

void onTimer(int tiempo) {
	update(); //Callback de atencion a la cuenta atras
	glutTimerFunc(tiempo, onTimer, tiempo); //encolo un nuevo timer
}

void onKey(unsigned char tecla, int x, int i) {
	//Callback de atencion al teclado

	/*switch (tecla) {
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
*/
	glutPostRedisplay(); // evento de redibujo con los cambios
}

void onSpecialKey(int tecla, int x, int y) {

	switch (tecla) {
	case GLUT_KEY_LEFT:
		angulo += 0.25;
		break;
	case GLUT_KEY_RIGHT:
		angulo -= 0.25;
		break;
	case GLUT_KEY_UP:
		modulo_velocidad += 0.1;
		break;
	case GLUT_KEY_DOWN:
		if (modulo_velocidad >= 0.1) {
			modulo_velocidad -= 0.1;
		}else{
			modulo_velocidad = 0;
		}
		break;
	}

	//glutPostRedisplay(); // REVISAR TECLA LANZA REDISPLAY
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv); // Reserva Buffers a usar (area de memoria para habilitar)
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // un buffer , buffer color y buffer profundidad
	glutInitWindowPosition(100, 200); //Posicionar la ventana de dibujo en la pantalla
	glutInitWindowSize(400, 400); // Ajustar tamanyo ventana
	glutCreateWindow(PROYECTO); //Crear la ventana

	/*Registro de las callbacks (funcion que se dispara cuando sucede algo)*/
	glutDisplayFunc(display); // Evento display -> Cuando hay que dibujar
	glutReshapeFunc(reshape); // Evento resize/reshape (cambio ventana (dimension etc)) NO NECESARIO
	glutTimerFunc(1000 / 50, onTimer, 1000 / 50); // cada 20 ms
	glutSpecialFunc(onSpecialKey); // Evento special??

	init(); // el init hace los calculos y los ajustes antes de dibujar
	std::cout << PROYECTO  " en marcha" << std::endl; // print por la salida estandar
	glutMainLoop(); //Poner en marcha el bucle de atencion a eventos
}