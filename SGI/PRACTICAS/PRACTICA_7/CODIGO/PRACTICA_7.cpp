/**
*	PRACTICA7.cpp
*	@author: anigmo@inf.upv.es
*	@version: v1.0 Dic,2018
*	@require: freeglut
*
**/

#define PROYECTO "PRACTICA_7 CARRETERA + ILUMINACION" // Lo usamos para mostrar un mensaje

#include "CARRETERA.h" // calculo y dibujo carretera
#include "CONTROL_FPS.h" // controlo los fps y printo titulo
#include "LUCES.h" // CONTROLA LA ILUMINACION
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

ESTADO_DIA estado_dia = DIA;
RENDERIZADO renderizado = SOLIDO;


/* este metodo solo se ejecuta una vez, los calculos y las creaciones
	   de poligonos deberían hacerse aqui en lugar de en display() */
void init() {
	glClearColor(0, 0, 0, 1);  //fijar el color de borrado (fondo)
	// Desde aqui doy valores iniciales de la cámara. ( elegidos aleatoriamente )
	x = 0;
	y = 1;
	z = 0;

	/* LUCES */
	inicializa_Luces();
	habilita_Luces();


	/* CARACTERISTICAS RENDER */
	glEnable(GL_LIGHTING); // CONFIGURAR CALCULO LUCES con esto saldran practicamente negras
	glEnable(GL_DEPTH_TEST);//configurar motor de render
	//glEnable(GL_COLOR_MATERIAL);
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // borrar el fondo y profundidad
	glMatrixMode(GL_MODELVIEW); // selecciono model-view
	glLoadIdentity(); // carga la identidad en el tope de la pila

	/* LUZ MINERO (VA CON LA CAMARA) FAROS COCHE*/
	setPosicion_y_direccion_faro();
	
	//glColor3f(0, 0, 0); ya no vale para nada

	// Situar y orientar la camara: punto de vista, punto de interes, vertical
	gluLookAt(x, y, z, x_a_la_que_miro, 0, z_a_la_que_miro, 0, 1, 0);

	// Posiciona y define el tipo de luz
	GLfloat posicion[] = { 0,10,0,0 };
	glLightfv(GL_LIGHT0, GL_POSITION, posicion); // Luz direccional

	ejes();

	calcula_punto_en_x_y_punto_en_z_iniciales(x);

	coloca_luces_farolas((int)punto_en_x-10);
	

	glPolygonMode(GL_FRONT, GL_LINE);
	for (int i = 0; i < NUM_TRAMOS + 1; i++) {
		calcula_vectores_t_y_n_normalizados();

		calcula_vertice(VECTOR_1);
		calcula_vertice(VECTOR_2);

		if (i > 0) {
			//MATERIAL CARRETERA
			set_material_carretera();
			/*void quadtex(GLfloat v0[3], GLfloat v1[3], GLfloat v2[3], GLfloat v3[3],
				GLfloat smin, GLfloat smax, GLfloat tmin, GLfloat tmax,
				int M, int N)*/
			dibuja_carretera(QUAD_COMPLETO);

		};

		/* LOS PUNTOS ACTUALES SERÁN LOS ANTERIORES EN LA SIGUIENTE ITERACION*/
		pasar_v1_a_v0();
		pasar_v2_a_v3();

		/*ACTUALIZAMOS X Y Z*/
		actualiza_punto_en_x_y_z();

		//cin.get();

	}
	glEnd();

	

	glutSwapBuffers(); // indicar que han acabado las ordenes de dibujo
		

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

	vector_direccion[X_DIM] = cos(angulo*PI / 180);
	vector_direccion[Z_DIM] = -sin(angulo*PI / 180);

	x_a_la_que_miro = x + 10 * vector_direccion[X_DIM];
	z_a_la_que_miro = z + 10 * vector_direccion[Z_DIM];

	//incremento = velocidad * tiempo 
	x += tiempo_transcurrido * modulo_velocidad * vector_direccion[X_DIM];
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

	switch (tecla) {

	case 'L':
	case 'l':
		if (estado_dia == DIA) {
			glClearColor(0, 0, 0, 1);  //fijar el color de borrado (fondo)
			glEnable(GL_LIGHTING);
			estado_dia = NOCHE;
		}else {
			glClearColor(1, 1, 1, 1);  //fijar el color de borrado (fondo)
			glDisable(GL_LIGHTING);
			estado_dia = DIA;
		}
		break;
	case 'S':
	case 's':
		if (renderizado == SOLIDO) {
			glClearColor(1, 1, 1, 1);  //fijar el color de borrado (fondo)
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			renderizado = ALAMBRICO;
		}
		else {
			glClearColor(0, 0, 0, 1);  //fijar el color de borrado (fondo)
			glEnable(GL_LIGHTING);
			glEnable(GL_TEXTURE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			renderizado = SOLIDO;
		}
		break;
	}

	//glutPostRedisplay(); // evento de redibujo con los cambios
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
		}
		else {
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
	glutKeyboardFunc(onKey);

	init(); // el init hace los calculos y los ajustes antes de dibujar
	std::cout << PROYECTO  " en marcha" << std::endl; // print por la salida estandar
	glutMainLoop(); //Poner en marcha el bucle de atencion a eventos
}