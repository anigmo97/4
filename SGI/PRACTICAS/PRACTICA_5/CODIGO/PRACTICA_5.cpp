/**
*	PRACTICA5.cpp
*	@author: anigmo@inf.upv.es
*	@version: v1.0 Oct,2018
*	@require: freeglut
*
**/


#include "LISTAS_DIBUJO_RELOJ.h" //INCLUYO UN ARCHIVO EN EL QUE TENGO OTROS DEFINES Y LAS LISTAS DE DIBUJO

#define PROYECTO "RELOJ 3D" // Lo usamos para mostrar un mensaje

/*VARIABLES DEFINIDAS EN OTROS FICHEROS*/
extern GLint marcas_grandes_reloj;
extern GLint marcas_pequeñas_reloj;
extern GLint circulo;
extern GLint aguja;
extern GLint segundero;
extern double angulo;

/*VALORES CAMARA*/
float x;
float y;
float z;
/* ANGULO A MARCAR EN CADA INSTANTE*/
float angulo_alfa_seg; 
float angulo_alfa_min;
float angulo_alfa_hor;
/*AUMENTO TAMAÑO CIRCULO*/
float aumento_circulo;
/*COLOR ALEATORIO*/
float cant_rojo;
float cant_verde;
float cant_azul;


/* ESTE METODO CREA EL RELOJ A EXCEPCION DE LAS AGUJAS */
void dibuja_estructura_reloj() {

	/* AÑADIMOS MARCAS AL RELOJ */
	glCallList(marcas_grandes_reloj); // llamamos a la lista de dibujo y se ejecutan sus acciones
	glCallList(marcas_pequeñas_reloj);

	/* CREAMOS CIRCULO PEQUEÑO EN MEDIO DEL RELOJ*/
	glPushMatrix();
		glColor3f(cant_rojo, cant_azul, cant_verde);
		glScalef(0.05+aumento_circulo, 0.05+aumento_circulo, 0.5+aumento_circulo); // z da igual
		glCallList(circulo); // llamamos a la lista de dibujo y se ejecutan sus acciones
	glPopMatrix();

	/* CREAMOS CIRCULO INTERIOR */
	glPushMatrix();
		glColor3f(1, 1, 1);
		glScalef(0.9, 0.9, 0.5); // z da igual
		glCallList(circulo); // llamamos a la lista de dibujo y se ejecutan sus acciones
	glPopMatrix();

	/* CREAMOS CIRCULO EXTERIOR */
	glColor3f(1, 0.0, 0.0);
	glCallList(circulo); // llamamos a la lista de dibujo y se ejecutan sus acciones
}



void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // borrar el fondo y profundidad z-buffer
	glMatrixMode(GL_MODELVIEW); // selecciono model-view
	glLoadIdentity(); // carga la identidad en el tope de la pila

	// Situar y orientar la camara: punto de vista, punto de interes, vertical
	gluLookAt(x, y, z, 0, 0, 0, 0, 1, 0);

	/* DIBUJAMOS SEGUNDERO */
	glPushMatrix();
		glRotatef(angulo_alfa_seg, 0, 0, 1);
		glCallList(segundero);
	glPopMatrix();

	/* DIBUJAMOS AGUJA HORAS */
	glPushMatrix();
		glColor3f(0.3, 0.3, 0.3);
		glScalef(0.5, 0.5, 0.5); // z da igual
		glRotatef(angulo_alfa_hor, 0, 0, 1);
		glCallList(aguja); // llamamos a la lista de dibujo y se ejecutan sus acciones
	glPopMatrix();

	/*DIBUJAMOS AGUJA MINUTOS*/
	glPushMatrix();
		glColor3f(0, 0, 0);
		glRotatef(angulo_alfa_min, 0, 0, 1);
		glCallList(aguja);
	glPopMatrix();

	//ejes(); // instruccion auxiliar para ver el eje x , y , z 
	// DIBUJA TODA LA ESTRUCTURA DEL RELOJ
	dibuja_estructura_reloj(); 

	glutSwapBuffers(); // indicar que han acabado las ordenes de dibujo
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h); //utiliza toda el area de dibujo
	float razon_aspecto = float(w) / float(h);
	float apertura;
	float distancia;

	
	glMatrixMode(GL_PROJECTION);// Matriz de la proyeccion
	glLoadIdentity();

	/* HACER QUE LA ESFERA SE AJUSTE AL TAMAÑO DE LA VENTANA (TOQUE ARRIBA Y ABAJO)  */
	distancia = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	apertura = asin(1 / distancia) * 2;
	apertura = (180 / PI) * apertura;

	gluPerspective(apertura, razon_aspecto, 0.1, 20); //defino una camara de perspectiva
}


void update() {
	//Fase de actualiacion
	
	/*VARIABLES DE LA HORA INICIAL    STATIC ->Solo se inicializan la primera vez   CONST -> Solo lectura*/
	static const time_t t = time(0);
	static const struct tm* antes = localtime(&t); //obtenemos la hora en nuestra zona horaria

	static const int horas_antes = (antes->tm_hour) % 12; // El reloj solo marca 12 posiciones
	static const int min_antes = antes->tm_min;
	static const int seg_antes = antes->tm_sec;

	static const float angulo_alfa_seg_ini = -(360 * (1 / 60.0)*seg_antes);
	static const float angulo_alfa_min_ini = -(360 * (1 / 60.0)*min_antes);
	static const float angulo_alfa_hor_ini = -(360 * (1 / 12.0)*horas_antes);


	/* OBTENEMOS EL TIEMPO DE AHORA */
	time_t tf = time(0); // NO NECESITAMOS EL LOCALTIME PORQUE SOLO LO USAMOS PARA CALCULAR INCREMENTO

	int segs_transcurridos = (int)difftime(tf, t); //tiempo transcurrido en segundos
	int mins_transcurridos = ((segs_transcurridos+seg_antes)/60); //sumamos los seg_antes para que aumente el minuto cuando el segundero pase por 12
	int horas_transcurridas = (mins_transcurridos/60);

	/*SE FUERZA A POSICIONES ENTERAS PARA QUE SE MUEVAN CADA SEGUNDO, MINUTO Y HORA Y NO DE FORMA GRADUAL*/
	// ANGULOS NEGATIVOS -> sentido agujas reloj
	angulo_alfa_seg = angulo_alfa_seg_ini -((360/60) * segs_transcurridos);  //ANGULO = ANGULO_INICIAL + ANGULO_INCREMENTO 
	angulo_alfa_min = angulo_alfa_min_ini -((360/60) * mins_transcurridos);  //ANGULO = ANGULO_INICIAL + ANGULO_INCREMENTO
	angulo_alfa_hor = angulo_alfa_hor_ini -((360/12) * horas_transcurridas); //ANGULO = ANGULO_INICIAL + ANGULO_INCREMENTO
	
	/*NUMERO ALEATORIO AUMENTO CIRCULO*/
	aumento_circulo = (rand() % 10)/100.0;

	/*COLOR ALEATORIO CIRCULO*/
	cant_rojo = (rand() % 5) / 10.0;
	cant_verde = (rand() % 5) / 10.0;
	cant_azul = (rand() % 5) / 10.0;

	/*ENVIAMOS EVENTO DE REDIBUJO*/
	glutPostRedisplay();
}

/* este metodo solo se ejecuta una vez, los calculos y las creaciones
	   de poligonos deberían hacerse aqui en lugar de en display() */
void init() {

	glClearColor(1, 1, 1, 1);  //fijar el color de borrado (fondo)
	glColor3f(1, 0, 0); //fijar el color de dibujo

	/*INICIALIZACION LISTAS DE DIBUJO*/
	crea_lista_dibujo_marcas_grandes_reloj();
	crea_lista_dibujo_marcas_pequeñas_reloj();
	crea_lista_dibujo_circulo();
	crea_lista_dibujo_aguja();
	crea_lista_dibujo_segundero();

	glEnable(GL_DEPTH_TEST);//configurar motor de render
	// Desde aqui doy valores iniciales de la cámara. ( elegidos aleatoriamente )
	x = 0;
	y = 0;
	z = 3;
}

/*CALLBACK ATENCION CUENTA ATRAS*/
void onTimer(int tiempo) {
	update();
	glutTimerFunc(tiempo, onTimer, tiempo);//encolo un nuevo timer
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv); // Reserva Buffers a usar (area de memoria para habilitar)
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // un buffer , buffer color y buffer profundidad (z buffer)
	glutInitWindowPosition(100, 200); //Posicionar la ventana de dibujo en la pantalla
	glutInitWindowSize(400, 400); // Ajustar tamanyo ventana
	glutCreateWindow(PROYECTO); //Crear la ventana

	/*Registro de las callbacks (funcion que se dispara cuando sucede algo)*/
	glutDisplayFunc(display); // Evento display -> Cuando hay que dibujar
	glutReshapeFunc(reshape); // Evento resize/reshape (cambio ventana (dimension etc))
	glutTimerFunc(1000 / 20, onTimer, 1000 / 20); // cada 20 ms

	init(); // el init hace los calculos y los ajustes antes de dibujar
	std::cout << PROYECTO  " en marcha" << std::endl; // print por la salida estandar
	glutMainLoop(); //Poner en marcha el bucle de atencion a eventos
}